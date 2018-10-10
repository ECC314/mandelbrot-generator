#ifndef MANDELBROT_COMPLEX_H
#define MANDELBROT_COMPLEX_H

#include <stdio.h>
#include <stdlib.h>

// A complex plane, divided into `pixel_width` by `pixel_height` units.
typedef struct
{
	int pixel_width;
	int pixel_height;
	double real_min;
	double real_max;
	double imaginary_min;
	double imaginary_max;
} complex_plane_t;

// A complex number `a + ib`, saved as `real` (a) and `imaginary` (b).
typedef struct
{
	double real;
	double imaginary;
} complex_t;

// Obtain Mandelbrot iteration z_(n+1) from z_n and c.
complex_t next_mandelbrot_iteration(complex_t z, complex_t c);

// Returns the index n of the first Mandelbrot iteration of c where abs(z_n) is greater than `limit`.
// Returns 0 if the series does not exceed `limit` within `max_iterations` steps.
int mandelbrot_iteration_exceeds_limit(complex_t c, double limit, int max_iterations);

// Allocate a new complex plane. Must be freed with free()!
complex_plane_t *create_complex_plane(int height, int width, double real_min, double real_max, double imaginary_min,
									  double imaginary_max);

// Converts a pixel coordinate to a complex number on the complex plane.
// This method will return the coordinate at the center (horizontally and vertically) of the given pixel.
complex_t coordinate_to_complex(complex_plane_t* plane, int x, int y);

#endif //MANDELBROT_COMPLEX_H