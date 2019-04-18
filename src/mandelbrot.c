#include "include/debug.h"
#include "include/mandelbrot.h"


int mandelbrot_iteration_exceeds_limit(complex double c, double limit, int max_iterations)
{
	// z_0 := 0
	complex double z = 0;
	for (int n = 1; n <= max_iterations; n++)
	{
		// z_(n+1) := (z_n)^2 + c
		z = z * z + c;
		if (cabs(z) > limit)
		{
			return n;
		}
	}
	return 0;
}
