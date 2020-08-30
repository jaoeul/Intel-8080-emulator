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

static void
emulate(State *state)
{
    // Use addres of opcode instead of value to
    // access data / addreses following it easily
    uint8_t *opcode = &state->memory[state->pc];

    // Emulate forever
    for (;;) {
        opcode = &state->memory[state->pc];

        switch (*opcode) {

            // NOP
            case 0x00:
                state->pc++;
                break;

            case 0x01:
                state->c  = opcode[1];
                state->b  = opcode[2];
                state->pc += 3;
                break;

            default:
                unimp(state);
                exit(1);
        }
    }
}

int 
main()
{
    State *state = calloc(sizeof state, 1);
    state->memory_size = read_rom(&state->memory, "./invaders/invaders.full");

    emulate(state);

    return 0;
}
