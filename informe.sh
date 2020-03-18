#!/bin/bash
for i in {1..9};
do
	gtimeout 10s ./solver tests/level_0$i.txt output.txt
	exit_status=$?
	if [[ $exit_status -eq 124 ]]; then
		echo "TIMEOUT"
	fi
done
for i in {10..30};
do
	gtimeout 10s ./solver tests/level_$i.txt output.txt
	exit_status=$?
	if [[ $exit_status -eq 124 ]]; then
		echo "TIMEOUT"
fi
done