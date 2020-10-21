#!/bin/bash

# ps -ef|grep ./main|grep -v grep|awk '{print $2}'|xargs kill

gridSize=4
stepValue=1
filter=0.9995
readLength=10000

g++ -fopenmp -o main ptsa/*.cpp editor/*.cpp shouji/*.cpp main.cpp
chmod u+x main

for threadCount in 1 4 8 16;
do
	inputFilename=`pwd`/dataset/${readLength}.txt
	outputFilename=`pwd`/result/result_${threadCount}.txt
	echo $inputFilename
	echo $outputFilename

	for e in {25..300..25};
  do
    e=$(printf "%.3f" `echo "scale=3;$e/1000.0"|bc`)
    echo "./main $gridSize $stepValue $e $filter $threadCount $inputFilename >> $outputFilename"
    ./main $gridSize $stepValue $e $filter $threadCount $inputFilename >> $outputFilename
    echo -e '\n\n\n' >> $outputFilename
  done
done

