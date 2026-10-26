#!/usr/bin/bash

for i in {01..10}
do
	make ARGS="data/pub$i.in" >res/pub$i.out
done

for i in {01..10}
do
	echo $i
	#make run ARGS="data/pub$i.in" >res/pub$i.out
	time make run ARGS="data/pub$i.in"
	
	#cat data/pub$i.out
	#cat res/pub$i.out
	#diff data/pub$i.out res/pub$i.out
	echo "====="
	echo ""
done

