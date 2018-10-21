#!/bin/bash

if [[ $# -lt 3 ]]; then
	echo "This script will create a MP4 video of the iteration steps."
	echo
	echo "Usage:"
	echo "    $(basename $0) output_folder image_count normal_generator_parameters"
	echo
	echo "One would normally include `-f output_file` with the generator parameters. However, this script uses `output_folder` instead, which will create a new folder (if it doesn't exist) and save both the PNG images and the resulting MP4 file."

	exit
fi

out="$1"
count="$2"
args=""
for i in $(seq 3 $#); do
	args="$args ${!i}"
done
mkdir -p "$out"

echo "Output folder: $out"
echo "Output folder: $out"
echo "Mandelbrot generator parameters:"
echo "    $args"

for i in $(seq -f "%05g" 1 $count); do
	echo "Generating image $i..."
	./mandelbrot-generator $args -f "$out/mandelbrot-$i.png" -d $i > /dev/null
done

ffmpeg -i "$out/mandelbrot-%05d.png" -c:v libx264 -r 24 "$out/out.mp4"
