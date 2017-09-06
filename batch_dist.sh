#!/bin/bash

CLEANUP=true

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

DISTGEN="$DIR/prob.py"
PLOTGEN="$DIR/plot.py"

for var in *; do
	if [ -d "${var}" ]; then
		unset values
		pushd -- "${var}"
		for dir in *; do
			if [ -d "${dir}" ]; then
				values+=(`basename "${dir}"`)

				pushd -- "${dir}"

				if $CLEANUP; then
					rm wait_time_dist duration_dist depol_dist active_dist
				fi

				if [[ ! -a wait_time_dist ]]; then
					python "${DISTGEN}" 2 data/* > wait_time_dist
				fi

				if [[ ! -a duration_dist ]]; then			
					python "${DISTGEN}" 3 data/* > duration_dist
				fi
			
				if [[ ! -a depol_dist ]]; then
					python "${DISTGEN}" 4 data/* > depol_dist
				fi

				if [[ ! -a active_dist ]]; then
					python "${DISTGEN}" 5 data/* > active_dist
				fi

				popd
			fi
		done

		python "${PLOTGEN}" ${values[@]/%//wait_time_dist} wait_time.png
		python "${PLOTGEN}" ${values[@]/%//duration_dist} duration.png
		python "${PLOTGEN}" ${values[@]/%//depol_dist} depol.png
		python "${PLOTGEN}" ${values[@]/%//active_dist} active.png

		popd
	fi
done
