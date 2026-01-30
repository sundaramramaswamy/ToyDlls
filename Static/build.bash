#!/usr/bin/bash

clang++ -c -Iinc src/add.cpp src/sub.cpp

ar rcs StaticMath.a add.o sub.o

clang++ -Iinc -c example/main.cpp

clang++ -o example.exe main.o StaticMath.a
