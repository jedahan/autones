Make your arduino a programmable NES/famicom controller.

<blockquote class="instagram-media" data-instgrm-version="5" style=" background:#FFF; border:0; border-radius:3px; box-shadow:0 0 1px 0 rgba(0,0,0,0.5),0 1px 10px 0 rgba(0,0,0,0.15); margin: 1px; max-width:658px; padding:0; width:99.375%; width:-webkit-calc(100% - 2px); width:calc(100% - 2px);"><div style="padding:8px;"> <div style=" background:#F8F8F8; line-height:0; margin-top:40px; padding:50% 0; text-align:center; width:100%;"> <div style=" background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACwAAAAsCAMAAAApWqozAAAAGFBMVEUiIiI9PT0eHh4gIB4hIBkcHBwcHBwcHBydr+JQAAAACHRSTlMABA4YHyQsM5jtaMwAAADfSURBVDjL7ZVBEgMhCAQBAf//42xcNbpAqakcM0ftUmFAAIBE81IqBJdS3lS6zs3bIpB9WED3YYXFPmHRfT8sgyrCP1x8uEUxLMzNWElFOYCV6mHWWwMzdPEKHlhLw7NWJqkHc4uIZphavDzA2JPzUDsBZziNae2S6owH8xPmX8G7zzgKEOPUoYHvGz1TBCxMkd3kwNVbU0gKHkx+iZILf77IofhrY1nYFnB/lQPb79drWOyJVa/DAvg9B/rLB4cC+Nqgdz/TvBbBnr6GBReqn/nRmDgaQEej7WhonozjF+Y2I/fZou/qAAAAAElFTkSuQmCC); display:block; height:44px; margin:0 auto -44px; position:relative; top:-22px; width:44px;"></div></div><p style=" color:#c9c8cd; font-family:Arial,sans-serif; font-size:14px; line-height:17px; margin-bottom:0; margin-top:8px; overflow:hidden; padding:8px 0 7px; text-align:center; text-overflow:ellipsis; white-space:nowrap;"><a href="https://instagram.com/p/tlbn_ERxJx/" style=" color:#c9c8cd; font-family:Arial,sans-serif; font-size:14px; font-style:normal; font-weight:normal; line-height:17px; text-decoration:none;" target="_blank">A video posted by @jedahan</a> on <time style=" font-family:Arial,sans-serif; font-size:14px; line-height:17px;" datetime="2014-09-30T21:33:03+00:00">Sep 30, 2014 at 2:33pm PDT</time></p></div></blockquote>
<script async defer src="//platform.instagram.com/en_US/embeds.js"></script>

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

### Converting fm2 to ino movie manually

    fceux --loadlua scripts/movie_to_autones.lua --playmov movies/micro_ghosts_n_goblins.fm2 roms/ghosts_n_goblins.zip

## Building the docker image

    docker-machine create --driver virtualbox dev
    eval "$(docker-machine env dev)"

    make fceuxos

## Creating a .ino file from the docker image

    make
