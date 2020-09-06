emu:
	gcc -g -Wall -Werror dissas.c emu.c -o emu

dis:
	gcc -g -Wall -Werror dissas.c disdriver.c -o dis
