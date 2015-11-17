Make your arduino a programmable NES/famicom controller.

[![Autones on ghosts n goblins](https://j.gifs.com/yN4MZ5.gif)](https://youtu.be/DHVvrFHG_10)

Signal | Arduino | Famicom | NES | color
-------|---------|---------|-----|------
Ground |  GND    | 1       | 1   | brown
Clock  |  D3     | 14      | 2   | red
Latch  |  D2     | 12      | 3   | orange
Data   |  D8     | 13      | 4   | yellow
Power  |  Aref   | 15      | 7   | white


The NES Controller Port looks like this:

![](nes-controller-pinout.png)

The famicom Expansion Port looks like this:

![](famicom-expansion-pinout.jpg)

### Converting fm2 to ino movie manually

    fceux --loadlua scripts/movie_to_autones.lua --playmov movies/micro_ghosts_n_goblins.fm2 roms/ghosts_n_goblins.zip

## Building the docker image

    docker-machine create --driver virtualbox dev
    eval "$(docker-machine env dev)"

    make fceuxos

## Creating a .ino file from the docker image

    make
