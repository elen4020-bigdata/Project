#!/bin/bash

mpic++ -o transpose.exe ./mpiTest.cpp
mpic++ -o format.exe ./mpiWrite.cpp
g++ -o generate.exe ./generator.cpp
