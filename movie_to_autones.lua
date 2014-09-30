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

while (true) do
    -- If a movie is loaded into the emulator
    if (movie.active() == true) then
        -- When a movie is loaded for the first time, we need to do some setup
        if (movie_loaded == false) then
            -- First, restart the movie at the beginning
            movie.playbeginning();
            
            -- Lets make up the output filename
            -- Take the video name, remove the .fm2 and replace with .txt
            movie_filename = movie.getname();
            output_filename = string.sub(movie_filename, 0, string.len(movie_filename)-4) .. ".txt";
            
            -- Print it out for debugging
            print(output_filename);
            
            -- Setup the file handle to write to it
            handle = io.open(output_filename, "w");
	    handle:write("{");
            
            -- Now we are ready to go.
            movie_loaded = true;
        end;
        
        -- There are some issues with starting this code, so we pretend to skip the first frame
        -- I believe the problem is because when the video is loaded it is at frame 0 which has no input
        -- We need to skip this frame, look for lag, then start watching for input
        frame_number = movie.framecount();
        if (frame_number > 1)
        then
            -- We need to skip any lag frames and only output frames where the console is looking for input
            -- if (lagged == true)
            -- then
            --     handle:write("");
            -- else
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
		if (number == previous_number)
		then
		    run_count = run_count + 1;
		else
                    handle:write("{"..run_count..","..previous_number.."},");
		    previous_number = number;
		    run_count = 1;
		end;
            -- end;
        end;
        
        -- Check for lag frames and record it
        if (FCEU.lagged() == true)
        then
            lagged = true;
        else
            lagged = false;
        end;
        
        -- Also read the input for the previous frame
        previous_input = joypad.get(1);
      
    else
        -- If the movie has ended, then our work here is done. Clean up
        if (movie_loaded == true) then
            handle:write("{"..run_count..","..previous_number.."}}");
            handle:close();
            print("DONE");
            movie_loaded = false;
            frame = 0;
        end;
    end;
    
    -- Mandatory
    FCEU.frameadvance();
end;
