#!/bin/bash

TIMEOUT=30m
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

trap 'kill -INT -$pid; rm data/out$i; echo; exit' INT

for i in `seq 0 99`; do
	echo "run $i"

	timeout $TIMEOUT "$DIR/net" > data/out$i &
	pid=$!
	wait $pid

	if [ $? != 0 ]; then
		echo "Timed out"
		sleep 1
		rm data/out$i
	fi
done

echo "Outputting final probability distribution"
python "$DIR/prob.py" data/out* > final