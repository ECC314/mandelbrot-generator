#include <stdio.h>
#include <stdlib.h>

#include "include/complex.h"
#include "include/image.h"
#include "include/configuration.h"

int main(int argc, char** argv)
{
	config_t *config = parse_args(argc, argv);

	if (config == NULL)
	{
		fprintf(stderr, "At least one parameter is missing or invalid. Aborting.\n");
		return 1;
	}

	const int height = config->plane->pixel_height;
	const int width = config->plane->pixel_width;

	printf("%d/%d\n", height, width);

	int *mandelbrot_data = calloc((size_t) width * height, sizeof(int));

	image_t image = {NULL, width, height};
	image.data = calloc((size_t) width * height, sizeof(pixel_t));

	int *iteration_data = calloc((size_t) width * height, sizeof(int));
	// Highest iteration count, will be used to scale colors.
	int maximum = 0;

	for (int i = 0; i < height; i++)
	{
		if (i % 100 == 0)
			printf("Calculating line %d...\n", i);
		for (int r = 0; r < width; r++)
		{
			int index = i * width + r;
			complex_t c = coordinate_to_complex(config->plane, r, i);
			int iteration = mandelbrot_iteration_exceeds_limit(c, config->limit, config->iteration_depth);

			iteration_data[index] = iteration;
			if (iteration > maximum)
				maximum = iteration;
		}
	}

	for (int i = 0; i < width * height; i++)
	{
		image.data[i] = int_to_grayscale(iteration_data[i], maximum);
	}

	if (write_image_to_file(&image, config->file_name))
	{
		fprintf(stderr, "An error occurred while attempting to write the PNG data. Aborting.\n");
		free_config(config);
		return 1;
	}

	free(iteration_data);
	free(image.data);
	free(mandelbrot_data);
	free_config(config);
	return 0;


}