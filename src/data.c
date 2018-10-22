#include "include/data.h"

int get_maximum(data_array_t *data)
{
	int max = 0;

	for (int i = 0; i < data->length; i++)
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

	for (int i = 0; i < data->length; i++)
	{
		if (data->values[i] < min || min == 0)
		{
			min = data->values[i];
		}
	}

	return min;
}

void free_data(data_array_t *data)
{
	if (data != NULL)
	{
		free(data->values);
		free(data);
	}
}