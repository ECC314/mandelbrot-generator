#include <stdio.h>
#include <stdlib.h>

#include "include/complex.h"
#include "include/image.h"

int main(int argc, char** argv)
{
	int width = 300;
	int height = 300;
	size_t pixel_count = (size_t) width * height;

	complex_plane_t *plane = create_complex_plane(width, height, -3.5, 2.5, 3, -3);

	int *mandelbrot_data = calloc(pixel_count, sizeof(int));

	image_t image = {NULL, width, height};
	image.data = calloc(pixel_count, sizeof(pixel_t));

	int *iteration_data = calloc(pixel_count, sizeof(int));
	// Highest iteration count, will be used to scale colors.
	int maximum = 0;

	for (int i = 0; i < height; i++)
	{
		for (int r = 0; r < width; r++)
		{
			int index = i * width + r;
			complex_t c = coordinate_to_complex(plane, r, i);
			int iteration = mandelbrot_iteration_exceeds_limit(c, 1000, 200);

			iteration_data[index] = iteration;
			if (iteration > maximum)
				maximum = iteration;
		}
	}

	for (int i = 0; i < pixel_count; i++)
	{
		image.data[i] = int_to_grayscale(iteration_data[i], maximum);
	}

	if (write_image_to_file(&image, argv[1]))
	{
		fprintf(stderr, "An error occurred while attempting to write the PNG data. Aborting.\n");
	}

	free(iteration_data);
	free(image.data);
	free(mandelbrot_data);
	free(plane);
	return 0;


}