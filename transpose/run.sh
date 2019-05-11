#!/bin/bash

rm input
rm inputMatrix.txt
rm output

echo "Generating matrix"
./generate.exe $1
echo "Formating matrix"
mpirun ./format.exe
echo "Running transpose on " $2 " ranks"
mpirun -np $2 ./transpose.exe input
echo "Programme complete"
