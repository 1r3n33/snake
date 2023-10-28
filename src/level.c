#include <gb/gb.h>
#include "level.h"
#include "game.h"
#include "titlescreen/titlescreen.h"
#include "level1_garden/garden.h"
#include "level2_clouds/clouds.h"
#include "level3_underground/underground.h"

Level titlescreen;
Level garden;
Level clouds;
Level underground;

Level *level_init()
{
    titlescreen.rom_id = 1;
    titlescreen.init = titlescreen_init;
    titlescreen.loop = titlescreen_loop;
    titlescreen.prev = &titlescreen;
    titlescreen.next = &garden;

    garden.rom_id = 2;
    garden.init = garden_init;
    garden.loop = game_loop;
    garden.prev = &titlescreen;
    garden.next = &underground;

    underground.rom_id = 4;
    underground.init = underground_init;
    underground.loop = game_loop;
    underground.prev = &garden;
    underground.next = &clouds;

    clouds.rom_id = 3;
    clouds.init = clouds_init;
    clouds.loop = game_loop;
    clouds.prev = &underground;
    clouds.next = &titlescreen;

#ifdef DEBUG
    return &underground;
#else
    return &titlescreen;
#endif
}
