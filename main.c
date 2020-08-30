#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "dissas.h"

int 
main()
{
    size_t  rom_len;
    uint8_t *rom;

    rom_len = read_rom(&rom, "./invaders/invaders.full");

    dissas(rom, rom_len);

    return 0;
}
