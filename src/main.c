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

	DEBUG_PRINT("Generating iteration data...\n");
	data_array_t *data = get_mandelbrot_limit_data(config);
	DEBUG_PRINT("Generating image data...\n");
	image_t *image = data_to_image(data, width, height);
	DEBUG_PRINT("Writing PNG...\n");
	if (write_image_to_file(image, config->file_name))
	{
		fprintf(stderr, "An error occurred while attempting to write the PNG values. Aborting.\n");
		free_config(config);
		return 1;
	}

	free_image(image);
	free_data(data);
	free_config(config);
	return 0;


}