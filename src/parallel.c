#include <sys/wait.h>
#include "include/parallel.h"
#include "include/mandelbrot.h"

int *create_shared_data(size_t size)
{
	int access = PROT_READ | PROT_WRITE;
	int type = MAP_SHARED | MAP_ANONYMOUS;

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

// Calculates the iteration data for every [thread_count]-th line of the image.
void render_lines(int offset, int thread_count, data_array_t *data, config_t *config)
{
	size_t height = config->plane->pixel_height;
	size_t width = config->plane->pixel_width;

	for (int i = offset; i < height; i += thread_count)
	{
		for (int r = 0; r < width; r++)
		{
			size_t index = i * width + r;
			complex_t c = coordinate_to_complex(config->plane, r, i);
			data->values[index] = mandelbrot_iteration_exceeds_limit(c, config->limit, config->iteration_depth);
		}
	}
}

void get_multithreaded_data(data_array_t *data, int count, config_t *config)
{
	pid_t pid;

	for (int i = 0; i < count; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			fprintf(stderr, "Failed to fork() worker thread #%d.\n", i);
			abort();
		}
		else if (pid == 0)
		{
			render_lines(i, count, data, config);
			exit(0);
		}
		else
		{
			DEBUG_PRINT("Started worker thread #%d (PID %d).\n", i, pid);
		}

	}

	printf("Created %d worker threads.\n", count);

	while (count > 0)
	{
		int status;
		pid = wait(&status);

		if (status == 0)
		{
			DEBUG_PRINT("Worker thread with PID %ld finished iterating.\n", (long) pid);
		}
		else
		{
			fprintf(stderr, "Worker thread with PID %ld finished with non-zero exit code %d.\n", (long) pid, status);
		}

		count--;
	}
}

