#ifndef MANDELBROT_COMPLEX_H
#define MANDELBROT_COMPLEX_H

#include <stdlib.h>
#include <complex.h>

// A complex plane, divided into `pixel_width` by `pixel_height` units.
typedef struct
{
	size_t pixel_width;
	size_t pixel_height;
	double real_min;
	double real_max;
	double imaginary_min;
	double imaginary_max;
} complex_plane_t;

// Allocate a new complex plane. Must be freed with free()!
complex_plane_t *create_complex_plane(int height, int width, double real_min, double real_max, double imaginary_min,
									  double imaginary_max);

// Converts a pixel coordinate to a complex number on the complex plane.
// This method will return the coordinate at the center (horizontally and vertically) of the given pixel.
complex double coordinate_to_complex(complex_plane_t* plane, int x, int y, unsigned int ssaa_scale);

#endif //MANDELBROT_COMPLEX_H