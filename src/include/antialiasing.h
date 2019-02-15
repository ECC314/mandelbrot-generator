#ifndef MANDELBROT_ANTIALIASING_H
#define MANDELBROT_ANTIALIASING_H

#include "data.h"
#include "configuration.h"

/* Creates a new data array by applying supersample antialiasing to the raw data.
 * This method will merge factor*factor raw data points into a single data point.
 * The resulting data array will be allocated automatically.
 */
data_array_t *get_ssaa_data(data_array_t *raw_data, int factor, config_t *config);

#endif //MANDELBROT_ANTIALIASING_H
