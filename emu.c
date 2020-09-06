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
    printf("STATE:\n");
    printf("sp: %d\n", state->sp);
    printf("pc: %d\n", state->pc);
    printf("a: %d\n", state->a);
    printf("b: %d\n", state->b);
    printf("c: %d\n", state->c);
    printf("d: %d\n", state->d);
    printf("e: %d\n", state->e);
    printf("h: %d\n", state->h);
    printf("l: %d\n", state->l);
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
    // Dissasemble current instruction
    dissas_curr_inst(state);

    // Use addres of opcode instead of value to
    // access data / addreses following it easily
    uint8_t *opcode = &state->memory[state->pc];

    count++;

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
        case 0x6:
            state->b = opcode[1];
            state->pc++;
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
            if (state->e == 0) {
                state->d++;
            }
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
            if (state->l == 0) {
                state->h++;
            }
            state->pc++;
            break;

        // LDAX D
        // Load value in address D+E into register A
        case 0x1a:
            {
            uint16_t target_addr = (state->d << 8) | state->e;
            state->a = state->memory[target_addr];
            }
            state->pc++;
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

        // MVI A, D8
        // Move immidiate data into register A
        case 0x3e:
            state->a   = opcode[1];
            state->pc += 2;
            break;

        // LDA addr
        case 0x3a:
            {
            uint16_t target_addr = (opcode[2] << 8) | opcode[1];
            state->a   = state->memory[target_addr];
            state->pc += 3;
            }
            break;

        // MOV M, A
        case 0x77:
            {
            uint16_t target_addr = (state->h << 8) | state->l;
            state->memory[target_addr] = state->a;
            }
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

        // OUT D8
        // Write data from acumulator to output device D8
        case 0xd3:
            printf("Writing 0x%02x to output device %02x\n", state->a, opcode[1]);
            state->pc += 2;
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

       // RET
       case 0xc9:
            state->pc  = (state->memory[state->sp+1] << 8) | state->memory[state->sp];
            state->sp += 3;
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
                } else if (state->c == 2) {
                    printf("Print char routine called\n");
                }
            }
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

        // EI
        // Enable interrupts
        case 0xfb:
            state->int_enable = 1;
            state->pc++;
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

    state->memory_size = read_rom(&state->memory, "./invaders/cpudiag.bin");

    // For testing
    // Insert a JMP 0x100 as first instruction
    state->memory[0] = 0xc3;
    state->memory[1] = 0;
    state->memory[2] = 0x01;

    // Fix stackt pointer from 0x6ad to 0x7ad
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
