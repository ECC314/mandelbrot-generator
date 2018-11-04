#include "include/parallel.h"

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