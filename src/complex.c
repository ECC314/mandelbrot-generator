#include "include/complex.h"

complex_t complex_add(complex_t z1, complex_t z2)
{
	complex_t result = {z1.real + z2.real, z1.imaginary + z2.imaginary};
	return result;
}

double complex_absolute(complex_t z)
{
	// abs(z) = sqrt(Re(z)^2 + Im(z)^2)
	return sqrt(z.real * z.real + z.imaginary * z.imaginary);
}

complex_t complex_square(complex_t z)
{
	// z^2 = (a + ib)^2 = a^2 + 2iab - b^2
	return (complex_t) {z.real * z.real - z.imaginary * z.imaginary, 2 * z.real * z.imaginary};
}

complex_t next_mandelbrot_iteration(complex_t z, complex_t c)
{
	// z_(n+1) := (z_n)^2 + c
	return complex_add(complex_square(z), c);
}

int mandelbrot_iteration_exceeds_limit(complex_t c, double limit, int max_iterations)
{
	complex_t z = {0, 0}; // z_0 := 0
	for (int n = 1; n <= max_iterations; n++)
	{
		z = next_mandelbrot_iteration(z, c);
		if (complex_absolute(z) > limit)
			return n;
	}
	return 0;
}

complex_plane_t *create_complex_plane(int height, int width, double real_min, double real_max, double imaginary_min,
									  double imaginary_max)
{
	complex_plane_t *plane = malloc(sizeof(complex_plane_t));

	plane->pixel_height = height;
	plane->pixel_width = width;
	plane->real_min = real_min;
	plane->real_max = real_max;
	plane->imaginary_max = imaginary_max;
	plane->imaginary_min = imaginary_min;

	return plane;
}

complex_t coordinate_to_complex(complex_plane_t *plane, int x, int y)
{
	complex_t result;
	result.real = plane->real_min + (x + 0.5) * (plane->real_max - plane->real_min) / plane->pixel_width;
	result.imaginary = plane->imaginary_max + (y + 0.5) * (plane->imaginary_min - plane->imaginary_max) / plane->pixel_height;
	
	return result;
}