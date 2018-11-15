#include "include/data.h"
#include "include/parallel.h"

int get_maximum(data_array_t *data)
{
	int max = 0;

	for (size_t i = 0; i < data->length; i++)
	{
		if (data->values[i] > max)
		{
			max = data->values[i];
		}
	}

	return max;
}

int get_minimum(data_array_t *data)
{
	int min = 0;

	for (size_t i = 0; i < data->length; i++)
	{
		if (data->values[i] != 0 && (data->values[i] < min || min == 0))
		{
			min = data->values[i];
		}
	}
	return min;
}

data_array_t *create_data_array(size_t length)
{
	data_array_t *data = calloc(1, sizeof(data_array_t));
	data->length = length;
	data->values = create_shared_data(length * sizeof(int));

	return data;
}

void free_data_array(data_array_t *data)
{
	if (data != NULL)
	{
		free_shared_data(data->values, data->length);
		free(data);
	}
}