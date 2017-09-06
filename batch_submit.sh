#!/bin/bash

CLEANUP=true
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CONDOR_FILE="$DIR/doNet.cmd"

for var in *; do
	if [ -d "${var}" ]; then
		pushd -- "${var}"
		for dir in *; do
			if [ -d "${dir}" ]; then
				pushd "${dir}"

				if $CLEANUP; then
					rm -rf data errors logs
				fi

				mkdir data errors logs

				condor_submit "${CONDOR_FILE}"

				popd
			fi
		done
		popd
	fi
done
