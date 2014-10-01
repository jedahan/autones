fs = require 'fs'
CSV = require 'comma-separated-values'

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
        ino += "{#{run_count},#{buttonstring_to_integer(prev_buttons)}},"
        prev_buttons = buttons
        run_count = 1
    console.log ino[0..ino.length-2] + "}"

buttonstring_to_integer = (buttonstring) ->
  number = 0
  for character,index in buttonstring
    if character isnt '.'
      number += (1 << index)

  number
