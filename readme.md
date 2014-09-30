Make your arduino a programmable NES controller.

 Arduino | NES Controller
---------|----------------
      D2 | Latch
      D3 | Clock
      A3 | Data	

The NES Controller Port looks like this:

![](nes-controller-pinout.png)

Record a movie in [fceux](www.fceux.com), then open the [movie_to_autones.lua](movie_to_autones.lua), and play back the movie.
It should create a <moviename>.txt file, which you can then copy to the header of the autones.cpp file.
