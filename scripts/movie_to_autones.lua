local movie_loaded = false; -- This tells if the program has initialized for the movie
local movie_filename = ""; -- Will hold the movie file name (.fm2)
local output_filename = ""; -- Will hold the output file name (.txt)
local handle; -- For ouputing to the file

local lagged = false;
local previous_input = {}

local run_count = 0;
local previous_number = 0;

local frame_number = 0;

local skip_lag = true;
local frame = 0;

local frames = {};
local buttons = {};

memory.register(0x4016, record_latch)

while (true) do
  -- If a movie is loaded into the emulator
  if (movie.active() == true) then
    -- When a movie is loaded for the first time, we need to do some setup
    if (movie_loaded == false) then
      -- First, restart the movie at the beginning
      movie.playbeginning();
      movie_filename = movie.getname();
      output_filename = string.sub(movie_filename, 0, string.len(movie_filename)-4) .. ".txt";
      print(output_filename);
      handle = io.open(output_filename, "w");

      -- Now we are ready to go.
      movie_loaded = true;
    end;

    -- Check for lag frames and record it
    lagged = FCEU.lagged()
    previous_input = joypad.get(1);

    -- we have finished the movie
    if (movie_loaded == true) then
      write_movie()
    end

    FCEU.frameadvance();
  end
end

function record_latch ()
  value = memory.read(0x4016)
  print(value)
  if (value == 0) then -- we have latched down
    -- There are some issues with starting this code, so we pretend to skip the first frame
    -- I believe the problem is because when the video is loaded it is at frame 0 which has no input
    -- We need to skip this frame, look for lag, then start watching for input
    frame_number = movie.framecount();
    if (frame_number > 1) then
      -- We need to skip any lag frames and only output frames where the console is looking for input
      if (lagged ~= true) then
       frame = frame + 1;

        -- Convert the buttons from the movie into byte format and store it to the file
        local number = 0;
        -- Order of buttons: RLDUTSBA
        if (previous_input.right == true)  then number = OR(number,BIT(7)); end;
        if (previous_input.left == true)   then number = OR(number,BIT(6)); end;
        if (previous_input.down == true)   then number = OR(number,BIT(5)); end;
        if (previous_input.up == true)     then number = OR(number,BIT(4)); end;
        if (previous_input.start == true)  then number = OR(number,BIT(3)); end;
        if (previous_input.select == true) then number = OR(number,BIT(2)); end;
        if (previous_input.B == true)      then number = OR(number,BIT(1)); end;
        if (previous_input.A == true)      then number = OR(number,BIT(0)); end;

        if (number == previous_number) then
          run_count = run_count + 1;
	      else
          table.insert(frames,run_count);
          table.insert(buttons,previous_number);
          previous_number = number;
          run_count = 1;
	      end;
      end
    end
  end
end

function write_movie()
  handle:write("PROGMEM const uint16_t frames[] = {");
  for i, frame in ipairs(frames) do
    handle:write(frame..",");
  end
  handle:write("0};\n\n")
  handle:write("PROGMEM const uint8_t buttons[] = {");
  for i, button in ipairs(buttons) do
    handle:write(button..",");
  end
  handle:write("0};")
  handle:close();
  print("DONE");
  movie_loaded = false;
  frame = 0;
end
