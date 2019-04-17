#include "include/debug.h"
#include "include/mandelbrot.h"


int mandelbrot_iteration_exceeds_limit(complex double c, double limit, int max_iterations)
{
	// z_0 := 0
	complex double z = 0;
	for (int n = 1; n <= max_iterations; n++)
	{
		// z_(n+1) := (z_n)^2 + c
		z = z * z + c;
		if (cabs(z) > limit)
		{
			return n;
		}
	}
	return 0;
}

void get_mandelbrot_limit_data(data_array_t *data, config_t *config)
{
	const unsigned int height = (unsigned int) config->plane->pixel_height * config->ssaa_factor;
	const unsigned int width  = (unsigned int) config->plane->pixel_width * config->ssaa_factor;

	for (unsigned int i = 0; i < height; i++)
	{
		if (i % 100 == 0)
			DEBUG_PRINT("Calculating line %d...\n", i);
		for (unsigned int r = 0; r < width; r++)
		{
			size_t index = i * width + r;
			complex double c = coordinate_to_complex(config->plane, r, i, config->ssaa_factor);
			data->values[index] = mandelbrot_iteration_exceeds_limit(c, ITERATION_ABS_LIMIT, config->iteration_depth);
		}
	}
}