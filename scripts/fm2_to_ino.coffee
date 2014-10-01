fs = require 'fs'
CSV = require 'comma-separated-values'
bytes_saved = 0
movie_buttons = []
movie_frames = []

fs.readFile process.argv[2], {encoding: 'UTF8'}, (err, data) ->
    csv = new CSV data, {cellDelimiter: '|'}
    csv = csv.parse()
    ino = "{"
    prev_buttons = csv[0][2]
    run_count = 1
    for [_, _, buttons, _...] in csv
      if buttons is prev_buttons
        run_count++
      else
        bytes_saved += (run_count-4)
        movie_buttons.push buttonstring_to_integer(prev_buttons)
        movie_frames.push run_count
        prev_buttons = buttons
        run_count = 1
    console.log "PROGMEM const uint8_t buttons[] = {#{movie_buttons}};"
    console.log "PROGMEM const uint16_t frames[] = {#{movie_frames}};"
    #console.log "#{bytes_saved} bytes saved with RLE"

buttonstring_to_integer = (buttonstring) ->
  number = 0
  for character,index in buttonstring
    #console.log character,index
    if character isnt '.'
      number += (1 << (7-index))

  number
