#include "include/antialiasing.h"
#include "include/debug.h"

data_array_t *get_ssaa_data(data_array_t *raw_data, int factor, config_t *config)
{
	data_array_t *result_data = create_data_array(raw_data->length / (factor * factor));
	if (result_data == NULL)
	{
		return NULL;
	}

	size_t width = config->plane->pixel_width;
	size_t height = config->plane->pixel_height;

	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			double sum = 0;
			for (int y_aa = 0; y_aa < factor; y_aa++)
			{
				for (int x_aa = 0; x_aa < factor; x_aa++)
				{
					size_t y_index = y * factor + y_aa;
					size_t x_index = x * factor + x_aa;

					sum += raw_data->values[y_index * width * factor + x_index];
				}
			}

			result_data->values[y * width + x] = (int) (sum / (factor * factor));
		}
	}

	return result_data;
}
