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

void free_data(data_array_t *data)
{
	if (data != NULL)
	{
		free(data->values);
		free(data);
	}
}