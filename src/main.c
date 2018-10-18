#include "include/main.h"

int main(int argc, char** argv)
{
	DEBUG_PRINT("Verbose mode is active.\n");

	config_t *config = parse_args(argc, argv);

	if (config == NULL)
	{
		fprintf(stderr, "At least one parameter is missing or invalid. Aborting.\n");
		return 1;
	}

	const size_t height = config->plane->pixel_height;
	const size_t width = config->plane->pixel_width;

	palette_t *palette = NULL;
	if (config->palette_file != NULL)
	{
		DEBUG_PRINT("Defining color palette...\n");
		palette = read_color_palette(config->palette_file);
		if (palette == NULL)
		{
			fprintf(stderr, "Failed to read color palette from %s. Aborting.\n", config->palette_file);
			free_config(config);
			return 1;
		}
	}
	else
	{
		DEBUG_PRINT("Using default color palette: Linear grayscale.\n");
	}

	DEBUG_PRINT("Generating iteration data...\n");
	data_array_t *data = get_mandelbrot_limit_data(config);

	DEBUG_PRINT("Generating image data...\n");
	image_t *image = data_to_image(data, width, height, palette);
	free_palette(palette);
	free_data(data);

	DEBUG_PRINT("Writing PNG...\n");
	if (write_image_to_file(image, config->output_file))
	{
		fprintf(stderr, "An error occurred while attempting to write the PNG values. Aborting.\n");
		free_image(image);
		free_config(config);
		return 1;
	}

	free_image(image);
	free_config(config);
	return 0;
}