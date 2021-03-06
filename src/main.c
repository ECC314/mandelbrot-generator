#include <stdio.h>

#include "include/configuration.h"
#include "include/debug.h"
#include "include/palette.h"
#include "include/processing.h"
#include "include/antialiasing.h"

int main(int argc, char **argv)
{
	config_t *config = calloc(1, sizeof(config_t));
	if (parse_args(argc, argv, config) != 0)
	{
		LOG_ERROR("At least one parameter is invalid, or a required parameter is missing. Aborting.\n");
		print_usage();
		free_config(config);
		return 1;
	}

	const size_t height = config->plane->pixel_height;
	const size_t width = config->plane->pixel_width;

	palette_t *palette = NULL;
	if (config->palette_file != NULL)
	{
		LOG_INFO("Defining color palette...\n");
		palette = read_color_palette(config->palette_file, config->palette_type);
		if (palette == NULL)
		{
			LOG_ERROR("Failed to read color palette from %s. Aborting.\n", config->palette_file);
			free_config(config);
			return 1;
		}
	}
	else
	{
		LOG_INFO("Using default color palette: Linear grayscale.\n");
	}

	LOG_INFO("Generating iteration data...\n");
	data_array_t *data = create_data_array(height * config->ssaa_factor * width * config->ssaa_factor);
	if (data == NULL)
	{
		free_palette(palette);
		free_config(config);
		return 1;
	}

	calculate_iteration_data(data, config);


	if (config->ssaa_factor > 1)
	{
		LOG_INFO("Beginning supersampling antialiasing...\n");
		data_array_t *processed_data = get_ssaa_data(data, config->ssaa_factor, config);
		free(data);
		if (processed_data == NULL)
		{
			free_palette(palette);
			free_config(config);
			return 1;
		}
		data = processed_data;
	}

	LOG_INFO("Generating image data...\n");
	image_t *image = data_to_image(data, width, height, palette);
	free_palette(palette);
	free_data_array(data);

	LOG_INFO("Writing PNG to %s...\n", config->output_file);
	if (write_image_to_file(image, config->output_file))
	{
		LOG_ERROR("An error occurred while attempting to write the PNG values. Aborting.\n");
		free_image(image);
		free_config(config);
		return 1;
	}

	free_image(image);
	free_config(config);
	return 0;
}