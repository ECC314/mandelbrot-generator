#ifndef MANDELBROT_DATA_UTIL_H
#define MANDELBROT_DATA_UTIL_H

#include <stdlib.h>

typedef struct
{
	int *values;
	size_t length;
} data_array_t;


// Returns the largest value contained within the data array.
int get_maximum(data_array_t *data);

void free_data(data_array_t *data);

#endif //MANDELBROT_DATA_UTIL_H
