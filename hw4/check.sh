#!/usr/bin/bash

mkdir -p res

# for i in {01..10}
# do
	# make ARGS="pub$i"
# done

for i in {01..10}
do
	echo $i
	# make run ARGS="pub$i" >res/pub$i.out
	# ./rescue <data/pub$i.in >res/pub$i.out
	time make run ARGS="pub$i"
	
	# cat data/pub$i.out
	# cat res/pub$i.out
	# diff data/pub$i.out res/pub$i.out
	echo "====="
	echo ""
done

