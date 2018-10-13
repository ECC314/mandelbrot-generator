
# Mandelbrot Generator

## Setup

This project relies on the following programs and libraries:
- [CMake](https://cmake.org)
- [libpng](http://libpng.org/pub/png/libpng.html)

Compiling the source code:
```
$ cmake CMakeLists.txt
$ make
```
## Usage

```
Usage: mandelbrot-generator -d depth -f output_file -i image_size -l limit -p plane_specs
    depth        Number of iterations per pixel.
    output_file  Path of the resulting PNG file.
    image_size   Size of the resulting PNG.
                 Format:  HEIGHTxWIDTH
                 Example: 100x200
                 -> 100 pixels high, 200 pixels wide.
    limit        Limit that must be exceeded by abs(z_n(c)).
    plane_specs  Specifications of the complex plane.
                 Format:  min_r/max_r/min_i/max_i
                 Example: -3.5/2.5/-3/3
                 -> Real axis from -3.5 to 2.5, imaginary from -3 to 3.
```
## Examples
An 800x800 image of the complex plane; the real axis ranges from -3.5 to 2.5 and the imaginary axis ranges from -3 to 3:
```
./mandelbrot-generator -f sample-1.png -i 800x800 -l 1000 -p -3.5/2.5/-3/3 -d 50
```
![Sample image 1](doc/images/sample-1.png)


A 400x800 image of the complex plane; the real axis ranges from -1 to 1 and the imaginary axis ranges from 0 to 1:
```
./mandelbrot-generator -f sample-2.png -i 400x800 -l 100 -p -1/1/0/1 -d 75
```
![Sample image 2](doc/images/sample-2.png)


An 400x800 image of the complex plane; the real axis ranges from -0.413 to -0.338 and the imaginary axis ranges from 0.623 to 0.698:
```
./mandelbrot-generator -f sample-3.png -i 800x800 -l 10 -p -0.413/-0.338/0.623/0.698 -d 150
```
![Sample image 2](doc/images/sample-3.png)


## Future Plans
- Code cleanup
- Multithreading support
- Color palettes
    - More grayscale options
    - RGB
