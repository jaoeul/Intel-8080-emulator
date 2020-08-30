CC=gcc

dissas: dissas.c
	gcc -g -Wall -Werror dissas.c main.c -o dis
