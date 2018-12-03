
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
If you wish to see verbose debug output, you can use `cmake -DCMAKE_BUILD_TYPE=Debug CMakeLists.txt` instead.
## Usage
```
Usage: mandelbrot-generator arguments...

Required arguments:
    --absolute (-A)
        Sets palette mode to PALETTE_ABSOLUTE.
        Cannot be combined with -R.
    --depth max_depth (-d)
        Maximum Mandelbrot iterations per pixel.
    --file output_file (-f)
        Path of the resulting PNG file.
    --image image_size (-i)
        Size of the resulting PNG.
        Format:  HEIGHTxWIDTH
        Example: '100x200' -> 100 pixels high, 200 pixels wide.
    --plane plane_specs (-p)
        Specifications of the complex plane.
        Format:  min_r/max_r/min_i/max_i
        Example: '-3.5/2.5/-3/3' -> Real axis from -3.5 to 2.5, imaginary from -3 to 3.
    --relative (-R)
        Sets palette mode to PALETTE_RELATIVE.
        Cannot be combined with -A.

Optional arguments:
    --limit abs_value (-l)
        The iteration for each c ends when abs(z_n(c)) > abs_value.
        Defaults to abs_value = 10.
    --threads num_threads (-t)
        Specifies the number of worker threads. Defaults to one.
    --palette palette_file (-P)
        Path of the color palette file.
        Defaults to linear grayscale if no palette is provided.
    --ssaa [factor] (-a)
        Enables supersample antialiasing, which will render factor*factor data
        points per pixel. `factor` defaults to 2.
```

### `PALETTE_ABSOLUTE` and `PALETTE_RELATIVE`
These two modes result in different color schemes at high iteration counts:
- `PALETTE_ABSOLUTE` will assign the first color palette entry to 1 and the highest to the maximum value.
- `PALETTE_RELATIVE` will assign the first color palette entry to the lowest value and the highest to the maximum value.

## Examples
An 800x800 image of the complex plane; the real axis ranges from -3.5 to 2.5 and the imaginary axis ranges from -3 to 3:
```
mandelbrot-generator -A -f sample-1.png -i 800x800 -l 1000 -p -3.5/2.5/-3/3 -d 50
```
![Sample image 1](doc/images/sample-1.png)


A 400x800 image of the complex plane; the real axis ranges from -1 to 1 and the imaginary axis ranges from 0 to 1:
```
mandelbrot-generator -A -f sample-2.png -i 400x800 -l 100 -p -1/1/0/1 -d 75
```
![Sample image 2](doc/images/sample-2.png)


An 800x800 image of the complex plane; the real axis ranges from -0.413 to -0.338 and the imaginary axis ranges from 0.623 to 0.698:
```
mandelbrot-generator -A -f sample-3.png -i 800x800 -l 10 -p -0.413/-0.338/0.623/0.698 -d 150 -P palettes/rgb-linear-red.txt
```
![Sample image 3](doc/images/sample-3.png)

### Comparison of `PALETTE_RELATIVE` and `PALETTE_ABSOLUTE`
TBA...