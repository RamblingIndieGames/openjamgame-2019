#include "game.h"
#include "scenes.h"
#include "tilemap.h"
#include "texture_cache.h"
#include "sprite.h"

struct play_data_t {
  struct sprite_t* player_sprite;
};
struct play_data_t play_data;

void init_play_scene() {
  printf("init_play\n");

  create_tilemap_from_array(60, 45, (int[]){
    // bg
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,2,2,2,2,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,
    1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    // mg
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,5,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    // fg
    0,0,0,0,0,0,6,6,6,0,4,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,6,6,6,6,4,6,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,0,4,6,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,4,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,4,6,6,6,0,0,0,0,0,0,0,4,6,6,6,6,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,4,4,4,0,0,0,0,0,0,0,
    0,0,0,0,0,0,4,4,4,4,4,0,6,6,6,6,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,
    0,0,0,0,0,0,4,6,6,6,6,0,0,6,6,6,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,4,4,0,0,0,0,
    0,0,0,0,0,0,4,6,6,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,6,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,4,0,0,0,0,
    0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,
    0,0,0,0,0,6,6,6,6,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,
    0,0,0,0,0,6,6,6,6,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,6,6,6,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,6,6,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,4,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,4,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,4,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,6,6,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,
    0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    // collision
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,
    0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  });

  game_ptr->tileset_texture = load_texture("../../data/testmaptiles.png");
  game_ptr->tileset_columns = 7;

  cam_x = 0;
  cam_y = 0;

  add_texture_to_cache(100, "../../data/testplayersprite.png");

  play_data.player_sprite = sprite_create(100, 32, 64);

  sprite_add_animation(play_data.player_sprite, 0, 4, 12);
  // sprite_add_animation(play_data.player_sprite, 0, 4, 12);
  // sprite_add_animation(play_data.player_sprite, 0, 4, 12);
  // sprite_add_animation(play_data.player_sprite, 0, 4, 12);

  sprite_select_animation(play_data.player_sprite, 0);

  sprite_start_animation(play_data.player_sprite);

}

void destroy_play_scene() {
  sprite_destroy(&play_data.player_sprite);
  destroy_texture_cache();
  destroy_tilemap();
  kill_texture(game_ptr->tileset_texture);
}

void enter_play_scene() {
  printf("enter_play\n");
}

void exit_play_scene() {
  printf("exit_play\n");
}

void update_play_scene(float dt) {
  // printf("update_play\n");

  if (game_ptr->input.right) {
    cam_x += 4;
  } else if (game_ptr->input.left) {
    cam_x -= 4;
  }

  if (game_ptr->input.up) {
    cam_y -= 4;
  } else if (game_ptr->input.down) {
    cam_y += 4;
  }

  sprite_update_animation(play_data.player_sprite);
}

void render_play_scene() {
  // printf("render_play\n");

  render_tilemap(cam_x, cam_y);

  sprite_render(play_data.player_sprite);
}
