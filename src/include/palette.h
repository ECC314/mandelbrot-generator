#ifndef MANDELBROT_PALETTE_H
#define MANDELBROT_PALETTE_H

#include <assert.h>

#include "complex.h"
#include "configuration.h"
#include "data.h"
#include "image.h"
#include "palette.h"

typedef struct
{
	int size;
	pixel_t *colors;
} palette_t;

void free_palette(palette_t *palette);

// Attempts to generate a color palette from a file.
palette_t *read_color_palette(char *filename);

// Converts a data_array_t to an image_t using the provided color palette.
image_t *data_to_image(data_array_t *data, size_t width, size_t height, palette_t *palette);

#endif //MANDELBROT_PALETTE_H