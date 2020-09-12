#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "intel8080.h"
#include "dissas.h"

#define FOR_CPUDIAG

uint64_t count = 0;

static void
print_state(State *state)
{
    printf("\n");
    printf("REGISTERS:\tFLAGS:\n");
    printf("sp: 0x%04x\t", state->sp);
    printf("cy:  %d\n", state->cc.cy);
    printf("pc: 0x%04x\t", state->pc);
    printf("p:   %d\n", state->cc.p);
    printf("a:  0x%02x\t", state->a);
    printf("z:   %d\n", state->cc.z);
    printf("b:  0x%02x\t", state->b);
    printf("ac:  %d\n", state->cc.ac);
    printf("c:  0x%02x\t", state->c);
    printf("pad: %d\n", state->cc.pad);
    printf("d:  0x%02x\n", state->d);
    printf("e:  0x%02x\n", state->e);
    printf("h:  0x%02x\n", state->h);
    printf("l:  0x%02x\n", state->l);
}

static void
unimp(State *state)
{
    printf("Unimplemented opcode: 0x%02x\n", state->memory[state->pc]);
    print_state(state);
    printf("Not taken\n"); // debug
    exit(1);
}

//static int
//parity(uint16_t number)
//{
//    int bits_n = 0;
//
//    if ((number & 0x80) != 0) bits_n++;
//    if ((number & 0x40) != 0) bits_n++;
//    if ((number & 0x20) != 0) bits_n++;
//    if ((number & 0x10) != 0) bits_n++;
//    if ((number & 0x8)  != 0) bits_n++;
//    if ((number & 0x4)  != 0) bits_n++;
//    if ((number & 0x2)  != 0) bits_n++;
//    if ((number & 0x1)  != 0) bits_n++;
//
//    if (bits_n % 2 == 0)
//        return 1;
//    else
//        return 0;
//}
//
int parity2(int x, int size)
{
	int i;
	int p = 0;
	x = (x & ((1<<size)-1));
	for (i=0; i<size; i++)
	{
		if (x & 0x1) p++;
		x = x >> 1;
	}
	return (0 == (p & 0x1));
}

static void
emulate(State *state)
{
    // Dissasemble current instruction
    dissas_curr_inst(state);

    // Use addres of opcode instead of value to
    // access data / addreses following it easily
    uint8_t *opcode = &state->memory[state->pc];

    switch (*opcode) {

        // NOP
        case 0x00:
            state->pc++;
            break;

        // Load 16 bit value into register pair
        // LXI B, D16
        case 0x01:
            state->c  = opcode[1];
            state->b  = opcode[2];
            state->pc += 3;
            break;

        // STAX B
        case 0x02:
        {
            uint16_t target_addr = (state->b << 8) | state->c;
            state->memory[target_addr] = state->a;
            state->pc++;
        }
        break;

        // INX B
        case 0x03:
            state->c++;
            if (state->c == 0) {
                state->b++;
            }
            state->pc++;
            break;

        // INR B
        case 0x04:
            state->b++;
            state->cc.z = (state->b == 0);
            state->cc.s = ((state->b & 0x80) != 0);
            state->cc.p = parity2(state->b, 8);
            state->pc++;
            break;

        // DCR b
        case 0x05:
            printf("Taken\n"); // debug
            printf("Taken\n"); // debug
            {
                uint8_t res = state->b - 1;
                printf("Not taken\n"); // debug
                state->cc.z = (res == 0);
                printf("Not taken\n"); // debug
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->b    = res;
                state->pc++;
            }
            break;

        // MVI B, D8
        case 0x06:
            state->b   = opcode[1];
            state->pc += 2;
            break;

        // LDAX B
        case 0x0a:
            {
                uint16_t target_addr = (state->b << 8) | state->c;
                state->a             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // DCX B
        case 0x0b:
            state->c--;
            if (state->c == 0xff) {
                state->b--;
            }
            state->pc++;
            break;

        // INR C
        case 0x0c:
            state->c++;
            state->cc.z = (state->c == 0);
            state->cc.s = ((state->c & 0x80) != 0);
            state->cc.p = parity2(state->c, 8);
            state->pc++;
            break;

        // DCR C
        case 0x0d:
            {
                uint8_t res = state->c - 1;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->c    = res;
                state->pc++;
            }
            break;

        // MVI C, D8
        case 0x0e:
            state->c   = opcode[1];
            state->pc += 2;
            break;

        // LXI D, D16
        case 0x11:
            state->d   = opcode[2];
            state->e   = opcode[1];
            state->pc += 3;
            break;

        // STAX D
        case 0x12:
        {
            uint16_t target_addr = (state->d << 8) | state->e;
            state->memory[target_addr] = state->a;
            state->pc++;
        }
        break;

        // INX D
        // Increment register pair D + E
        case 0x13:
            state->e++;
            // If e overflows into D, inc D
            if (state->e == 0)
                state->d++;
            state->pc++;
            break;

        // INR D
        case 0x14:
            state->d++;
            state->cc.z = (state->d == 0);
            state->cc.s = ((state->d & 0x80) != 0);
            state->cc.p = parity2(state->d, 8);
            state->pc++;
            break;

        // DCR D
        case 0x15:
            {
                uint8_t res = state->d - 1;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->d    = res;
                state->pc++;
            }
            break;

        // MVI D, D8
        case 0x16:
            state->d   = opcode[1];
            state->pc += 2;
            break;

        // DAD D
        // Double add HL + DE
        case 0x19:
            {
                uint32_t hl = (state->h << 8) | state->l;
                uint32_t de = (state->d << 8) | state->e;
                uint32_t res = hl + de;
                state->h = (res & 0xff00) >> 8;           // Two highest bytes of res
                state->l = res & 0xff;                    // Two lowest bytes of res
                state->cc.cy = ((res & 0xffff0000) != 0); // Set carry flag if res overflows 32 bit int
                state->pc++;
            }
            break;

        // DCX D
        case 0x1b:
            state->e--;
            if (state->e == 0xff) {
                state->d--;
            }
            state->pc++;
            break;

        // INR E
        case 0x1c:
            state->e++;
            state->cc.z = (state->e == 0);
            state->cc.s = ((state->e & 0x80) != 0);
            state->cc.p = parity2(state->e, 8);
            state->pc++;
            break;

        // DCR E
        case 0x1d:
            {
                uint8_t res = state->e - 1;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->e    = res;
                state->pc++;
            }
            break;

        // MVI E, D8
        case 0x1e:
            state->e   = opcode[1];
            state->pc += 2;
            break;

        // LDAX D
        case 0x1a:
            {
                uint16_t target_addr = (state->d << 8) | state->e;
                state->a             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // LXI H, D16
        case 0x21:
            state->h   = opcode[2];
            state->l   = opcode[1];
            state->pc += 3;
            break;

        // SHLD addr
        case 0x22:
            {
                uint16_t target_addr           = (opcode[2] << 8) | opcode[1];
                state->memory[target_addr]     = state->l;
                state->memory[target_addr + 1] = state->h;
                state->pc                     += 3;
            }
            break;

        // INX H
        case 0x23:
            state->l++;
            if (state->l == 0)
                state->h++;
            state->pc++;
            break;

        // INR H
        case 0x24:
            state->h++;
            state->cc.z = (state->h == 0);
            state->cc.s = ((state->h & 0x80) != 0);
            state->cc.p = parity2(state->h, 8);
            state->pc++;
            break;

        // DCR H
        case 0x25:
            {
                uint8_t res = state->h - 1;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->h    = res;
                state->pc++;
            }
            break;

        // MVI H, D8
        case 0x26:
            state->h   = opcode[1];
            state->pc += 2;
            break;

        // LHLD addr
        case 0x2a:
            {
                uint16_t target_addr = (opcode[2] << 8) | opcode[1];
                state->l             = state->memory[target_addr];
                state->h             = state->memory[target_addr + 1];
                state->pc           += 3;
            }
            break;

        // DCX H
        case 0x2b:
            state->l--;
            if (state->l == 0xff) {
                state->h--;
            }
            state->pc++;
            break;

        // INR L
        case 0x2c:
            state->l++;
            state->cc.z = (state->l == 0);
            state->cc.s = ((state->l & 0x80) != 0);
            state->cc.p = parity2(state->l, 8);
            state->pc++;
            break;

        // DCR L
        case 0x2d:
            {
                uint8_t res = state->l - 1;

                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->l    = res;
                state->pc++;
            }
            break;

        // MVI L, D8
        case 0x2e:
            state->l   = opcode[1];
            state->pc += 2;
            break;

        // LXI SP, D16
        case 0x31:
            state->sp  = (opcode[2] << 8) | opcode[1];
            state->pc += 3;
            break;

        // STA adr
        // Replace value at direct address with value of accumulator
        case 0x32:
            {
                uint16_t target_addr = (opcode[2] << 8) | opcode[1];
                state->memory[target_addr] = state->a;
                state->pc += 3;
            }
            break;

        // INR M
        case 0x34:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr]++;
                state->cc.z          = (state->memory[target_addr] == 0);
                state->cc.s          = ((state->memory[target_addr] & 0x80) != 0);
                state->cc.p          = parity2(state->memory[target_addr], 8);
                state->pc++;
            }
            break;

        // DCR M
        case 0x35:
            {
                uint16_t target_addr       = (state->h << 8) | state->l;
                uint8_t res                = state->memory[target_addr] - 1;
                state->cc.z                = (res == 0);
                state->cc.s                = ((res & 0x80) != 0);
                state->cc.p                = parity2(res, 8);
                state->memory[target_addr] = res;
                state->pc++;
            }
            break;

        // MVI M, D8
        case 0x36:
            {
                uint16_t target_addr       = (state->h << 8) | state->l;
                state->memory[target_addr] = opcode[1];
                state->pc += 2;
            }
            break;

        // LDA addr
        case 0x3a:
            {
                uint16_t target_addr = (opcode[2] << 8) | opcode[1];
                state->a   = state->memory[target_addr];
                state->pc += 3;
            }
            break;

        // DCX SP
        case 0x3b:
            state->sp--;
            state->pc++;
            break;

        // INR A
        case 0x3c:
            state->a++;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->pc++;
            break;

        // DCR A
        case 0x3d:
            {
                uint8_t res = state->a - 1;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->a    = res;
                state->pc++;
            }
            break;

        // MVI A, D8
        case 0x3e:
            state->a   = opcode[1];
            state->pc += 2;
            break;

        // MOV B, B
        case 0x40:
            state->b = state->b;
            state->pc++;
            break;

        // MOV B, C
        case 0x41:
            state->b = state->c;
            state->pc++;
            break;

        // MOV B, D
        case 0x42:
            state->b = state->d;
            state->pc++;
            break;

        // MOV B, E
        case 0x43:
            state->b = state->e;
            state->pc++;
            break;

        // MOV B, H
        case 0x44:
            state->b = state->h;
            state->pc++;
            break;

        // MOV B, L
        case 0x45:
            state->b = state->l;
            state->pc++;
            break;

        // MOV B, M
        case 0x46:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->b             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // MOV B, A
        case 0x47:
            state->b = state->a;
            state->pc++;
            break;

        // MOV C, B
        case 0x48:
            state->c = state->b;
            state->pc++;
            break;

        // MOV C, C
        case 0x49:
            state->c = state->c;
            state->pc++;
            break;

        // MOV C, D
        case 0x4a:
            state->c = state->d;
            state->pc++;
            break;

        // MOV C, E
        case 0x4b:
            state->c = state->e;
            state->pc++;
            break;

        // MOV C, H
        case 0x4c:
            state->c = state->h;
            state->pc++;
            break;

        // MOV C, L
        case 0x4d:
            state->c = state->l;
            state->pc++;
            break;

        // MOV C, M
        case 0x4e:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->c             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // MOV C, A
        case 0x4f:
            state->c = state->a;
            state->pc++;
            break;

        // MOV D, B
        case 0x50:
            state->d = state->b;
            state->pc++;
            break;

        // MOV D, C
        case 0x51:
            state->d = state->c;
            state->pc++;
            break;

        // MOV D, D
        case 0x52:
            state->d = state->d;
            state->pc++;
            break;

        // MOV D, E
        case 0x53:
            state->d = state->e;
            state->pc++;
            break;

        // MOV D, H
        case 0x54:
            state->d = state->h;
            state->pc++;
            break;

        // MOV D, L
        case 0x55:
            state->d = state->l;
            state->pc++;
            break;

        // MOV D, L
        case 0x56:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->d             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // MOV D, A
        case 0x57 :
            state->d = state->a;
            state->pc++;
            break;

        // MOV E, B
        case 0x58:
            state->e = state->b;
            state->pc++;
            break;

        // MOV E, C
        case 0x59:
            state->e = state->c;
            state->pc++;
            break;

        // MOV E, D
        case 0x5a:
            state->e = state->d;
            state->pc++;
            break;

        // MOV E, E
        case 0x5b:
            state->e = state->e;
            state->pc++;
            break;

        // MOV E, H
        case 0x5c:
            state->e = state->h;
            state->pc++;
            break;

        // MOV E, L
        case 0x5d:
            state->e = state->l;
            state->pc++;
            break;

        // MOV E, M
        case 0x5e:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->e             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // MOV E, A
        case 0x5f:
            state->e = state->a;
            state->pc++;
            break;

        // MOV H, B
        case 0x60:
            state->h = state->b;
            state->pc++;
            break;

        // MOV H, C
        case 0x61:
            state->h = state->c;
            state->pc++;
            break;

        // MOV H, D
        case 0x62:
            state->h = state->d;
            state->pc++;
            break;

        // MOV H, E
        case 0x63:
            state->h = state->e;
            state->pc++;
            break;

        // MOV H, H
        case 0x64:
            state->h = state->h;
            state->pc++;
            break;

        // MOV H, L
        case 0x65:
            state->h = state->l;
            state->pc++;
            break;

        // MOV H, M
        case 0x66:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->h             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // MOV H, A
        case 0x67:
            state->h = state->a;
            state->pc++;
            break;

        // MOV L, B
        case 0x68:
            state->l = state->b;
            state->pc++;
            break;

        // MOV L, C
        case 0x69:
            state->l = state->c;
            state->pc++;
            break;

        // MOV L, D
        case 0x6a:
            state->l = state->d;
            state->pc++;
            break;

        // MOV L, E
        case 0x6b:
            state->l = state->e;
            state->pc++;
            break;

        // MOV L, H
        case 0x6c:
            state->l = state->h;
            state->pc++;
            break;

        // MOV L, L
        case 0x6d:
            state->l = state->l;
            state->pc++;
            break;

        // MOV L, M
        case 0x6e:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->l             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // MOV L, A
        case 0x6f:
            state->l = state->a;
            state->pc++;
            break;

        // MOV M, B
        case 0x70:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr] = state->b;
                state->pc++;
            }
            break;

        // MOV M, C
        case 0x71:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr] = state->c;
                state->pc++;
            }
            break;

        // MOV M, D
        case 0x72:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr] = state->d;
                state->pc++;
            }
            break;

        // MOV M, E
        case 0x73:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr] = state->e;
                state->pc++;
            }
            break;

        // MOV M, H
        case 0x74:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr] = state->h;
                state->pc++;
            }
            break;

        // MOV M, L
        case 0x75:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr] = state->l;
                state->pc++;
            }
            break;

        // HLT
        case 0x76:
            break;

        // MOV M, A
        case 0x77:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->memory[target_addr] = state->a;
            }
            state->pc++;
            break;

        // MOV A, B
        case 0x78:
            state->a = state->b;
            state->pc++;
            break;

        // MOV A, C
        case 0x79:
            state->a = state->c;
            state->pc++;
            break;

        // MOV A, D
        case 0x7a:
            state->a = state->d;
            state->pc++;
            break;

        // MOV A, E
        case 0x7b:
            state->a = state->e;
            state->pc++;
            break;

        // MOV A, H
        case 0x7c:
            state->a = state->h;
            state->pc++;
            break;

        // MOV A, L
        case 0x7d:
            state->a = state->l;
            state->pc++;
            break;

        // MOV A, M
        case 0x7e:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->a             = state->memory[target_addr];
                state->pc++;
            }
            break;

        // MOV A, A
        case 0x7f:
            state->a = state->a;
            state->pc++;
            break;

        // ADD B
        case 0x80:
            {
                uint16_t res = state->a + state->b;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADD C
        case 0x81:
            {
                uint16_t res = state->a + state->c;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADD D
        case 0x82:
            {
                uint16_t res = state->a + state->d;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADD E
        case 0x83:
            {
                uint16_t res = state->a + state->e;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADD H
        case 0x84:
            {
                uint16_t res = state->a + state->h;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADD L
        case 0x85:
            {
                uint16_t res = state->a + state->l;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADD M
        case 0x86:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                uint16_t res = state->a + state->memory[target_addr];
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADD A
        case 0x87:
            {
                uint16_t res = state->a + state->a;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC B
        case 0x88:
            {
                uint16_t res = state->a + state->b + state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC C
        case 0x89:
            {
                uint16_t res = state->a + state->c + state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC D
        case 0x8a:
            {
                uint16_t res = state->a + state->d + state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC E
        case 0x8b:
            {
                uint16_t res = state->a + state->e + state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC H
        case 0x8c:
            {
                uint16_t res = state->a + state->h + state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC L
        case 0x8d:
            {
                uint16_t res = state->a + state->l + state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC M
        case 0x8e:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                uint16_t res         = state->a + state->memory[target_addr] + state->cc.cy;
                state->cc.z          = (res == 0);
                state->cc.s          = ((res & 0x80) != 0);
                state->cc.p          = (parity2(res, 8));
                state->cc.cy         = (res > 0xff);
                state->a             = (res & 0xff);
                state->pc++;
            }
            break;

        // ADC A
        case 0x8f:
            {
                uint16_t res = state->a + state->a + state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB B
        case 0x90:
            {
                uint16_t res = state->a - state->b;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB C
        case 0x91:
            {
                uint16_t res = state->a - state->c;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB D
        case 0x92:
            {
                uint16_t res = state->a - state->d;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB E
        case 0x93:
            {
                uint16_t res = state->a - state->e;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB H
        case 0x94:
            {
                uint16_t res = state->a - state->h;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB L
        case 0x95:
            {
                uint16_t res = state->a - state->l;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB M
        case 0x96:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                uint16_t res         = state->a - state->memory[target_addr];
                state->cc.z          = (res == 0);
                state->cc.s          = ((res & 0x80) != 0);
                state->cc.p          = (parity2(res, 8));
                state->cc.cy         = (res > 0xff);
                state->a             = (res & 0xff);
                state->pc++;
            }
            break;

        // SUB A
        case 0x97:
            {
                uint16_t res = state->a - state->a;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB B
        case 0x98:
            {
                uint16_t res = state->a - state->b - state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB C
        case 0x99:
            {
                uint16_t res = state->a - state->c - state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB D
        case 0x9a:
            {
                uint16_t res = state->a - state->d - state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB E
        case 0x9b:
            {
                uint16_t res = state->a - state->e - state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB H
        case 0x9c:
            {
                uint16_t res = state->a - state->h - state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB L
        case 0x9d:
            {
                uint16_t res = state->a - state->l - state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB M
        case 0x9e:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                uint16_t res         = state->a - state->memory[target_addr] - state->cc.cy;
                state->cc.z          = (res == 0);
                state->cc.s          = ((res & 0x80) != 0);
                state->cc.p          = (parity2(res, 8));
                state->cc.cy         = (res > 0xff);
                state->a             = (res & 0xff);
                state->pc++;
            }
            break;

        // SBB A
        case 0x9f:
            {
                uint16_t res = state->a - state->a - state->cc.cy;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->cc.cy = (res > 0xff);
                state->a     = (res & 0xff);
                state->pc++;
            }
            break;

        // ANA B
        case 0xa0:
            state->a     = state->a & state->b;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ANA C
        case 0xa1:
            state->a     = state->a & state->c;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ANA D
        case 0xa2:
            state->a     = state->a & state->d;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ANA E
        case 0xa3:
            state->a     = state->a & state->e;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ANA H
        case 0xa4:
            state->a     = state->a & state->h;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ANA L
        case 0xa5:
            state->a     = state->a & state->l;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ANA M
        case 0xa6:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->a             = state->a & state->memory[target_addr];
                state->cc.z          = (state->a == 0);
                state->cc.s          = ((state->a & 0x80) != 0);
                state->cc.p          = parity2(state->a, 8);
                state->cc.cy         = 0;
                state->pc++;
            }
            break;

        // ANA A
        case 0xa7:
            state->a     = state->a & state->a;
            state->cc.cy = 0;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->pc++;
            break;

        // XRA B
        case 0xa8:
            state->a = state->a ^ state->b;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // XRA C
        case 0xa9:
            state->a = state->a ^ state->c;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // XRA D
        case 0xaa:
            state->a = state->a ^ state->d;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // XRA E
        case 0xab:
            state->a = state->a ^ state->e;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // XRA H
        case 0xac:
            state->a = state->a ^ state->h;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // XRA L
        case 0xad:
            state->a = state->a ^ state->l;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // XRA M
        case 0xae:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->a = state->a ^ state->memory[target_addr];
                state->cc.z = (state->a == 0);
                state->cc.s = ((state->a & 0x80) != 0);
                state->cc.p = parity2(state->a, 8);
                state->cc.cy = 0;
                state->pc++;
            }
            break;

        // XRA A
        case 0xaf:
            state->a = state->a ^ state->a;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ORA B
        case 0xb0:
            state->a     = state->a | state->b;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ORA C
        case 0xb1:
            state->a     = state->a | state->c;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ORA D
        case 0xb2:
            state->a     = state->a | state->d;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ORA E
        case 0xb3:
            state->a     = state->a | state->e;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ORA H
        case 0xb4:
            state->a     = state->a | state->h;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ORA L
        case 0xb5:
            state->a     = state->a | state->l;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // ORA M
        case 0xb6:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                state->a     = state->a | state->memory[target_addr];
                state->cc.z  = (state->a == 0);
                state->cc.s  = ((state->a & 0x80) != 0);
                state->cc.p  = parity2(state->a, 8);
                state->cc.cy = 0;
                state->pc++;
            }
            break;

        // ORA A
        case 0xb7:
            state->a     = state->a | state->a;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->cc.cy = 0;
            state->pc++;
            break;

        // CMP B
        case 0xb8:
            {
                uint16_t res = state->a - state->b;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->cc.cy = (res > 0xff);
                state->pc++;
            }
            break;

        // CMP C
        case 0xb9:
            {
                uint16_t res = state->a - state->c;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->cc.cy = (res > 0xff);
                state->pc++;
            }
            break;

        // CMP D
        case 0xba:
            {
                uint16_t res = state->a - state->d;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->cc.cy = (res > 0xff);
                state->pc++;
            }
            break;

        // CMP E
        case 0xbb:
            {
                uint16_t res = state->a - state->e;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->cc.cy = (res > 0xff);
                state->pc++;
            }
            break;

        // CMP H
        case 0xbc:
            {
                uint16_t res = state->a - state->h;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->cc.cy = (res > 0xff);
                state->pc++;
            }
            break;

        // CMP L
        case 0xbd:
            {
                uint16_t res = state->a - state->l;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->cc.cy = (res > 0xff);
                state->pc++;
            }
            break;

        // CMP M
        case 0xbe:
            {
                uint16_t target_addr = (state->h << 8) | state->l;
                uint16_t res         = state->a - state->memory[target_addr];
                state->cc.z          = (res == 0);
                state->cc.s          = ((res & 0x80) != 0);
                state->cc.p          = parity2(res, 8);
                state->cc.cy         = (res > 0xff);
                state->pc++;
            }
            break;

        // CMP A
        case 0xbf:
            {
                uint16_t res = state->a - state->a;
                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->cc.cy = (res > 0xff);
                state->pc++;
            }
            break;

        // RNZ
        case 0xc0:
            if (state->cc.z == 0) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
                printf("Taken\n"); // debug
            }
            else {
                state->pc++;
                printf("Not taken\n"); // debug
            }
            break;

        // JNZ addr
        case 0xc2:
            if (state->cc.z == 0)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        // JMP addr
        case 0xc3:
            // Move third byte 8 bits to the right
            // bitwise or to add the second byte to the lower space
            state->pc = (opcode[2]) << 8 | opcode[1];
            break;

        // CNZ addr
        case 0xc4:
            {
                if (state->cc.z == 0) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;

        // ADI D8
        case 0xc6:
            {
                uint16_t res = (uint16_t) state->a + (uint16_t) opcode[1];

                state->cc.cy = (res > 0xff);
                state->cc.z  = ((res & 0xff) == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = parity2(res, 8);

                state->a     = (uint8_t) res;
                state->pc   += 2;
            }
            break;

       // RZ
       case 0xc8:
            if (state->cc.z == 1) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
                printf("Taken\n"); // debug

                // Debug
                print_state(state);
                exit(0);
            }
            else {
                state->pc++;
                printf("Not taken\n"); // debug
            }
            break;

        // RET
        case 0xc9:
            state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
            state->sp += 3;
            break;

        // JZ addr
        case 0xca:
            if (state->cc.z == 1)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        // CZ addr
        case 0xcc:
            {
                if (state->cc.z == 1) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;

        // ACI D8
        case 0xce:
            {
            uint16_t res = ((uint16_t) state->a + (uint16_t) opcode[1]) + state->cc.cy;
            state->cc.z  = ((res & 0xff) == 0);
            state->cc.s  = ((res & 0x80) != 0);
            state->cc.cy = (res > 0xff);
            state->cc.p  = parity2(res, 8);
            state->a     = (uint8_t) res;
            state->pc   += 2;
            }
            break;


        // CALL addr
        case 0xcd:
// Print to console. For testing
#ifdef FOR_CPUDIAG
            if (((opcode[2] << 8) | opcode[1]) == 5) {
                if (state->c == 9) {
                    uint16_t target_addr = (state->d << 8) | state->e;
                    char *str = (char*)&state->memory[target_addr + 3];
                    while (*str != '$')
                        printf("%c", *str++);
                    printf("\n");
                    print_state(state);
                    exit(1);
                } else if (state->c == 2) {
                    printf("Print char routine called\n");
                }
            } else if ((opcode[2] << 8 | opcode[1]) == 0) {
                exit(0);
            }
            else
#endif
            {
            // Push ret addr onto stack
            uint16_t ret_addr = state->pc + 2;
            // Push MSB first
            state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
            state->memory[state->sp-2] = (ret_addr & 0xff);
            // Stack grows towards lower addresses
            state->sp = state->sp - 2;
            // Transfer execution to subrutine
            state->pc = (opcode[2] << 8) | opcode[1];
            }
            break;

       // RNC
       case 0xd0:
            if (state->cc.cy == 0) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
                printf("Taken\n"); // debug
            }
            else {
                state->pc++;
                printf("Not taken\n"); // debug
            }
            break;

        // JNC addr
        case 0xd2:
            if (state->cc.cy == 0)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        // OUT D8
        // Write data from acumulator to output device D8
        case 0xd3:
            printf("Writing 0x%02x to output device %02x\n", state->a, opcode[1]);
            state->pc += 2;
            break;

        // CNC addr
        case 0xd4:
            {
                if (state->cc.cy == 0) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;

        // PUSH D
        // PUSH register pair DE
        case 0xd5:
            state->memory[state->sp-1] = state->d;
            state->memory[state->sp-2] = state->e;
            state->sp = state->sp - 2;
            state->pc++;
            break;

        // SUI D8
        case 0xd6:
            {
                uint8_t res  = state->a - opcode[1];
                state->cc.cy = (state->a < opcode[1]);
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->a     = res;
                state->pc   += 2;
            }
            break;

       // RC
       case 0xd8:
            if (state->cc.cy == 1) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
                printf("Taken\n"); // debug
            }
            else {
                state->pc++;
                printf("Not taken\n"); // debug
            }
            break;

        // JC
        case 0xda:
            if (state->cc.cy == 1)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        // CC addr
        case 0xdc:
            {
                if (state->cc.cy == 1) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;

        // SBI d8
        case 0xde:
            {
                uint8_t res  = state->a - (opcode[1] + state->cc.cy);
                state->cc.cy = (state->a < (opcode[1] + state->cc.cy));
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->a     = res;
                state->pc += 2;
            }
            break;

       // RPO
       case 0xe0:
            if (state->cc.p == 0) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
                printf("Taken\n"); // debug
            }
            else {
                state->pc++;
                printf("Not taken\n"); // debug
            }
            break;

       // JPO addr
       case 0xe2:
            if (state->cc.p == 0)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        // CPO addr
        case 0xe4:
            {
                if (state->cc.p == 0) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;


       // ANI D8
       case 0xe6:
            state->a     = state->a & opcode[1];
            state->cc.cy = 0;
            state->cc.z  = (state->a == 0);
            state->cc.s  = ((state->a & 0x80) != 0);
            state->cc.p  = parity2(state->a, 8);
            state->pc   += 2;
            break;

       // RPE
       case 0xe8:
            if (state->cc.p == 1) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
            }
            else {
                state->pc++;
            }
            break;

       // JPE addr
       case 0xea:
            if (state->cc.p == 1)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        //CPE addr
        case 0xec:
            {
                if (state->cc.p == 1) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;

       // XCHG
       case 0xeb:
            {
                uint8_t tmp1 = state->d;
                uint8_t tmp2 = state->e;
                state->d     = state->h;
                state->e     = state->l;
                state->h     = tmp1;
                state->l     = tmp2;
                state->pc++;
            }
            break;

       // XRI D8
       case 0xee:
            {
                uint8_t res  = state->a ^ opcode[1];
                state->cc.cy = 0;
                state->cc.z  = (res == 0);
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.p  = (parity2(res, 8));
                state->a     = res;
                state->pc += 2;
            }
            break;

       // RP
       case 0xf0:
            if (state->cc.s == 0) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
                printf("Taken\n"); // debug
            }
            else {
                state->pc++;
                printf("Not taken\n"); // debug
            }
            break;

       // JP addr
       case 0xf2:
            if (state->cc.s == 0)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        // CP addr
        case 0xf4:
            {
                if (state->cc.s == 0) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;

       // ORI D8
       case 0xf6:
            {
            uint8_t res  = state->a | opcode[1];
            state->cc.cy = 0;
            state->cc.z  = (res == 0);
            state->cc.s  = ((res & 0x80) != 0);
            state->cc.p  = (parity2(res, 8));
            state->a     = res;
            state->pc += 2;
            }
            break;

       // RM
       case 0xf8:
            if (state->cc.s == 1) {
                state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
                state->sp += 3;
                printf("Taken\n"); // debug
            }
            else {
                state->pc++;
                printf("Not taken\n"); // debug
            }
            break;

       // JM addr
       case 0xfa:
            if (state->cc.s == 1)
                state->pc = (opcode[2] << 8) | opcode[1];
            else
                state->pc += 3;
            break;

        // EI
        // Enable interrupts
        case 0xfb:
            state->int_enable = 1;
            state->pc++;
            break;

        // CM addr
        case 0xfc:
            {
                if (state->cc.s == 1) {
                    // Push ret addr onto stack
                    uint16_t ret_addr = state->pc + 2;
                    // Push MSB first
                    state->memory[state->sp-1] = (ret_addr >> 8) & 0xff;
                    state->memory[state->sp-2] = (ret_addr & 0xff);
                    // Stack grows towards lower addresses
                    state->sp = state->sp - 2;
                    // Transfer execution to subrutine
                    state->pc = (opcode[2] << 8) | opcode[1];
                    }
                else {
                    state->pc +=3;
                }
            }
            break;

        // CPI D8
        case 0xfe:
            {
                uint16_t res = state->a - opcode[1];
                state->cc.z  = (state->a == opcode[1]); // 1 if accumulator and immidiate are equal
                state->cc.s  = ((res & 0x80) != 0);
                state->cc.cy = (res > 0xff);
                state->cc.p  = parity2(res, 8);
                state->pc   += 2;
            }
            break;

        default:
            unimp(state);
            exit(2);
    }
}

static void
dump_memory(State *state, size_t mem_start_int, size_t amount_int)
{
    printf("Memory 0x%04lx - 0x%04lx\n", mem_start_int, mem_start_int + amount_int);

    for (size_t i = 0; i < amount_int; i++) {
        printf("\n0x%04lx  ", mem_start_int + i);
        printf("%02x ", state->memory[mem_start_int + i]);
    }
    printf("\n\n");
}

int
main(int argc, char** argv)
{
    State *state = calloc(sizeof state, 1);

    // Read rom with an offset of 0x100 bytes
    state->memory_size = read_rom(&state->memory, "./invaders/test.com", 0x100);

    // For testing
    // Insert a JMP 0x100 as first instruction
    state->memory[0] = 0xc3;
    state->memory[1] = 0;
    state->memory[2] = 0x01;

    //// Fix stackt pointer from 0x6ad to 0x7ad
    state->memory[368] = 0x7;

    // Skip DAA test
    state->memory[0x59c] = 0xc3; // JMP
    state->memory[0x59d] = 0xc2;
    state->memory[0x59e] = 0x05;

    // Initial information
    printf("*******************************\n");
    printf("Press d to dump memory, q to quit\n");
    printf("Memory allocated at: %p\n", state->memory);
    printf("Memory size: %ld\n", state->memory_size);
    printf("********************************\n");

    bool print_memory = false;
    for (;;) {
        if (argc != 1) {
            print_state(state);

            // Ask if user wants to dump memory
            uint16_t mem_start_int;
            uint16_t amount_int;
            size_t n = 64;
            char* junk = calloc(n, sizeof(char));

            int dump = fgetc(stdin);

            if (dump == 0x64) {

                // Ask for the address range to dump
                printf("Start print mem at 0x");
                char* mem_start = calloc(n, sizeof(char));
                getline(&junk, &n, stdin); // Clear stdin
                getline(&mem_start, &n, stdin);

                printf("Amount: ");
                char* amount = calloc(n, sizeof(char));
                getline(&amount, &n, stdin);

                mem_start_int = strtol(mem_start, NULL, 16);
                amount_int    = strtol(amount, NULL, 10);

                free(mem_start);
                free(amount);

                dump_memory(state, mem_start_int, amount_int);

                // Ask if user want to keep printing the same memory each intsruction
                printf("Keep printing? [y/n]: ");
                int keep_printing_answer = fgetc(stdin);

                if (keep_printing_answer == 0x79) {
                    print_memory = true;
                }
                else {
                    print_memory = false;
                }
                getline(&junk, &n, stdin); // Clear stdin
            }
            else if (dump == 0x71) {
                free(state->memory);
                free(state);
                exit(0);
            }

            if (print_memory) {
                dump_memory(state, mem_start_int, amount_int);
            }
        }
        emulate(state);
    }
    free(state->memory);

    return 0;
}
