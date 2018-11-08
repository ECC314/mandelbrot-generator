#ifndef MANDELBROT_PARALLEL_H
#define MANDELBROT_PARALLEL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "data.h"
#include "configuration.h"

int* create_shared_data(size_t size);
void free_shared_data(int *addr, size_t size);

void get_multithreaded_data(data_array_t *data, int count, config_t *config);

#endif //MANDELBROT_PARALLEL_H
