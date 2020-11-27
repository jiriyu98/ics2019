#!/bin/bash

folder="/home/jiriyu/Desktop/ics2019_1/nexus-am/tests/cputest/tests"
files=$(ls $folder)
for file in $files
do
	file=${file%.*}
	echo $file
	make ARCH=riscv32-nemu ALL=$file run
	echo 'please input key to step into the next file.'
	read next
done

## load-store.c  leap-year.c
