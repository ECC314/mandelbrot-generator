#include "include/palette.h"

void free_palette(palette_t *palette)
{
	if (palette != NULL)
	{
		free(palette->colors);
		free(palette);
	}
}

palette_t *read_color_palette(char *filename, int type)
{
	palette_t *palette = calloc(1, sizeof(palette_t));
	palette->type = type;

	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Failed to open %s.\n", filename);
		free_palette(palette);
		return NULL;
	}


	int r, g, b;
	int line = 1;
	int read_count;

	while ((read_count = fscanf(file, "RGB(%d,%d,%d)\n", &r, &g, &b)) != EOF)
	{
		if (read_count != 3 || r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255)
		{
			fprintf(stderr, "Invalid color in line %d.\n", line);
			fclose(file);
			free_palette(palette);
			return NULL;
		}

		palette->size++;
		palette->colors = realloc(palette->colors, palette->size * sizeof(pixel_t));
		palette->colors[palette->size - 1] = (pixel_t) {(uint8_t) r, (uint8_t) g, (uint8_t) b};
		line++;
	}

	fclose(file);
	DEBUG_PRINT("Loaded %d colors from %s.\n", palette->size, filename);

	return palette;
}

pixel_t get_linear_grayscale(int value, int max)
{
	uint8_t color;

	if (value == 0)
	{
		color = 0;
	}
	else
	{
		double scale = (max - value) / (double) max;
		color = (uint8_t) ceil(scale * 255);
	}

	return (pixel_t) {color, color, color};
}

pixel_t get_custom_rgb(int value, int max, int min, palette_t *palette)
{
	int max_index = palette->size - 1;

	if (value == 0 || min == max)
	{
		return palette->colors[max_index];
	}
	else
	{
		double scale;
		if (palette->type == PALETTE_ABSOLUTE)
			scale = value / (double) max;
		else
			scale = (value - min) / (double) (max - min);

		int index = (int) ceil(scale * max_index);
		return palette->colors[index];
	}
}

image_t *data_to_image(data_array_t *data, size_t width, size_t height, palette_t *palette)
{
	assert(data->length == width * height);

	image_t *image = calloc(1, sizeof(image_t));
	image->data = calloc((size_t) data->length, sizeof(pixel_t));
	image->width = width;
	image->height = height;

	int max = get_maximum(data);
	int min = get_minimum(data);
	for (int i = 0; i < data->length; i++)
	{
		if (palette == NULL)
		{
			image->data[i] = get_linear_grayscale(data->values[i], max);
		}
		else
		{
			image->data[i] = get_custom_rgb(data->values[i], max, min, palette);
		}
	}

	return image;
}