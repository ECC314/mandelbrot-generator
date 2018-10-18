#include "include/image.h"

#define PIXEL_SIZE 3 // RGB -> 3B/px
#define BIT_DEPTH 8

int write_image_to_file(image_t *image, char *file_name)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;

	fp = fopen(file_name, "wb");
	if (fp == NULL)
	{
		fprintf(stderr, "Failed to open %s.\n", file_name);
		return 1;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fprintf(stderr, "Failed to create the PNG writer struct.\n");
		fclose(fp);
		return 1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fprintf(stderr, "Failed to create the PNG info struct.\n");
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return 1;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return 1;
	}

	png_set_IHDR(png_ptr,
				 info_ptr,
				 (size_t) image->width,
				 (size_t) image->height,
				 BIT_DEPTH,
				 PNG_COLOR_TYPE_RGB,
				 PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_DEFAULT,
				 PNG_FILTER_TYPE_DEFAULT
	);

	png_byte **rows = png_malloc(png_ptr, image->height * sizeof(png_byte *));
	for (size_t y = 0; y < image->height; y++)
	{
		png_byte *row = png_malloc(png_ptr, sizeof(uint8_t) * image->width * PIXEL_SIZE);
		rows[y] = row;
		for (size_t x = 0; x < image->width; x++)
		{
			pixel_t *pixel = image->data + (y * image->width) + x;
			row[0] = pixel->red;
			row[1] = pixel->green;
			row[2] = pixel->blue;
			row += 3;
		}
	}

	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, rows);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);


	for (int i = 0; i < image->height; i++)
	{
		png_free(png_ptr, rows[i]);
	}
	png_free(png_ptr, rows);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	return 0;
}

void free_image(image_t *image)
{
	if (image != NULL)
	{
		free(image->data);
		free(image);
	}
}
