#!/bin/bash

for i in `seq 0 99`; do
	echo "run $i"
	./net > data/out$i
done

python prob.py data/out*