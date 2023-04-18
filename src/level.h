#ifndef LEVEL_H
#define LEVEL_H

#include <gb/gb.h>

typedef struct Level
{
    uint8_t rom_id;
    void (*init)(void);
    int8_t (*loop)(void);
    struct Level *prev;
    struct Level *next;
} Level;

Level *level_init();

#endif // LEVEL_H
