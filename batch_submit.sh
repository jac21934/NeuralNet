#!/bin/bash

CLEANUP=false
CONDOR_FILE="/home/alnwarr/net/doNet.cmd"

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