# A7 Sprite Editor
#Authors: Devin White, Xuyen Nguyen, Taylor Adamson, Ansam Al Sharif

Features:

  Open, Save, and New File:
    Open: Allows the user to Open a sprite file with the format .ssp
    Save: allows a user to save their sprite as a file with the format .ssp
    New File: Opens a fresh sprite. Will prompt the user if they haven't saved

  Tools:

  Pencil: Draws pixels in the users chosen color. Follows mouse cursor
  Eraser: Makes pixels transparent(removes them). Follows mouse cursor
  Paint bucket: fills area in the users chosen color, bounded by different colored pixels but not transparent ones
  Move tool: Allows the user to move the canvas anywhere within the GUI
  eyedropper: changes the color of the pencil and paintbucket to the selected color
  Zoom slider: allows magnification of the sprite, ranging from 1:1 to 32x
  color pallette: will change the color of the paint bucket and pencil to the selected collor

Preview window:

  Previews the currently selected frame. If another frame in the list is single clicked, will preview that
  Actual size: displays the sprite in the preview window in its actual size
  Play animation: plays the sprite animation
  Loop animation: loops through the animation. Will play until untoggled
  Animate on canvas: Will play the animation on the canvas, as well as in the preview window
  FPS: sets the frames per second of the animation, ranging from 1 to 99

Frame List:

  Displays a list of all the current frames in the sprite.
  Single click: will change the sprite shown in the preview window to the selected frame
  Double click: will change the current frame(canvas) to the selected frame
  Add frane: Will add a frame to the list, after the currently selected frame
  Remove frame: will remove the currently selected frame from the list

Sprite size adjustments

  Allows the size of the frame to be adjusted, width and height independently, from 2 to 128
  Sprite width: Adjusts the width of the sprite, from 2 to 128
  Sprite height: Adjusts the height of the sprite, from 2 to 128
