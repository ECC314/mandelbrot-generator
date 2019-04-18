#ifndef MANDELBROT_PARALLEL_H
#define MANDELBROT_PARALLEL_H

#include "configuration.h"
#include "data.h"

int* create_shared_data(size_t size);
void free_shared_data(int *addr, size_t size);

void get_multithreaded_data(data_array_t *data, config_t *config);

#endif //MANDELBROT_PARALLEL_H
