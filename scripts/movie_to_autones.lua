local movie_loaded = false -- This tells if the program has initialized for the movie
local movie_filename = "" -- Will hold the movie file name (.fm2)
local output_filename = "" -- Will hold the output file name (.txt)
local handle -- For ouputing to the file

local lagged = false
local previous_input = {}

local run_count = 0
local previous_number = 0

local skip_lag = true
local frame = 0

local frames = {}
local buttons = {}

memory.register(0x4016, record_latch)

while true do
  -- If a movie is loaded into the emulator
  if (movie.active()) then
    previous_input = joypad.get(1)
    -- When a movie is loaded for the first time, we need to do some setup
    if (movie_loaded == false) then
      setup_recording()
    else
      write_movie()
    end

    lagged = FCEU.lagged()
    FCEU.frameadvance()
  end
end

function setup_recording ()
  -- First, restart the movie at the beginning
  movie.playbeginning()
  movie_filename = movie.getname()
  output_filename = string.sub(movie_filename, 0, string.len(movie_filename)-4) .. ".txt"
  print(output_filename)
  handle = io.open(output_filename, "w")

  -- Now we are ready to go.
  movie_loaded = true;
end

function record_latch ()
  if memory.read(0x4016) ~= 0 then return end -- only record on a falling edge
  if lagged then return end -- Skip any lag frames (where console is not looking for input?)

  frame = frame + 1

  -- Convert the buttons from the movie into byte format
  local number = 0
  -- Order of buttons: RLDUTSBA
  if previous_input.right then number = OR(number, BIT(7)) end
  if previous_input.left then number = OR(number, BIT(6)) end
  if previous_input.down then number = OR(number, BIT(5)) end
  if previous_input.up then number = OR(number, BIT(4)) end
  if previous_input.start then number = OR(number, BIT(3)) end
  if previous_input.select then number = OR(number, BIT(2)) end
  if previous_input.B then number = OR(number, BIT(1)) end
  if previous_input.A then number = OR(number, BIT(0)) end

  -- Store it to the file
  if number == previous_number then
    run_count = run_count + 1
  else
    table.insert(frames, run_count)
    table.insert(buttons, previous_number)
    previous_number = number
    run_count = 1
  end;
end

function write_movie()
  handle:write("PROGMEM const uint16_t frames[] = {")
  for i, frame in ipairs(frames) do
    handle:write(frame..",")
  end
  handle:write("0};\n\n")
  handle:write("PROGMEM const uint8_t buttons[] = {")
  for i, button in ipairs(buttons) do
    handle:write(button..",")
  end
  handle:write("0};")
  handle:close()
  print("DONE")
  movie_loaded = false
  frame = 0
end
