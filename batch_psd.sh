#!/bin/bash

CLEANUP=true
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

PSD="$DIR/psd.py"
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
					rm mean_psd
				fi

				if [[ ! -a mean_psd ]]; then
					python "${PSD}" data/* > mean_psd
				fi

				popd
			fi
		done

		python "${PLOTGEN}" ${values[@]/%//mean_psd} psd.png

		popd
	fi
done
