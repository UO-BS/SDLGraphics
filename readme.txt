SDLGraphics project

Attempting to create a graphical engine using SDL to be used in some of my other projects.

This project is magnitudes less efficient than off-the-shelf software, and should not be used for any production.
This is a passion project, so I'm only working on it when I have some free time

A useful resource is: https://lazyfoo.net/tutorials/SDL/

TODO:
Consider using more OpenGL (SDL only for window management, not rendering):
 - Look into GLM - I should probably keep using mine since its a nice challenge
 - Will have to rework the texture manager (opengl does not use SDL_Texture*, it just passes you an int)
 - GGraphicalComponent and its derivatives are likely useless now... 
 - MainWindow: Instead of doing matrix math on CPU, I pass the matrixes to OpenGL to do it for me
 - MainWindow: My clipping algorithm is useless... OpenGL does it automatically
 - MainWindow: The draw loop will have to be completely redesigned 