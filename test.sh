#! /bin/bash

if [[ $# -lt 2 ]]; then
	echo "usage: $0 <relative_path_to_executable> <relative_path_to_tests_directory> <optional>"
	echo "provide 'clean' as optional flag to remove generated files"
	exit
fi

bin="${1}"
test_dir="${2}"

for f in $test_dir/*.in; do
	if [[ -f $f ]]; then
		($PWD/$bin) < $f > ${f%.in}.myout
	fi
done

for f in $test_dir/*.myout; do
	diff -c $f ${f%.myout}.out > ${f%.myout}.diff 
done

for f in $test_dir/*.diff; do
	echo "${f:(-11):(-5)} result"
	cat $f
	echo $'\n'
done

if [[ "${3}" == "clean" ]]; then
	echo "removed .diff and .myout files"
	rm $test_dir/*.myout $test_dir/*.diff
fi
