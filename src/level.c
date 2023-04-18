#include <gb/gb.h>
#include "level.h"

#include "titlescreen/titlescreen.h"
#include "level1_garden/garden.h"
#include "level2_clouds/clouds.h"

Level titlescreen;
Level garden;
Level clouds;

Level *level_init()
{
    titlescreen.rom_id = 1;
    titlescreen.init = titlescreen_init;
    titlescreen.loop = titlescreen_loop;
    titlescreen.prev = &titlescreen;
    titlescreen.next = &garden;

    garden.rom_id = 2;
    garden.init = garden_init;
    garden.loop = garden_loop;
    garden.prev = &titlescreen;
    garden.next = &clouds;

    clouds.rom_id = 3;
    clouds.init = clouds_init;
    clouds.loop = clouds_loop;
    clouds.prev = &garden;
    clouds.next = &titlescreen;

    return &titlescreen;
}
