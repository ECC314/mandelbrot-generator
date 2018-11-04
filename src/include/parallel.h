#ifndef MANDELBROT_PARALLEL_H
#define MANDELBROT_PARALLEL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>


int* create_shared_data(size_t size);
void free_shared_data(int *addr, size_t size);


#endif //MANDELBROT_PARALLEL_H
