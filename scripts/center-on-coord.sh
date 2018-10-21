#!/bin/bash


if [[ $# != 7 ]] && [[ $# != 8 ]]; then
	echo "This script will generate a (square) image of the Mandelbrot set, centered on the given coordinate."
	echo
	echo "Usage:"
	echo "    $(basename $0) real imag border size depth output_file [palette_file]"
	echo
	echo "        real          Real value of the target coordinate."
	echo "        imag          Imaginary value of the target coordinate."
	echo "        border        Distance from the center to the edge of the image."
	echo "        size          Height and width of the image. This script only generates square images."
	echo "        limit         The limit that has to be exceeded by abs(z_n)."
	echo "        depth         Maximum iteration depth."
	echo "        output_file   Location of the output PNG."
	echo "        palette_file  Location of the color palette file. (Optional)"

	exit
fi

REAL="$1"
IMAG="$2"
BORDER="$3"
SIZE="$4"
LIMIT="$5"
DEPTH="$6"
FILENAME="$7"
PALETTE="$8"

r1=$(echo "${REAL}-$BORDER" | bc)
r2=$(echo "${REAL}+$BORDER" | bc)
i1=$(echo "${IMAG}-$BORDER" | bc)
i2=$(echo "${IMAG}+$BORDER" | bc)

if [[ "$PALETTE" != "" ]]; then
	../cmake-build-debug/mandelbrot-generator -f "$FILENAME" -i "${SIZE}x${SIZE}" -l "$LIMIT" \
		-p "${r1}/${r2}/${i1}/${i2}" -d "$DEPTH" -P "$PALETTE"
else
	../cmake-build-debug/mandelbrot-generator -f "$FILENAME" -i "${SIZE}x${SIZE}" -l "$LIMIT" \
		-p "${r1}/${r2}/${i1}/${i2}" -d "$DEPTH"
fi
