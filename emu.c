#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "dissas.h"

typedef struct ConditionCodes{
    uint8_t z:  1;
    uint8_t s:  1;
    uint8_t p:  1;
    uint8_t cy: 1;
    uint8_t ac: 1;
    uint8_t pad:1;
} ConditionCodes;

typedef struct State{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t sp;
    uint16_t pc;
    uint8_t *memory;
    size_t memory_size;
    struct ConditionCodes cc;
    uint8_t int_enable;
} State;

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

static int
parity(uint16_t number)
{
    int bits_n = 0;

    if ((number & 0x80) != 0) bits_n++;
    if ((number & 0x40) != 0) bits_n++;
    if ((number & 0x20) != 0) bits_n++;
    if ((number & 0x10) != 0) bits_n++;
    if ((number & 0x8)  != 0) bits_n++;
    if ((number & 0x4)  != 0) bits_n++;
    if ((number & 0x2)  != 0) bits_n++;
    if ((number & 0x1)  != 0) bits_n++;

    if (bits_n % 2 == 0)
        return 1;
    else
        return 0;
}

static void
emulate(State *state)
{
    // Use addres of opcode instead of value to
    // access data / addreses following it easily
    uint8_t *opcode = &state->memory[state->pc];

    opcode = &state->memory[state->pc];

    switch (*opcode) {

        // Variable to hold answer for arthmetic operations
        uint16_t answer;

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

        // DEC b
        case 0x05:
            answer = state->b - 1;

            /// Set the flags
            // if the answer anded wich 0xff is 0 than the zero flag will be set
            state->cc.z = ((answer & 0xff) == 0);
            // if the answer anded with 0x80 (128) is not 0 than the 7 bit is set
            state->cc.s = ((answer & 0x80) != 0);
            // if the answer is bigger than the landing size the carry flag will be set
            state->cc.cy = (answer > 0xff);
            // if the parity is even set the parity flag
            state->cc.p = parity(answer);

            // save the result into the accumulator
            state->a = answer;
            state->pc++;
            break;

        // LXI SP, D16
        case 0x31:
            state->sp = (opcode[2] << 8) | opcode[1];
            state->pc += 3;
            break;

        // JMP addr
        case 0xc3:
            // Move third byte 8 bits to the right
            // bitwise or to add the second byte to the lower space
            state->pc = (opcode[2]) << 8 | opcode[1];
            state->pc += 3;
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

    state->memory_size = read_rom(&state->memory, "./invaders/invaders.full");

    printf("%d\n", parity(15));

    for (;;) {
        emulate(state);
    }

    free(state->memory);

    return 0;
}
