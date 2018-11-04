#include "include/mandelbrot.h"

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

data_array_t *get_mandelbrot_limit_data(config_t *config)
{
	const size_t height = config->plane->pixel_height;
	const size_t width = config->plane->pixel_width;

	data_array_t *data = calloc(1, sizeof(data_array_t));
	data->length = width * height;
	data->values = create_shared_data(data->length * sizeof(int));

	if (data->values == NULL)
	{
		free(data);
		return NULL;
	}

	for (int i = 0; i < height; i++)
	{
		if (i % 100 == 0)
			DEBUG_PRINT("Calculating line %d...\n", i);
		for (int r = 0; r < width; r++)
		{
			size_t index = i * width + r;
			complex_t c = coordinate_to_complex(config->plane, r, i);
			data->values[index] = mandelbrot_iteration_exceeds_limit(c, config->limit, config->iteration_depth);
		}
	}
	return data;
}