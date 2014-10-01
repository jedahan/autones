Make your arduino a programmable NES controller.

 Arduino | NES Controller
---------|----------------
      D2 | Latch
      D3 | Clock
      A3 | Data	

The NES Controller Port looks like this:

![](nes-controller-pinout.png)

### Converting fm2 to ino movie

#### The emulator way:

Record a movie in [fceux](www.fceux.com), then open the [movie_to_autones.lua](movie_to_autones.lua), and play back the movie.
It should create a <moviename>.txt file, which you can then copy to the header of the autones.cpp file.

#### The script way:

We first need to install the node libraries:

    cd scripts
    npm install

Now we need to filter the header junk from the fm2

    grep '||' my.fm2 > filtered.fm2

Now we can copy the output to a buffer, and paste the result in the header of autones.cpp

    npm start
