#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float r32;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define INPUT_DPAD_UP 1
#define INPUT_DPAD_DOWN 2
#define INPUT_DPAD_LEFT 4
#define INPUT_DPAD_RIGHT 8
#define INPUT_BUTTON_PRIMARY 1
#define INPUT_BUTTON_SECONDARY 2
#define INPUT_BUTTON_MENU 4
#define INPUT_BUTTON_PAUSE 8

#ifndef max
#define max(a,b) ((a)>(b)?(b):(a))
#endif

#define kill_texture(t){if(t){SDL_DestroyTexture(t);t=0;}}

struct input_t {
  // b0 - up
  // b1 - down
  // b2 - left
  // b3 - right
  u8 dpad;

  // b0 - primary action
  // b1 - secondary action
  // b2 - menu
  // b3 - pause
  u8 buttons;
};

struct sprite_t {
  u32 id;

  r32 world_x;
  r32 world_y;

  u16 world_xtile;
  u16 world_ytile;

  u8 hitbox_x;
  u8 hitbox_y;
  u8 hitbox_width;
  u8 hitbox_height;

  u8 animation_started; // 0 stopped, 1 - started, 2 - paused
  u8 animation_loop; // 0 - play once and stop, 1 - loop to start at end

  /*
    an animation is defined by
      frame_start
      frame_count
      frame_timeout

      animations is an array of 3 * animation_count unsigned char number values
      every 3 values defines one animation
  */
  u8 animation_index;
  u8 animation_count;
  u8* animations;

  u8 animation_frame_index;
  u8 animation_frame_start;
  u8 animation_frame_count;
  u8 animation_frame_time;
  u8 animation_frame_timeout;

  u8 render_alpha;
  SDL_Rect render_src_rect;
  SDL_Rect render_dest_rect;
  SDL_Texture* render_texture;
  u16 render_texture_width;
  u16 render_texture_height;
};

struct tilemap_camera_t {
  r32 x;
  r32 y;
  u16 width;
  u16 height;
};

struct tilemap_context_t {
  SDL_Renderer* renderer;
  SDL_Texture* tileset;
  SDL_Rect* tile_src_rect;
  SDL_Rect* tile_dest_rect;
  u8 tileset_tile_count;
  u8* map_data;
  u8 map_width;
  u8 map_height;
  u16 map_size;
  u8 tile_width;
  u8 tile_height;
};


typedef struct sprite_t spr;
extern spr* sprite_create(SDL_Texture* texture, u8 width, u8 height);
extern void sprite_destroy(spr** sprite);
extern void sprite_animation_loop(spr* sprite, u8 loop);
extern void sprite_add_animation(spr* sprite, u8 start, u8 count, u8 timeout);
extern void sprite_select_animation(spr* sprite, u8 index);
extern void sprite_start_animation(spr* sprite);
extern void sprite_pause_animation(spr* sprite);
extern void sprite_resume_animation(spr* sprite);
extern void sprite_stop_animation(spr* sprite);
extern void sprite_update_animation(spr* sprite);
extern void sprite_render(spr* sprite, SDL_Renderer* renderer);
extern void sprite_debug_animationlist (spr* sprite);

extern void input_update_dpad_state(struct input_t* input, SDL_Scancode code, u8 unset);
extern void input_update_button_state(struct input_t* input, SDL_Scancode code, u8 unset);
extern void input_update_state(struct input_t* input, SDL_Event* sdl_event);
extern void input_debug_state(struct input_t* input, SDL_Renderer* renderer);

extern r32 get_epoch_time();
extern char* get_resource_path(char* filename);
extern SDL_Texture* load_texture(SDL_Renderer* renderer, char* filename);

extern void tilemap_render_bg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera);
extern void tilemap_render_mg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera);
extern void tilemap_render_ob_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera);
extern void tilemap_render_fg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera);
extern void tilemap_render_xx_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera);
extern void tilemap_render(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera);

#endif
