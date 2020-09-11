#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "intel8080.h"
#include "dissas.h"

#define FOR_CPUDIAG

uint64_t count = 0;

static void
print_state(State *state)
{
    printf("REGISTERS:\n");
    printf("sp: %d\n", state->sp);
    printf("pc: %d Next inst: ", state->pc);
    dissas_curr_inst(state);
    printf("a: %d\n", state->a);
    printf("b: %d\n", state->b);
    printf("c: %d\n", state->c);
    printf("d: %d\n", state->d);
    printf("e: %d\n", state->e);
    printf("h: %d\n", state->h);
    printf("l: %d\n", state->l);
    printf("FLAGS:\n");
    printf("cy: %d\n", state->cc.cy);
    printf("p: %d\n", state->cc.p);
    printf("s: %d\n", state->cc.s);
    printf("z: %d\n", state->cc.z);
    printf("ac: %d\n", state->cc.ac);
    printf("pad: %d\n", state->cc.pad);
    printf("mem: %p\n", state->memory);
    printf("mem sz: %ld\n", state->memory_size);
    printf("int enable: %d\n", state->int_enable);
}

static void
unimp(State *state)
{
    printf("Unimplemented opcode: 0x%02x\n", state->memory[state->pc]);
    print_state(state);
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
    printf("%ld\t", count);
    count++;

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

        // INX B
        case 0x03:
            state->c++;
            if (state->c == 0) {
                state->b++;
            }
            state->pc++;
            break;

        // DCR b
        case 0x05:
            {
                uint8_t res = state->b - 1;

                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->b = res;
                state->pc++;
            }
            break;

        // MVI B, D8
        case 0x06:
            state->b = opcode[1];
            state->pc++;
            break;

        // DCR C
        case 0x0d:
            {
                uint8_t res = state->c - 1;

                state->cc.z = (res == 0);
                state->cc.s = ((res & 0x80) != 0);
                state->cc.p = parity2(res, 8);
                state->b = res;
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

        // INX D
        // Increment register pair D + E
        case 0x13:
            state->e++;
            // If e overflows into D, inc D
            if (state->e == 0)
                state->d++;
            state->pc++;
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

        // INX H
        case 0x23:
            state->l++;
            // If e overflows into D, inc D
            if (state->l == 0)
                state->h++;
            state->pc++;
            break;

        // LDAX D
        // Load value in address D+E into register A
        case 0x1a:
            {
                uint16_t target_addr = (state->d << 8) | state->e;
                state->a = state->memory[target_addr];
                state->pc++;
            }
            break;

        // LXI H, D16
        case 0x21:
            state->h   = opcode[2];
            state->l   = opcode[1];
            state->pc += 3;
            break;

        // LXI SP, D16
        case 0x31:
            state->sp  = (opcode[2] << 8) | opcode[1];
            state->pc += 3;
            break;

        // STA adr
        // Replace value of accumulater at direct address
        case 0x32:
            {
                uint16_t target_addr = (opcode[2] << 8) | opcode[1];
                state->memory[target_addr] = state->a;
                state->pc += 3;
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

        // INR A
        case 0x3c:
            state->a++;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            state->pc++;

        // MVI A, D8
        // Move immidiate data into register A
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

        // ANA A
        case 0xa7:
            state->a = state->a & state->a;

            state->cc.cy = 0;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);
            
            state->pc++;
            break;

        // XRA A
        case 0xaf:
            state->a = state->a ^ state->a;

            state->cc.cy = 0;
            state->cc.z = (state->a == 0);
            state->cc.s = ((state->a & 0x80) != 0);
            state->cc.p = parity2(state->a, 8);

            state->pc++;
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

int
main()
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

    for (;;) {
        emulate(state);
    }

    free(state->memory);

    return 0;
}
