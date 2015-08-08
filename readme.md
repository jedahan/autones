Make your arduino a programmable NES/famicom controller.

Signal | Arduino | Famicom | NES
-------|---------|---------|----
Ground |  GND    | 1       | 1
Clock  |  D3     | 14      | 2
Latch  |  D2     | 12      | 3
Data   |  D8     | 13      | 4


The NES Controller Port looks like this:

![](nes-controller-pinout.png)

The famicom Expansion Port looks like this:

![](famicom-expansion-pinout.jpg)

### Converting fm2 to ino movie

    fceux --loadlua scripts/movie_to_autones.lua --playmov movies/micro_ghosts_n_goblins.fm2 roms/ghosts_n_goblins.zip

