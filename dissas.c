#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dissas.h"
#include "intel8080.h"

size_t
read_rom(uint8_t **buffer, char *rom_filename)
{
    FILE *fileptr;
    size_t filelen;

    fileptr = fopen(rom_filename, "rb");
    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);

    *buffer = (uint8_t*) calloc(sizeof(uint8_t), filelen);

    if (*buffer == NULL) {
        printf("*buffer is NULL\n");
    }

    fread(*buffer, filelen, 1, fileptr);

    fclose(fileptr);

    return filelen;
}

void
dissas_curr_inst(State *state)
{
    uint8_t *opcode = &state->memory[state->pc];

    switch (*opcode) {

        case 0x00:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("NOP");
            printf("\n");
            break;

        case 0x01:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("LXI B #0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x02:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("STAX B");
            printf("\n");
            break;

        case 0x03:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("STAX B");
            printf("\n");
            break;

        case 0x04:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR B");
            printf("\n");
            break;

        case 0x05:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR B");
            printf("\n");
            break;

        case 0x06:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI B #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x07:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RLC");
            printf("\n");
            break;

        case 0x08: // Not specified
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0x09:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DAD B");
            printf("\n");
            break;

        case 0x0a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("LDAX B");
            printf("\n");
            break;

        case 0x0b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCX B");
            printf("\n");
            break;

        case 0x0c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR C");
            printf("\n");
            break;

        case 0x0d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR C");
            printf("\n");
            break;

        case 0x0e:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI C #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x0f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RRC");
            printf("\n");
            break;

        case 0x10: // Not specified
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0x11:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("LXI D #0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x12:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("STAX D");
            printf("\n");
            break;

        case 0x13:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INX D");
            printf("\n");
            break;

        case 0x14:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR D");
            printf("\n");
            break;

        case 0x15:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR D");
            printf("\n");
            break;

        case 0x16:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI D, #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x17:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RAL");
            printf("\n");
            break;

        case 0x18: // Not specified
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0x19:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DAD D");
            printf("\n");
            break;

        case 0x1a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("LDAX D");
            printf("\n");
            break;

        case 0x1b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCX D");
            printf("\n");
            break;

        case 0x1c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR E");
            printf("\n");
            break;

        case 0x1d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR E");
            printf("\n");
            break;

        case 0x1e:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI E #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x1f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RAR");
            printf("\n");
            break;

        case 0x20:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RIM");
            printf("\n");
            break;

        case 0x21:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("LXI H #0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x22:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("SHLD 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x23:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INX H");
            printf("\n");
            break;

        case 0x24:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR H");
            printf("\n");
            break;

        case 0x25:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR H");
            printf("\n");
            break;

        case 0x26:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI H, #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x27:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("DAA");
            printf("\n");
            break;

        case 0x28: // Not specified
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0x29:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DAD H");
            printf("\n");
            break;

        case 0x2a:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("LHLD 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x2b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCX H");
            printf("\n");
            break;

        case 0x2c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR L");
            printf("\n");
            break;

        case 0x2d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR L");
            printf("\n");
            break;

        case 0x2e:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI L #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x2f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMA");
            printf("\n");
            break;

        case 0x30:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SIM");
            printf("\n");
            break;

        case 0x31:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("LXI SP #0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x32:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("STA 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x33:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INX SP");
            printf("\n");
            break;

        case 0x34:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR M");
            printf("\n");
            break;

        case 0x35:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR M");
            printf("\n");
            break;

        case 0x36:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI M, #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x37:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("STC");
            printf("\n");
            break;

        case 0x38:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0x39:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DAD SP");
            printf("\n");
            break;

        case 0x3a:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("LDA 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0x3b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCX SP");
            printf("\n");
            break;

        case 0x3c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("INR A");
            printf("\n");
            break;

        case 0x3d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DCR A");
            printf("\n");
            break;

        case 0x3e:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("MVI A ##0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0x3f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMC");
            printf("\n");
            break;

        case 0x40:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, B");
            printf("\n");
            break;

        case 0x41:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, C");
            printf("\n");
            break;

        case 0x42:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, D");
            printf("\n");
            break;

        case 0x43:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, E");
            printf("\n");
            break;

        case 0x44:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, H");
            printf("\n");
            break;

        case 0x45:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, L");
            printf("\n");
            break;

        case 0x46:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, M");
            printf("\n");
            break;

        case 0x47:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV B, A");
            printf("\n");
            break;

        case 0x48:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, B");
            printf("\n");
            break;

        case 0x49:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, C");
            printf("\n");
            break;

        case 0x4a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, D");
            printf("\n");
            break;

        case 0x4b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, E");
            printf("\n");
            break;

        case 0x4c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, H");
            printf("\n");
            break;

        case 0x4d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, L");
            printf("\n");
            break;

        case 0x4e:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, M");
            printf("\n");
            break;

        case 0x4f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV C, A");
            printf("\n");
            break;

        case 0x50:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, B");
            printf("\n");
            break;

        case 0x51:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, C");
            printf("\n");
            break;

        case 0x52:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, D");
            printf("\n");
            break;

        case 0x53:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, E");
            printf("\n");
            break;

        case 0x54:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, H");
            printf("\n");
            break;

        case 0x55:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, L");
            printf("\n");
            break;

        case 0x56:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, M");
            printf("\n");
            break;

        case 0x57:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV D, A");
            printf("\n");
            break;

        case 0x58:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, B");
            printf("\n");
            break;

        case 0x59:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, C");
            printf("\n");
            break;

        case 0x5a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, D");
            printf("\n");
            break;

        case 0x5b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, E");
            printf("\n");
            break;

        case 0x5c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, H");
            printf("\n");
            break;

        case 0x5d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, L");
            printf("\n");
            break;

        case 0x5e:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, M");
            printf("\n");
            break;

        case 0x5f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV E, M");
            printf("\n");
            break;

        case 0x60:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, B");
            printf("\n");
            break;

        case 0x61:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, C");
            printf("\n");
            break;

        case 0x62:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, D");
            printf("\n");
            break;

        case 0x63:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, E");
            printf("\n");
            break;

        case 0x64:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, H");
            printf("\n");
            break;

        case 0x65:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, L");
            printf("\n");
            break;

        case 0x66:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, M");
            printf("\n");
            break;

        case 0x67:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV H, A");
            printf("\n");
            break;

        case 0x68:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, B");
            printf("\n");
            break;

        case 0x69:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, C");
            printf("\n");
            break;

        case 0x6a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, D");
            printf("\n");
            break;

        case 0x6b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, E");
            printf("\n");
            break;

        case 0x6c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, H");
            printf("\n");
            break;

        case 0x6d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, L");
            printf("\n");
            break;

        case 0x6e:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, M");
            printf("\n");
            break;

        case 0x6f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV L, A");
            printf("\n");
            break;

        case 0x70:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV M, B");
            printf("\n");
            break;

        case 0x71:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV M, C");
            printf("\n");
            break;

        case 0x72:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV M, D");
            printf("\n");
            break;

        case 0x73:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV M, E");
            printf("\n");
            break;

        case 0x74:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV M, H");
            printf("\n");
            break;

        case 0x75:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV M, L");
            printf("\n");
            break;

        case 0x76:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("HLT");
            printf("\n");
            break;

        case 0x77:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV M, A");
            printf("\n");
            break;

        case 0x78:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, B");
            printf("\n");
            break;

        case 0x79:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, C");
            printf("\n");
            break;

        case 0x7a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, D");
            printf("\n");
            break;

        case 0x7b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, E");
            printf("\n");
            break;

        case 0x7c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, H");
            printf("\n");
            break;

        case 0x7d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, L");
            printf("\n");
            break;

        case 0x7e:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, M");
            printf("\n");
            break;

        case 0x7f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("MOV A, A");
            printf("\n");
            break;

        case 0x80:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD B");
            printf("\n");
            break;

        case 0x81:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD C");
            printf("\n");
            break;

        case 0x82:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD D");
            printf("\n");
            break;

        case 0x83:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD E");
            printf("\n");
            break;

        case 0x84:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD H");
            printf("\n");
            break;

        case 0x85:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD L");
            printf("\n");
            break;

        case 0x86:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD M");
            printf("\n");
            break;

        case 0x87:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD A");
            printf("\n");
            break;

        case 0x88:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADC B");
            printf("\n");
            break;

        case 0x89:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD C");
            printf("\n");
            break;

        case 0x8a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADD D");
            printf("\n");
            break;

        case 0x8b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADC E");
            printf("\n");
            break;

        case 0x8c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADC H");
            printf("\n");
            break;

        case 0x8d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADC L");
            printf("\n");
            break;

        case 0x8e:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADC M");
            printf("\n");
            break;

        case 0x8f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ADC A");
            printf("\n");
            break;

        case 0x90:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB B");
            printf("\n");
            break;

        case 0x91:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB C");
            printf("\n");
            break;

        case 0x92:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB D");
            printf("\n");
            break;

        case 0x93:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB E");
            printf("\n");
            break;

        case 0x94:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB H");
            printf("\n");
            break;

        case 0x95:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB L");
            printf("\n");
            break;

        case 0x96:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB M");
            printf("\n");
            break;

        case 0x97:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB A");
            printf("\n");
            break;

        case 0x98:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SBB B");
            printf("\n");
            break;

        case 0x99:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SBB C");
            printf("\n");
            break;

        case 0x9a:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB D");
            printf("\n");
            break;

        case 0x9b:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SBB E");
            printf("\n");
            break;

        case 0x9c:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SBB H");
            printf("\n");
            break;

        case 0x9d:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SUB L");
            printf("\n");
            break;

        case 0x9e:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SBB M");
            printf("\n");
            break;

        case 0x9f:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SBB A");
            printf("\n");
            break;

        case 0xa0:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA B");
            printf("\n");
            break;

        case 0xa1:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA C");
            printf("\n");
            break;

        case 0xa2:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA D");
            printf("\n");
            break;

        case 0xa3:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA E");
            printf("\n");
            break;

        case 0xa4:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA H");
            printf("\n");
            break;

        case 0xa5:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA L");
            printf("\n");
            break;

        case 0xa6:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA M");
            printf("\n");
            break;

        case 0xa7:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ANA A");
            printf("\n");
            break;

        case 0xa8:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA B");
            printf("\n");
            break;

        case 0xa9:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA C");
            printf("\n");
            break;

        case 0xaa:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA D");
            printf("\n");
            break;

        case 0xab:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA E");
            printf("\n");
            break;

        case 0xac:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA H");
            printf("\n");
            break;

        case 0xad:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA L");
            printf("\n");
            break;

        case 0xae:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA M");
            printf("\n");
            break;

        case 0xaf:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XRA A");
            printf("\n");
            break;

        case 0xb0:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA B");
            printf("\n");
            break;

        case 0xb1:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA C");
            printf("\n");
            break;

        case 0xb2:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA D");
            printf("\n");
            break;

        case 0xb3:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA E");
            printf("\n");
            break;

        case 0xb4:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA H");
            printf("\n");
            break;

        case 0xb5:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA L");
            printf("\n");
            break;

        case 0xb6:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA M");
            printf("\n");
            break;

        case 0xb7:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("ORA A");
            printf("\n");
            break;

        case 0xb8:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP B");
            printf("\n");
            break;

        case 0xb9:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP C");
            printf("\n");
            break;

        case 0xba:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP D");
            printf("\n");
            break;

        case 0xbb:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP E");
            printf("\n");
            break;

        case 0xbc:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP H");
            printf("\n");
            break;

        case 0xbd:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP L");
            printf("\n");
            break;

        case 0xbe:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP M");
            printf("\n");
            break;

        case 0xbf:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("CMP A");
            printf("\n");
            break;

        case 0xc0:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RNZ");
            printf("\n");
            break;

        case 0xc1:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("POP B");
            printf("\n");
            break;

        case 0xc2:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JNZ 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xc3:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JMP 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xc4:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CNZ 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xc5:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("PUSH B");
            printf("\n");
            break;

        case 0xc6:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("ADI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xc7:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 0");
            printf("\n");
            break;

        case 0xc8:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RZ");
            printf("\n");
            break;

        case 0xc9:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RET");
            printf("\n");
            break;

        case 0xca:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JZ 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xcb:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0xcc:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CZ 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xcd:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CALL 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xce:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("ACI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xcf:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 1");
            printf("\n");
            break;

        case 0xd0:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RNC");
            printf("\n");
            break;

        case 0xd1:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("POP D");
            printf("\n");
            break;

        case 0xd2:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JNC 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xd3:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("OUT #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xd4:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CNC 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xd5:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("PUSH D");
            printf("\n");
            break;

        case 0xd6:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("SUI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xd7:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 2");
            printf("\n");
            break;

        case 0xd8:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RC");
            printf("\n");
            break;

        case 0xd9:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0xda:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JC 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xdb:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("IN #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xdc:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CC 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xdd:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0xde:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("SBI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xdf:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 3");
            printf("\n");
            break;

        case 0xe0:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RPO");
            printf("\n");
            break;

        case 0xe1:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("POP H");
            printf("\n");
            break;

        case 0xe2:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JPO 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xe3:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("XTHL");
            printf("\n");
            break;

        case 0xe4:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CPO 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xe5:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("PUSH H");
            printf("\n");
            break;

        case 0xe6:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("ANI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xe7:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 4");
            printf("\n");
            break;

        case 0xe8:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RPE 1");
            printf("\n");
            break;

        case 0xe9:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("PCHL");
            printf("\n");
            break;

        case 0xea:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JPE 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xeb:
            printf("%02x ", *opcode);
            printf("\t\t");
            printf("XCHG");
            printf("\n");
            break;

        case 0xec:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CPE 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xed:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("-");
            printf("\n");
            break;

        case 0xee:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("XRI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xef:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 5");
            printf("\n");
            break;

        case 0xf0:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RP");
            printf("\n");
            break;

        case 0xf1:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("POP PSW");
            printf("\n");
            break;

        case 0xf2:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JP 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xf3:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("DI");
            printf("\n");
            break;

        case 0xf4:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CP 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xf5:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("PUSH PSW");
            printf("\n");
            break;

        case 0xf6:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("ORI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xf7:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 6");
            printf("\n");
            break;

        case 0xf8:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RM");
            printf("\n");
            break;

        case 0xf9:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("SPHL");
            printf("\n");
            break;

        case 0xfa:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("JM 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xfb:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("EI");
            printf("\n");
            break;

        case 0xfc:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("%02x ", opcode[2]);
            printf("\t");
            printf("CM 0x%02x%02x", opcode[2], opcode[1]);
            printf("\n");
            break;

        case 0xfe:
            printf("%02x ", *opcode);
            printf("%02x ", opcode[1]);
            printf("\t\t");
            printf("CPI #0x%02x", opcode[1]);
            printf("\n");
            break;

        case 0xff:
            printf("%02x", *opcode);
            printf("\t\t");
            printf("RST 7");
            printf("\n");
            break;

        default:
            printf("\nUnimplemented instruction 0x%02x\n", *opcode);
            exit(1);

        printf("\n");
    }
}

void
dissas_rom(uint8_t *rom, size_t len)
{
    size_t i = 0;

    // Print all operations
    while (i < len) {
        printf("0x%08lx\t", i);

        switch (rom[i]) {

            case 0x00:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("NOP");
                i += 1;
                break;

            case 0x01:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("LXI B #0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x02:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("STAX B");
                i += 1;
                break;

            case 0x03:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("STAX B");
                i += 1;
                break;

            case 0x04:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR B");
                i += 1;
                break;

            case 0x05:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR B");
                i += 1;
                break;

            case 0x06:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI B #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x07:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RLC");
                i += 1;
                break;

            case 0x08: // Not specified
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0x09:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DAD B");
                i += 1;
                break;

            case 0x0a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("LDAX B");
                i += 1;
                break;

            case 0x0b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCX B");
                i += 1;
                break;

            case 0x0c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR C");
                i += 1;
                break;

            case 0x0d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR C");
                i += 1;
                break;

            case 0x0e:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI C #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x0f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RRC");
                i += 1;
                break;

            case 0x10: // Not specified
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0x11:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("LXI D #0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x12:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("STAX D");
                i += 1;
                break;

            case 0x13:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INX D");
                i += 1;
                break;

            case 0x14:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR D");
                i += 1;
                break;

            case 0x15:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR D");
                i += 1;
                break;

            case 0x16:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI D, #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x17:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RAL");
                i += 1;
                break;

            case 0x18: // Not specified
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0x19:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DAD D");
                i += 1;
                break;

            case 0x1a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("LDAX D");
                i += 1;
                break;

            case 0x1b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCX D");
                i += 1;
                break;

            case 0x1c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR E");
                i += 1;
                break;

            case 0x1d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR E");
                i += 1;
                break;

            case 0x1e:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI E #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x1f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RAR");
                i += 1;
                break;

            case 0x20:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RIM");
                i += 1;
                break;

            case 0x21:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("LXI H #0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x22:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("SHLD 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x23:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INX H");
                i += 1;
                break;

            case 0x24:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR H");
                i += 1;
                break;

            case 0x25:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR H");
                i += 1;
                break;

            case 0x26:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI H, #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x27:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("DAA");
                i += 2;
                break;

            case 0x28: // Not specified
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0x29:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DAD H");
                i += 1;
                break;

            case 0x2a:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("LHLD 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x2b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCX H");
                i += 1;
                break;

            case 0x2c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR L");
                i += 1;
                break;

            case 0x2d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR L");
                i += 1;
                break;

            case 0x2e:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI L #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x2f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMA");
                i += 1;
                break;

            case 0x30:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SIM");
                i += 1;
                break;

            case 0x31:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("LXI SP #0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x32:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("STA 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x33:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INX SP");
                i += 1;
                break;

            case 0x34:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR M");
                i += 1;
                break;

            case 0x35:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR M");
                i += 1;
                break;

            case 0x36:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI M, #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x37:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("STC");
                i += 1;
                break;

            case 0x38:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0x39:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DAD SP");
                i += 1;
                break;

            case 0x3a:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("LDA 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0x3b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCX SP");
                i += 1;
                break;

            case 0x3c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("INR A");
                i += 1;
                break;

            case 0x3d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DCR A");
                i += 1;
                break;

            case 0x3e:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("MVI A ##0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0x3f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMC");
                i += 1;
                break;

            case 0x40:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, B");
                i += 1;
                break;

            case 0x41:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, C");
                i += 1;
                break;

            case 0x42:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, D");
                i += 1;
                break;

            case 0x43:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, E");
                i += 1;
                break;

            case 0x44:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, H");
                i += 1;
                break;

            case 0x45:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, L");
                i += 1;
                break;

            case 0x46:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, M");
                i += 1;
                break;

            case 0x47:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV B, A");
                i += 1;
                break;

            case 0x48:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, B");
                i += 1;
                break;

            case 0x49:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, C");
                i += 1;
                break;

            case 0x4a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, D");
                i += 1;
                break;

            case 0x4b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, E");
                i += 1;
                break;

            case 0x4c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, H");
                i += 1;
                break;

            case 0x4d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, L");
                i += 1;
                break;

            case 0x4e:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, M");
                i += 1;
                break;

            case 0x4f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV C, A");
                i += 1;
                break;

            case 0x50:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, B");
                i += 1;
                break;

            case 0x51:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, C");
                i += 1;
                break;

            case 0x52:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, D");
                i += 1;
                break;

            case 0x53:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, E");
                i += 1;
                break;

            case 0x54:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, H");
                i += 1;
                break;

            case 0x55:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, L");
                i += 1;
                break;

            case 0x56:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, M");
                i += 1;
                break;

            case 0x57:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV D, A");
                i += 1;
                break;

            case 0x58:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, B");
                i += 1;
                break;

            case 0x59:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, C");
                i += 1;
                break;

            case 0x5a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, D");
                i += 1;
                break;

            case 0x5b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, E");
                i += 1;
                break;

            case 0x5c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, H");
                i += 1;
                break;

            case 0x5d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, L");
                i += 1;
                break;

            case 0x5e:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, M");
                i += 1;
                break;

            case 0x5f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV E, M");
                i += 1;
                break;

            case 0x60:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, B");
                i += 1;
                break;

            case 0x61:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, C");
                i += 1;
                break;

            case 0x62:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, D");
                i += 1;
                break;

            case 0x63:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, E");
                i += 1;
                break;

            case 0x64:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, H");
                i += 1;
                break;

            case 0x65:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, L");
                i += 1;
                break;

            case 0x66:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, M");
                i += 1;
                break;

            case 0x67:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV H, A");
                i += 1;
                break;

            case 0x68:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, B");
                i += 1;
                break;

            case 0x69:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, C");
                i += 1;
                break;

            case 0x6a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, D");
                i += 1;
                break;

            case 0x6b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, E");
                i += 1;
                break;

            case 0x6c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, H");
                i += 1;
                break;

            case 0x6d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, L");
                i += 1;
                break;

            case 0x6e:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, M");
                i += 1;
                break;

            case 0x6f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV L, A");
                i += 1;
                break;

            case 0x70:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV M, B");
                i += 1;
                break;

            case 0x71:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV M, C");
                i += 1;
                break;

            case 0x72:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV M, D");
                i += 1;
                break;

            case 0x73:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV M, E");
                i += 1;
                break;

            case 0x74:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV M, H");
                i += 1;
                break;

            case 0x75:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV M, L");
                i += 1;
                break;

            case 0x76:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("HLT");
                i += 1;
                break;

            case 0x77:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV M, A");
                i += 1;
                break;

            case 0x78:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, B");
                i += 1;
                break;

            case 0x79:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, C");
                i += 1;
                break;

            case 0x7a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, D");
                i += 1;
                break;

            case 0x7b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, E");
                i += 1;
                break;

            case 0x7c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, H");
                i += 1;
                break;

            case 0x7d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, L");
                i += 1;
                break;

            case 0x7e:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, M");
                i += 1;
                break;

            case 0x7f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("MOV A, A");
                i += 1;
                break;

            case 0x80:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD B");
                i += 1;
                break;

            case 0x81:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD C");
                i += 1;
                break;

            case 0x82:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD D");
                i += 1;
                break;

            case 0x83:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD E");
                i += 1;
                break;

            case 0x84:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD H");
                i += 1;
                break;

            case 0x85:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD L");
                i += 1;
                break;

            case 0x86:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD M");
                i += 1;
                break;

            case 0x87:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD A");
                i += 1;
                break;

            case 0x88:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADC B");
                i += 1;
                break;

            case 0x89:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD C");
                i += 1;
                break;

            case 0x8a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADD D");
                i += 1;
                break;

            case 0x8b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADC E");
                i += 1;
                break;

            case 0x8c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADC H");
                i += 1;
                break;

            case 0x8d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADC L");
                i += 1;
                break;

            case 0x8e:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADC M");
                i += 1;
                break;

            case 0x8f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ADC A");
                i += 1;
                break;

            case 0x90:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB B");
                i += 1;
                break;

            case 0x91:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB C");
                i += 1;
                break;

            case 0x92:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB D");
                i += 1;
                break;

            case 0x93:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB E");
                i += 1;
                break;

            case 0x94:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB H");
                i += 1;
                break;

            case 0x95:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB L");
                i += 1;
                break;

            case 0x96:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB M");
                i += 1;
                break;

            case 0x97:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB A");
                i += 1;
                break;

            case 0x98:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SBB B");
                i += 1;
                break;

            case 0x99:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SBB C");
                i += 1;
                break;

            case 0x9a:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB D");
                i += 1;
                break;

            case 0x9b:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SBB E");
                i += 1;
                break;

            case 0x9c:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SBB H");
                i += 1;
                break;

            case 0x9d:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SUB L");
                i += 1;
                break;

            case 0x9e:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SBB M");
                i += 1;
                break;

            case 0x9f:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SBB A");
                i += 1;
                break;

            case 0xa0:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA B");
                i += 1;
                break;

            case 0xa1:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA C");
                i += 1;
                break;

            case 0xa2:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA D");
                i += 1;
                break;

            case 0xa3:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA E");
                i += 1;
                break;

            case 0xa4:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA H");
                i += 1;
                break;

            case 0xa5:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA L");
                i += 1;
                break;

            case 0xa6:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA M");
                i += 1;
                break;

            case 0xa7:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ANA A");
                i += 1;
                break;

            case 0xa8:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA B");
                i += 1;
                break;

            case 0xa9:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA C");
                i += 1;
                break;

            case 0xaa:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA D");
                i += 1;
                break;

            case 0xab:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA E");
                i += 1;
                break;

            case 0xac:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA H");
                i += 1;
                break;

            case 0xad:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA L");
                i += 1;
                break;

            case 0xae:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA M");
                i += 1;
                break;

            case 0xaf:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XRA A");
                i += 1;
                break;

            case 0xb0:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA B");
                i += 1;
                break;

            case 0xb1:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA C");
                i += 1;
                break;

            case 0xb2:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA D");
                i += 1;
                break;

            case 0xb3:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA E");
                i += 1;
                break;

            case 0xb4:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA H");
                i += 1;
                break;

            case 0xb5:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA L");
                i += 1;
                break;

            case 0xb6:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA M");
                i += 1;
                break;

            case 0xb7:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("ORA A");
                i += 1;
                break;

            case 0xb8:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP B");
                i += 1;
                break;

            case 0xb9:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP C");
                i += 1;
                break;

            case 0xba:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP D");
                i += 1;
                break;

            case 0xbb:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP E");
                i += 1;
                break;

            case 0xbc:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP H");
                i += 1;
                break;

            case 0xbd:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP L");
                i += 1;
                break;

            case 0xbe:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP M");
                i += 1;
                break;

            case 0xbf:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("CMP A");
                i += 1;
                break;

            case 0xc0:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RNZ");
                i += 1;
                break;

            case 0xc1:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("POP B");
                i += 1;
                break;

            case 0xc2:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JNZ 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xc3:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JMP 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xc4:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CNZ 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xc5:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("PUSH B");
                i += 1;
                break;

            case 0xc6:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("ADI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xc7:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 0");
                i += 1;
                break;

            case 0xc8:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RZ");
                i += 1;
                break;

            case 0xc9:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RET");
                i += 1;
                break;

            case 0xca:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JZ 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xcb:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0xcc:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CZ 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xcd:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CALL 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xce:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("ACI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xcf:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 1");
                i += 1;
                break;

            case 0xd0:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RNC");
                i += 1;
                break;

            case 0xd1:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("POP D");
                i += 1;
                break;

            case 0xd2:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JNC 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xd3:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("OUT #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xd4:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CNC 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xd5:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("PUSH D");
                i += 1;
                break;

            case 0xd6:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("SUI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xd7:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 2");
                i += 1;
                break;

            case 0xd8:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RC");
                i += 1;
                break;

            case 0xd9:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0xda:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JC 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xdb:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("IN #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xdc:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CC 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xdd:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0xde:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("SBI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xdf:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 3");
                i += 1;
                break;

            case 0xe0:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RPO");
                i += 1;
                break;

            case 0xe1:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("POP H");
                i += 1;
                break;

            case 0xe2:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JPO 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xe3:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("XTHL");
                i += 1;
                break;

            case 0xe4:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CPO 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xe5:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("PUSH H");
                i += 1;
                break;

            case 0xe6:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("ANI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xe7:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 4");
                i += 1;
                break;

            case 0xe8:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RPE 1");
                i += 1;
                break;

            case 0xe9:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("PCHL");
                i += 1;
                break;

            case 0xea:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JPE 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xeb:
                printf("%02x ", rom[i]);
                printf("\t\t");
                printf("XCHG");
                i += 1;
                break;

            case 0xec:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CPE 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xed:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("-");
                i += 1;
                break;

            case 0xee:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("XRI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xef:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 5");
                i += 1;
                break;

            case 0xf0:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RP");
                i += 1;
                break;

            case 0xf1:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("POP PSW");
                i += 1;
                break;

            case 0xf2:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JP 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xf3:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("DI");
                i += 1;
                break;

            case 0xf4:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CP 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xf5:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("PUSH PSW");
                i += 1;
                break;

            case 0xf6:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("ORI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xf7:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 6");
                i += 1;
                break;

            case 0xf8:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RM");
                i += 1;
                break;

            case 0xf9:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("SPHL");
                i += 1;
                break;

            case 0xfa:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("JM 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xfb:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("EI");
                i += 1;
                break;

            case 0xfc:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("%02x ", rom[i+2]);
                printf("\t");
                printf("CM 0x%02x%02x", rom[i+2], rom[i+1]);
                i += 3;
                break;

            case 0xfe:
                printf("%02x ", rom[i]);
                printf("%02x ", rom[i+1]);
                printf("\t\t");
                printf("CPI #0x%02x", rom[i+1]);
                i += 2;
                break;

            case 0xff:
                printf("%02x", rom[i]);
                printf("\t\t");
                printf("RST 7");
                i += 1;
                break;

            default:
                printf("\nUnimplemented instruction 0x%02x\n", rom[i]);
                exit(1);

        }
        printf("\n");
    }
}
