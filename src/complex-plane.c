

#include "include/complex-plane.h"


complex_plane_t *create_complex_plane(int height, int width, double real_min, double real_max,
                                      double imaginary_min, double imaginary_max)
{
	complex_plane_t *plane = malloc(sizeof(complex_plane_t));

	plane->pixel_height = (size_t) height;
	plane->pixel_width  = (size_t) width;
	plane->real_min = real_min;
	plane->real_max = real_max;
	plane->imaginary_max = imaginary_max;
	plane->imaginary_min = imaginary_min;

	return plane;
}

complex double coordinate_to_complex(complex_plane_t *plane, int x, int y, unsigned int ssaa_scale)
{
	complex double result = 0;
	result += plane->real_min + (x + 0.5) * (plane->real_max - plane->real_min) / (plane->pixel_width * ssaa_scale);
	result += I * (plane->imaginary_max + (y + 0.5) * (plane->imaginary_min - plane->imaginary_max) / (plane->pixel_height * ssaa_scale));
	
	return result;
}