#!/bin/sh

gcc -I../src -lmodbus example.c -o example
gcc -I../src -lmodbus stress.c -o stress
gcc -I../src -lmodbus stream.c -o stream
