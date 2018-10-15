#ifndef MANDELBROT_IMAGE_H
#define MANDELBROT_IMAGE_H

#include <math.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

typedef struct
{
	int *values;
	size_t length;
} data_array_t;

// Writes the `image` pixel values to `file_name`. Returns 0 if successful.
int write_image_to_file(image_t *image, char *file_name);

//pixel_t int_to_grayscale(data_array_t *data, int index);

// Converts a values array_t to an image_t using int_to_grayscale().
image_t *data_to_image(data_array_t *data, size_t width, size_t height);

void free_data(data_array_t *data);
void free_image(image_t *image);


#endif //MANDELBROT_IMAGE_H
