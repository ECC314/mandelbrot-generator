#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

#include "include/debug.h"
#include "include/mandelbrot.h"
#include "include/parallel.h"

int *create_shared_data(size_t size)
{
	unsigned int access = (unsigned int)PROT_READ | (unsigned int) PROT_WRITE;
	unsigned int type = (unsigned int) MAP_SHARED | (unsigned int) MAP_ANONYMOUS;

	void *addr = mmap(NULL, size, access, type, 0, 0);

	if (addr == MAP_FAILED)
	{
		perror("Failed to map shared memory");
		return NULL;
	}

	return (int *) addr;
}

void free_shared_data(int *addr, size_t size)
{
	if (munmap(addr, size) == -1)
	{
		perror("Failed to free shared memory");
	}
}

typedef struct {
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


void get_multithreaded_data(data_array_t *data, config_t *config)
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
			fprintf(stderr, "Failed to spawn worker thread #%d.\n", id);
			abort();
		}
		else
		{
			DEBUG_PRINT("Spawned worker thread #%d.\n", id);
		}
	}
	printf("Created %d worker thread%s.\n", thread_count, (thread_count != 1 ? "s" : ""));

	for (unsigned int id = 0; id < thread_count; id++)
	{
		if(pthread_join(threads[id], NULL))
		{
			perror("Error while joining thread");
		}
		else
		{
			DEBUG_PRINT("Worker thread #%d finished.\n", id);
		}
	}
}

