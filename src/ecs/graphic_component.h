#ifndef ECS_GRAPHIC_COMPONENT_H
#define ECS_GRAPHIC_COMPONENT_H

#include <gb/gb.h>

typedef struct GraphicComponent
{
    uint8_t enabled;
    uint16_t x;         // x pixel coord
    uint16_t y;         // y pixel coord
    uint8_t timer;      //
    uint8_t id;         // sprite id
    uint8_t count;      // sprite count
    int8_t *xy_offsets; // xyxyxy... TODO: replace by struct { x,y }
    int8_t bb[4];       // bounding box (lrtb)
} GraphicComponent;

#define GRAPHIC_COMPONENT_MAX_COUNT 8U

extern GraphicComponent gfx_components[GRAPHIC_COMPONENT_MAX_COUNT];

inline void gfx_enable(GraphicComponent *gfx)
{
    gfx->enabled = 1U;
}

inline void gfx_disable(GraphicComponent *gfx)
{
    gfx->enabled = 0U;

    uint8_t i = gfx->id;
    uint8_t c = gfx->count;
    while (c--)
    {
        move_sprite(i++, 0U, 0U);
    }
}

inline void gfx_set_sprite_range(GraphicComponent *gfx, uint8_t first, uint8_t count)
{
    gfx->id = first;
    gfx->count = count;
}

inline void gfx_set_xy(GraphicComponent *gfx, uint16_t x, uint16_t y)
{
    gfx->x = x;
    gfx->y = y;
}

inline void gfx_move(GraphicComponent *gfx, int8_t dx, int8_t dy)
{
    gfx->x += dx;
    gfx->y += dy;
}

inline void gfx_set_xy_offsets(GraphicComponent *gfx, int8_t *offsets)
{
    gfx->xy_offsets = offsets;
}

inline void gfx_set_bounding_box(GraphicComponent *gfx, int8_t left, int8_t right, int8_t top, int8_t bottom)
{
    gfx->bb[0U] = left;
    gfx->bb[1U] = right;
    gfx->bb[2U] = top;
    gfx->bb[3U] = bottom;
}

inline void gfx_set_tile_ids(GraphicComponent *gfx, const uint8_t *ids)
{
    uint8_t i = gfx->id;
    uint8_t c = gfx->count;
    while (c--)
    {
        set_sprite_tile(i++, *ids++);
    }
}

inline void gfx_set_tile_props(GraphicComponent *gfx, const uint8_t *props)
{
    uint8_t i = gfx->id;
    uint8_t c = gfx->count;
    while (c--)
    {
        set_sprite_prop(i++, *props++);
    }
}

#endif // ECS_GRAPHIC_COMPONENT_H
