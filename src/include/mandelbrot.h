#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include "complex.h"
#include "configuration.h"
#include "debug.h"
#include "image.h"

// Obtain Mandelbrot iteration z_(n+1) from z_n and c.
complex_t next_mandelbrot_iteration(complex_t z, complex_t c);

// Returns the index n of the first Mandelbrot iteration of c where abs(z_n) is greater than `limit`.
// Returns 0 if the series does not exceed `limit` within `max_iterations` steps.
int mandelbrot_iteration_exceeds_limit(complex_t c, double limit, int max_iterations);

// Generates a data array of the first indices where z_n(c) exceeds the limit.
data_array_t *get_mandelbrot_limit_data(config_t *config);


#endif //MANDELBROT_MANDELBROT_H
