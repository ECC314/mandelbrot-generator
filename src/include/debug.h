#ifndef MANDELBROT_DEBUG_H
#define MANDELBROT_DEBUG_H

#ifdef MANDELBROT_DEBUG
	// Will printf() to stdout if debug mode is active and does nothing if it isn't.
	#define DEBUG_PRINT(x...) printf(x);

#else
	#define DEBUG_PRINT(x...)
#endif

#endif //MANDELBROT_DEBUG_H
