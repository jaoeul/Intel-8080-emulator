CC=gcc

emu:
	gcc -g -Wall -Werror dissas.c emu.c -o emu
