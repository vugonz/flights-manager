#! /bin/bash

if [[ ! $# -eq 2 ]]; then
	echo "usage $0 <path_to_executable> <path_to_tests_directory>"
	exit
fi

bin="${1}"			# executable
test_dir="${2}"		# tests folder

if [[ $# -eq 2 ]]; then
	bin="${1}"			# executable

	for t in $test_dir/*.in; do
		if [[ -f $t ]]; then
			./$bin < $t > ${t%.in}.myout
		fi
	done

	for t in $test_dir/*.myout; do
		diff -c $t ${t%.myout}.out > ${t%.myout}.diff 
	done

	for t in $test_dir/*.diff; do
		echo "${t:(-11):(-5)} result"
		cat $t
		echo $'\n'
	done

	rm $test_dir/*.myout $test_dir/*.diff
fi
