#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

#include "include/debug.h"
#include "include/mandelbrot.h"
#include "include/processing.h"

typedef struct
{
	unsigned int thread_id;
	data_array_t *data;
	config_t *config;
} arguments_t;

// Calculates the iteration data for every [thread_id]-th line of the image.
void *render_lines(void *void_args)
{
	arguments_t *args = (arguments_t *) void_args;
	config_t *config = args->config;
	data_array_t *data = args->data;
	unsigned int offset = args->thread_id;
	unsigned int step_size = config->num_threads;

	size_t height = config->plane->pixel_height * config->ssaa_factor;
	size_t width = config->plane->pixel_width * config->ssaa_factor;

	for (unsigned int i = offset; i < height; i += step_size)
	{
		for (unsigned int r = 0; r < width; r++)
		{
			size_t index = i * width + r;
			complex double c = coordinate_to_complex(config->plane, r, i, config->ssaa_factor);
			data->values[index] = mandelbrot_iteration_exceeds_limit(c, ITERATION_ABS_LIMIT, config->iteration_depth);
		}
	}
	return NULL;
}


void calculate_iteration_data(data_array_t *data, config_t *config)
{
	unsigned int thread_count = config->num_threads;

	pthread_t threads[thread_count];
	arguments_t args[thread_count];
	for (unsigned int id = 0; id < thread_count; id++)
	{
		args[id].thread_id = id;
		args[id].config = config;
		args[id].data = data;

		if (pthread_create(&threads[id], NULL, render_lines, &args[id]))
		{
			LOG_ERROR("Failed to spawn worker thread #%d.\n", id);
			abort();
		}
		else
		{
			LOG_INFO("Spawned worker thread #%d.\n", id);
		}
	}
	LOG_INFO("Created %d worker thread%s.\n", thread_count, (thread_count != 1 ? "s" : ""));

	for (unsigned int id = 0; id < thread_count; id++)
	{
		if (pthread_join(threads[id], NULL))
		{
			perror("Error while joining thread");
		}
		else
		{
			LOG_INFO("Worker thread #%d finished.\n", id);
		}
	}
}

