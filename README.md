# Mandlebrot Viewer #

Created by Zachary Russ

## Built Targets

Please do not use **movie**, **vid**, or **pic**. These were used to create the images and video seen in the project report. They require a certain directory structure, as well as a few dependencies.

- **default**: build for viewing/saving monochromatic images
- **color**: build for viewing/saving full-color images using the third coloring technique described previously
- **slow**: build for viewing/saving monochromatic images using long doubles for more precision (no difference?)
- **movie**: build for rendering a sequence of 1800 images each with increasing iterations
- **pic**: build for saving a single full-color image
- **vid**: build movie target, convert PPMs to PNGs with magick, and create a 30 second MP4 using ffmpeg (requires magick and ffmpeg)
- **clean**: remove *mandelbrot* and all PPMs/PNGs

## Exploring the Mandelbrot Set

- **`./mandelbrot`**: start the program
- **W/A/S/D**: Move Up, Left, Down, and Right
- **Q/E**: Zoom Out and Zoom In
- **R**: Decrease Image Resolution (Increases Number of Iterations)
- **F**: Increase Image Resolution (Decreases Number of Iterations)
- **G**: Toggle Adaptive Pixel Skip (Chunk Subdividing Drawing Technique)
- **B**: Toggle Bilinear Interpolation
- **O**: Write Current Window to `out.ppm`
