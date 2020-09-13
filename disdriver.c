#include <stdint.h>
#include <stdlib.h>

#include "dissas.h"

int
main(int argc, char **argv)
{
    uint8_t *rom;
    size_t rom_sz = read_rom(&rom, argv[1]);
    dissas_rom(rom, rom_sz);
    free(rom);
    return 0;
}

