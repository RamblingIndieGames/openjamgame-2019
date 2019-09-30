#include "game.h"
#include "scenes.h"
#include "tilemap.h"
#include "texture_cache.h"
#include "sprite.h"

#include "testmap.h"

// animation frame start indices
#define PLAYER_ATK_EAST 0
#define PLAYER_ATK_NORTH 1
#define PLAYER_ATK_SOUTH 2
#define PLAYER_ATK_WEST 3

#define PLAYER_WALK_EAST 4
#define PLAYER_WALK_NORTH 8
#define PLAYER_WALK_SOUTH 12
#define PLAYER_WALK_WEST 16

// animation index ids
#define ID_PLAYER_ATK_EAST 0
#define ID_PLAYER_ATK_NORTH 1
#define ID_PLAYER_ATK_SOUTH 2
#define ID_PLAYER_ATK_WEST 3
#define ID_PLAYER_WALK_EAST 4
#define ID_PLAYER_WALK_NORTH 5
#define ID_PLAYER_WALK_SOUTH 6
#define ID_PLAYER_WALK_WEST 7

#define ID_PLAYER_IDLE_EAST 8
#define ID_PLAYER_IDLE_NORTH 9
#define ID_PLAYER_IDLE_SOUTH 10
#define ID_PLAYER_IDLE_WEST 11

#define ID_SLIME_IDLE 0
#define ID_SLIME_HURT 1

// direction facing
#define DIR_EAST 0
#define DIR_NORTH 1
#define DIR_SOUTH 2
#define DIR_WEST 3

// player state
#define ST_IDLE 0
#define ST_WALKING 1
#define ST_ATTACKING 2

// player flags
#define FLG_WALK_ENABLED 1

// texture cache ids
#define ID_PLAYER_TEXTURE 100

#define ID_SLIME_TEXTURE 200

struct play_data_t {
  struct sprite_t* player_sprite;
  SDL_Rect sword_hitbox;
  int sword_active;
  struct sprite_t* slime_sprite;
};
struct play_data_t play_data;

void update_sword_hitbox() {
  struct sprite_t* player = play_data.player_sprite;
  SDL_Rect* sword = &play_data.sword_hitbox;

  // E N S W
  static int shift_x[] = { 32, 10, 0, -32 };
  static int shift_y[] = { -10, -24, 32, -10 };

  int player_center_x = player->world_x + (player->render_frame_width / 2);
  int player_center_y = player->world_y + (player->render_frame_height / 2);

  sword->x = player_center_x - sword->w / 2;
  sword->y = player_center_y - sword->h / 2;

  sword->x += shift_x[player->direction];
  sword->y += shift_y[player->direction];
}

void render_sword_hitbox() {
  if (!play_data.sword_active) {
    return;
  }

  static SDL_Rect dst;

  dst.x = play_data.sword_hitbox.x - cam_x;
  dst.y = play_data.sword_hitbox.y - cam_y;
  dst.w = play_data.sword_hitbox.w;
  dst.h = play_data.sword_hitbox.h;

  SDL_SetRenderDrawColor(main_renderer_ptr, 0, 128, 255, 0xF0);
  SDL_RenderDrawRect(main_renderer_ptr, &dst);
}

void init_play_scene() {
  printf("init_play\n");

  create_tilemap_from_array(testmap_width, testmap_height, testmap_arr);

  game_ptr->tileset_texture = load_texture("../../data/testmaptiles.png");
  game_ptr->tileset_columns = 7;

  cam_x = 0;
  cam_y = 0;

  add_texture_to_cache(ID_SLIME_TEXTURE, "../../data/testslimesprite.png");

  play_data.slime_sprite = sprite_create(ID_SLIME_TEXTURE, 32, 32);
  play_data.slime_sprite->hitbox_width = tile_width + 2;
  play_data.slime_sprite->hitbox_height = tile_height + 2;
  play_data.slime_sprite->hitbox_x = -1;
  play_data.slime_sprite->hitbox_y = -1;

  play_data.slime_sprite->world_x = tile_width * 34;
  play_data.slime_sprite->world_y = tile_height * 10;

  sprite_add_animation(play_data.slime_sprite, 0, 4, 24);
  sprite_add_animation(play_data.slime_sprite, 3, 2, 2);
  sprite_select_animation(play_data.slime_sprite, ID_SLIME_IDLE);
  sprite_start_animation(play_data.slime_sprite);

  add_texture_to_cache(ID_PLAYER_TEXTURE, "../../data/playersheet.png");

  play_data.player_sprite = sprite_create(ID_PLAYER_TEXTURE, 74, 85);

  play_data.player_sprite->hitbox_width = tile_width * 0.8;
  play_data.player_sprite->hitbox_height = tile_height * 0.8;
  play_data.player_sprite->hitbox_x = (play_data.player_sprite->render_frame_width - play_data.player_sprite->hitbox_width) / 2;
  play_data.player_sprite->hitbox_y = tile_height * 1.2;

  play_data.player_sprite->world_x = tile_width * 6;
  play_data.player_sprite->world_y = tile_height * 6;

  sprite_add_animation(play_data.player_sprite, PLAYER_ATK_EAST, 1, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_ATK_NORTH, 1, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_ATK_SOUTH, 1, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_ATK_WEST, 1, 12);

  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_EAST, 4, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_NORTH, 4, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_SOUTH, 4, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_WEST, 4, 12);

  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_EAST, 1, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_NORTH, 1, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_SOUTH, 1, 12);
  sprite_add_animation(play_data.player_sprite, PLAYER_WALK_WEST, 1, 12);

  sprite_select_animation(play_data.player_sprite, ID_PLAYER_WALK_SOUTH);
  play_data.player_sprite->direction = DIR_SOUTH;
  play_data.player_sprite->flags = FLG_WALK_ENABLED;

  play_data.sword_hitbox.w = tile_width;
  play_data.sword_hitbox.h = tile_height;

  sprite_start_animation(play_data.player_sprite);
}

void destroy_play_scene() {
  sprite_destroy(&play_data.slime_sprite);
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
  unsigned char sprite_mcc = 0;
  struct input_t* input = &game_ptr->input;
  struct sprite_t* player = play_data.player_sprite;

  int walk_enabled = player->flags & FLG_WALK_ENABLED;

  if (walk_enabled) {
    if (input->up) {
      if (!input->down) {
        sprite_mcc |= 2;
        player->state = ST_WALKING;
      }
      if (!input->left && !input->right) {
        player->direction = DIR_NORTH;
      }
    }

    if (input->down) {
      if (!input->up) {
        sprite_mcc |= 4;
        player->state = ST_WALKING;
      }
      if (!input->left && !input->right) {
        player->direction = DIR_SOUTH;
      }
    }

    if (input->left) {
      if (!input->right) {
        sprite_mcc |= 8;
        player->state = ST_WALKING;
      }
      if (!input->down && !input->up) {
        player->direction = DIR_WEST;
      }
    }

    if (input->right) {
      if (!input->left) {
        sprite_mcc |= 16;
        player->state = ST_WALKING;
      }
      if (!input->down && !input->up) {
        player->direction = DIR_EAST;
      }
    }

    if (!input->up && !input->down && !input->left && !input->right) {
      player->state = ST_IDLE;
    }
  }

  // attack logic
  if (input->primary_action && player->state != ST_ATTACKING) {
    player->flags ^= FLG_WALK_ENABLED;
    player->state = ST_ATTACKING;
    sprite_select_animation(player, ID_PLAYER_ATK_EAST + player->direction);
  }

  if (player->animation_index == ID_PLAYER_ATK_EAST + player->direction) {
    if (player->animation_frame_time == player->animation_frame_timeout - 1) {
      // create sword collider
      printf("create sword\n");
      play_data.sword_active = 1;
      // enable walk
      player->flags |= FLG_WALK_ENABLED;
      player->state = ST_IDLE;
    } else if (!player->animation_frame_time) {
      // play attack sfx
    }
  }

  if (play_data.sword_active && walk_enabled) {
    play_data.sword_active = 0;
    printf("sword inactive\n");
  }

  if (walk_enabled) {
    float speed = tile_width * 8;
    float motion_x = sprite_mt[sprite_mm[sprite_mcc]] * speed * dt;
    float motion_y = sprite_mt[sprite_mm[sprite_mcc + 1]] * speed * dt;

    float x_dist = fabs(motion_x);
    float y_dist = fabs(motion_y);

    #define sign(v)((v > 0) - (v < 0))
    for (int i = 0; i < x_dist; i++) {
      int step = sign(motion_x);
      if (!tilemap_collision(player, step, 0)) {
        player->world_x += step;
      }
    }
    for (int i = 0; i < y_dist; i++) {
      int step = sign(motion_y);
      if (!tilemap_collision(player, 0, step)) {
        player->world_y += step;
      }
    }
    #undef sign
  }

  lock_sprite_to_map(player);

  // update player animation
  if (player->state != ST_ATTACKING) {
    if (player->state == ST_WALKING) {
      sprite_select_animation(player, ID_PLAYER_WALK_EAST + player->direction);
    } else {
      sprite_select_animation(player, ID_PLAYER_IDLE_EAST + player->direction);
    }
  } else {
    sprite_select_animation(player, ID_PLAYER_ATK_EAST + player->direction);
  }
  sprite_update_animation(player);

  // make camera follow player
  int player_center_x = player->world_x + (player->render_frame_width / 2);
  int player_center_y = player->world_y + (player->render_frame_height / 2);
  lock_camera_to_pos(player_center_x, player_center_y);

  sprite_update_animation(play_data.slime_sprite);

  update_sword_hitbox();

  // check sword collisions
  // TODO: sprite list...

  // sprite_collision_against_sprite_in_list(spr* sprite, spr** list, int list_count)


  // (!((bottom <= tile_y) || (y >= tile_y + tile_height) || (x >= tile_x + tile_width) || (right <= tile_x)))

  struct sprite_t* list[] = { play_data.slime_sprite };
  struct sprite_t* hit = 0;
  if (play_data.sword_active) {
    int x,y,w,h;
    x = play_data.sword_hitbox.x - cam_x;
    y = play_data.sword_hitbox.y - cam_y;
    w = play_data.sword_hitbox.w;
    h = play_data.sword_hitbox.h;
    hit = rect_collision_against_sprite_in_list(x, y, w, h, list, 1);
    if (hit && hit->state != 1) {
      sprite_select_animation(hit, ID_SLIME_HURT);
      hit->state = 1;
      hit->hp -= 25;
      if (hit->hp <= 0) {
        hit->state = 0xdead;
        hit->render_alpha = 0;
      }
    }
  }
  hit = 0;
  hit = sprite_collision_against_sprite_in_list(player, list, 1);
  if (hit && hit->state != 1) {
    sprite_select_animation(hit, ID_SLIME_HURT);
    hit->state = 1;
  }

  // enemy hurt
  if (play_data.slime_sprite->state == 1) {
    play_data.slime_sprite->timer++;
    if (play_data.slime_sprite->timer == 10) {
      play_data.slime_sprite->timer = 0;
      sprite_select_animation(play_data.slime_sprite, ID_SLIME_IDLE);
      play_data.slime_sprite->state = 0;
    }
  }
}

void render_play_scene() {
  // printf("render_play\n");

  render_tilemap_bg();
  render_tilemap_mg();

  spr* sprite_list[] = {
    play_data.player_sprite,
    play_data.slime_sprite
  };
  int sprite_list_count = 2;
  sort_sprites(sprite_list, sprite_list_count);

  for (int i = 0; i < sprite_list_count; i++) {
    sprite_render(sprite_list[i]);
  }

  // sprite_render(play_data.player_sprite);

  // sprite_render(play_data.slime_sprite);
  sprite_render_hitbox(play_data.slime_sprite);
  sprite_render_hitbox(play_data.player_sprite);
  render_tilemap_fg();
  render_tilemap_xx();

  render_sword_hitbox();
}
