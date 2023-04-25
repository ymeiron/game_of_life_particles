#!/bin/bash
set -e
#make

seed=0 # 26 good for rho=2.25
fps=30
output_name=sigma.mp4

###python generate_ic.py --seed=$seed --density=2.000 --sigma=.40 --output="input/ic_$seed.dat"

python generate_ic.py --seed=$seed --density=2.00 --sigma=.40 --output="input.dat"
rm -f output/output.h5part
./build/simulation --extent=400 --tmax=3000 --input="input.dat" --output=output/output.h5part
rm -f images/*.png
python plot_from_hdf5.py
ffmpeg -y -r $fps -i "images/%04d.png" -pix_fmt yuv420p "output/$output_name"