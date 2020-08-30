CC=gcc
CFLAGS="-g -Warn -werror"

dissas: dissas.c
	gcc -g -Wall -Werror dissas.c -o dis
