#ifndef EMU_
#define EMU_

typedef struct ConditionCodes{
    uint8_t cy:   1;
    uint8_t pad1: 1;
    uint8_t p:    1;
    uint8_t pad2: 1;
    uint8_t ac:   1;
    uint8_t pad3: 1;
    uint8_t z:    1;
    uint8_t s:    1;

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

#endif // EMU_
