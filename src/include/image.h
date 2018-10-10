#ifndef MANDELBROT_IMAGE_H
#define MANDELBROT_IMAGE_H

#include <png.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

typedef struct
{
	pixel_t *data;
	int width;
	int height;
} image_t;

// Writes the `image` pixel data to `file_name`. Returns 0 if successful.
int write_image_to_file(image_t *image, char *file_name);

// Converts a `value` to a grayscale pixel in relation to the `max` value.
// A lower `value` yields a lighter color; except for 0, which yields black (as 0 serves as a replacement for 'infinite').
pixel_t int_to_grayscale(int value, int max);

#endif //MANDELBROT_IMAGE_H
