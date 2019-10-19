# Oscillating-Avian
![Osclillating-Avian](https://raw.githubusercontent.com/mkrupczak3/Oscillating-Avian/master/OscillatingAvian-191019-112319.png "Oscillating Avian")

![Oscillating-Avian](https://raw.githubusercontent.com/mkrupczak3/Oscillating-Avian/master/OscillatingAvian-191019-112327.png "Oscillating Avian")

This is a re-upload of a Game Boy Advance game I made as part of a class assignment in the summer of 2017. I wrote it in C, running "bare metal" on the Game Boy Advance with no operating system, utilities, etc.

## Play me!
1. Play the game (.gba file) [here](https://gba.ninja/?autorun=https://raw.githubusercontent.com/mkrupczak3/Oscillating-Avian/master/OscillatingAvian.gba&exclusive)


## NOTES:

Most of the display work is done through Mode 3 and Direct Memory Access (DMA) (for the image, background, etc) as per the scope of this assignment. This is much more demanding on the GBA than doing sprite-based graphics, so some performance considerations were made to keep the game running smoothly.


The vertical pipe obstacles are generated by DMA'ing a green pipe, and a trailing blue background color behind it constantly to keep the pipe moving to the left while "erasing" its previous position from the framebuffer in memory.


The score text is only drawn after every "screen wipe" by the pipes rather than every frame. The score text draws pixel-by-pixel and would have been too expensive to re-draw every frame without slowdown or complicated optimization.


Because the GBA doesn't have native floating point capability (i.e. decimal numbers for movement speeds, etc.), the bird's velocity is only updated on certain frames as to allow for a kind of "fractional" change in velocity over time. This means that gravity and velocity changes from movement input only "kick in" on certain frame intervals. I definitely would have implemented input buffering of some kind if I had more people playtest this before completion.
