#ifndef DISSAS_H
#define DISSAS_H

size_t read_rom(uint8_t **buffer, char *rom_filename);

void dissas(uint8_t *rom, size_t len);

#endif // DISSAS_H
