# GridTemplate
A Effective template for making 2D grids using SDL2

To run the code write following in terminal:
  1) "make"
  2) "./play"

In order to midify the **GridUpdate** medthod in *main.c*:

Pixel is a struct, it holds RGBA values, reprecenting a pixel in the texture.

In GridUpdate tempGrid is a array with GRID_SIZE of Pixel, COLUMNS can be used in order to interpret tempGrid as a 2D array.

Change ROWS and COLUMNS in order to change the size of each pixel.
