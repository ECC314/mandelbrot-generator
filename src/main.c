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
		palette = read_color_palette(config->palette_file, config->palette_type);
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

	int num_threads = 4;

	data_array_t *data = create_data_array(height * width);
	if (num_threads == 1)
	{
		get_mandelbrot_limit_data(data, config);
	}
	else
	{
		get_multithreaded_data(data, num_threads, config);
	}

	if (data == NULL)
	{
		free_palette(palette);
		free_config(config);
		return 1;
	}

	DEBUG_PRINT("Generating image data...\n");
	image_t *image = data_to_image(data, width, height, palette);
	free_palette(palette);
	free_data_array(data);

	DEBUG_PRINT("Writing PNG to %s...\n", config->output_file);
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