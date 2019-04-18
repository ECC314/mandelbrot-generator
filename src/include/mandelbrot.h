#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include <complex.h>

#include "configuration.h"
#include "data.h"

#define ITERATION_ABS_LIMIT 1000 // Iteration ends once abs(z) exceeds 10^3.

// Returns the index n of the first Mandelbrot iteration of c where abs(z_n) is greater than `limit`.
// Returns 0 if the series does not exceed `limit` within `max_iterations` steps.
int mandelbrot_iteration_exceeds_limit(complex double c, double limit, int max_iterations);

#endif //MANDELBROT_MANDELBROT_H
