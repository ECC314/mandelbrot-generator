#ifndef MANDELBROT_COMPLEX_H
#define MANDELBROT_COMPLEX_H

#include <stdlib.h>

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

// A complex number `a + ib`, saved as `real` (a) and `imaginary` (b).
typedef struct
{
	double real;
	double imaginary;
} complex_t;

// Returns the sum of two complex numbers.
complex_t complex_add(complex_t z1, complex_t z2);

// Returns the absolute value of a complex number.
// abs(z) = sqrt(Re(z)^2 + Im(z)^2)
double complex_absolute(complex_t z);

// Returns the square of a complex number.
// z^2 = (a + ib)^2 = a^2 + 2iab - b^2
complex_t complex_square(complex_t z);

// Allocate a new complex plane. Must be freed with free()!
complex_plane_t *create_complex_plane(int height, int width, double real_min, double real_max, double imaginary_min,
									  double imaginary_max);

// Converts a pixel coordinate to a complex number on the complex plane.
// This method will return the coordinate at the center (horizontally and vertically) of the given pixel.
complex_t coordinate_to_complex(complex_plane_t* plane, int x, int y, unsigned int ssaa_scale);

#endif //MANDELBROT_COMPLEX_H