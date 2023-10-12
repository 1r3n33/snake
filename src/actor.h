#ifndef ACTOR_H
#define ACTOR_H

// Clear actor data
void actor_reset(uint8_t id);

// Set sprite range (number of sprites used by the actor)
// Max id count is ACTOR_MAX_TILE_COUNT
void actor_set_sprite_range(uint8_t id, uint8_t first, uint8_t count);

// Set actor reference position.
// Applied to OAM mirror by update function.
void actor_set_xy(uint8_t id, uint16_t x, uint16_t y);

// Set xy offset for all actor's sprites.
// sprites[i].xy = actor.xy + offset[i*2]
// Applied to OAM mirror by update function.
// NOTE: This does not modify actor's bounding box.
void actor_set_xy_offsets(uint8_t id, int8_t *offsets);

// Set actor's bounding box.
// Used to compute left, right, top, bottom.
// Distances from the reference position.
void actor_set_bounding_box(uint8_t id, int8_t left, int8_t right, int8_t top, int8_t bottom);

// Set tile ids for all actor's sprites.
// Applied immediately to OAM mirror.
void actor_set_tile_ids(uint8_t id, uint8_t *ids);

// Set tile props for all actor's sprites.
// Applied immediately to OAM mirror.
void actor_set_tile_props(uint8_t id, uint8_t *props);

void actor_update(uint8_t id);
void actor_enable(uint8_t id);
void actor_disable(uint8_t id);

#endif // ACTOR_H
