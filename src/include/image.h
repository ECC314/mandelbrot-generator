#ifndef MANDELBROT_IMAGE_H
#define MANDELBROT_IMAGE_H

#include <assert.h>
#include <math.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

typedef struct
{
	pixel_t *data;
	size_t width;
	size_t height;
} image_t;

// Writes the `image` pixel values to `file_name`. Returns 0 if successful.
int write_image_to_file(image_t *image, char *file_name);

void free_image(image_t *image);


#endif //MANDELBROT_IMAGE_H
