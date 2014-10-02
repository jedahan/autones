Make your arduino a programmable NES controller.

 Arduino | NES Controller
---------|----------------
      D2 | Latch
      D3 | Clock
      A3 | Data	

The NES Controller Port looks like this:

![](nes-controller-pinout.png)

### Converting fm2 to ino movie

    fceux --loadlua scripts/movie_to_autones.lua --playmov movies/micro_ghosts_n_goblins.fm2 roms/ghosts_n_goblins.zip


