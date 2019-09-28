#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "testmap.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float r32;

#define INPUT_DPAD_UP 1
#define INPUT_DPAD_DOWN 2
#define INPUT_DPAD_LEFT 4
#define INPUT_DPAD_RIGHT 8
#define INPUT_BUTTON_PRIMARY 1
#define INPUT_BUTTON_SECONDARY 2
#define INPUT_BUTTON_MENU 4
#define INPUT_BUTTON_PAUSE 8

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

struct input_t input_player_one;

void input_update_dpad_state(struct input_t* input, SDL_Scancode code, u8 unset) {
  if (unset) {
    if (code == SDL_SCANCODE_W || code == SDL_SCANCODE_UP) {
      input->dpad ^= INPUT_DPAD_UP;
    } else if (code == SDL_SCANCODE_S || code == SDL_SCANCODE_DOWN) {
      input->dpad ^= INPUT_DPAD_DOWN;
    } else if (code == SDL_SCANCODE_A || code == SDL_SCANCODE_LEFT) {
      input->dpad ^= INPUT_DPAD_LEFT;
    } else if (code == SDL_SCANCODE_D || code == SDL_SCANCODE_RIGHT) {
      input->dpad ^= INPUT_DPAD_RIGHT;
    }
  } else {
    if (code == SDL_SCANCODE_W || code == SDL_SCANCODE_UP) {
      input->dpad |= INPUT_DPAD_UP;
    } else if (code == SDL_SCANCODE_S || code == SDL_SCANCODE_DOWN) {
      input->dpad |= INPUT_DPAD_DOWN;
    } else if (code == SDL_SCANCODE_A || code == SDL_SCANCODE_LEFT) {
      input->dpad |= INPUT_DPAD_LEFT;
    } else if (code == SDL_SCANCODE_D || code == SDL_SCANCODE_RIGHT) {
      input->dpad |= INPUT_DPAD_RIGHT;
    }
  }
}

void input_update_button_state(struct input_t* input, SDL_Scancode code, u8 unset) {
  if (unset) {
    if (code == SDL_SCANCODE_SPACE || code == SDL_SCANCODE_Z) {
      input->buttons ^= INPUT_BUTTON_PRIMARY;
    } else if (code == SDL_SCANCODE_LCTRL || code == SDL_SCANCODE_RCTRL || code == SDL_SCANCODE_C) {
      input->buttons ^= INPUT_BUTTON_SECONDARY;
    } else if (code == SDL_SCANCODE_ESCAPE || code == SDL_SCANCODE_M) {
      input->buttons ^= INPUT_BUTTON_MENU;
    } else if (code == SDL_SCANCODE_RETURN || code == SDL_SCANCODE_P) {
      input->buttons ^= INPUT_BUTTON_PAUSE;
    }
  } else {
    if (code == SDL_SCANCODE_SPACE || code == SDL_SCANCODE_Z) {
      input->buttons |= INPUT_BUTTON_PRIMARY;
    } else if (code == SDL_SCANCODE_LCTRL || code == SDL_SCANCODE_RCTRL || code == SDL_SCANCODE_C) {
      input->buttons |= INPUT_BUTTON_SECONDARY;
    } else if (code == SDL_SCANCODE_ESCAPE || code == SDL_SCANCODE_M) {
      input->buttons |= INPUT_BUTTON_MENU;
    } else if (code == SDL_SCANCODE_RETURN || code == SDL_SCANCODE_P) {
      input->buttons |= INPUT_BUTTON_PAUSE;
    }
  }
}

void input_update_state(struct input_t* input, SDL_Event* sdl_event) {
  if (sdl_event->type == SDL_KEYDOWN) {
    SDL_Scancode code = sdl_event->key.keysym.scancode;
    input_update_dpad_state(input, code, 0);
    input_update_button_state(input, code, 0);
  } else if (sdl_event->type == SDL_KEYUP) {
    SDL_Scancode code = sdl_event->key.keysym.scancode;
    input_update_dpad_state(input, code, 1);
    input_update_button_state(input, code, 1);
  }
}

void input_debug_state(struct input_t* input, SDL_Renderer* renderer) {
  // draws gray boxes for inputs not pressed, and green boxes for inputs pressed
  SDL_Rect box;
  box.w = 20;
  box.h = 20;
  #define dbg_box(px,py,btn){box.x=px;box.y=py;SDL_SetRenderDrawColor(renderer,0x70,(btn)?0xFF:0x70,0x70,0xFF);SDL_RenderDrawRect(renderer, &box);}
  dbg_box(3 * box.w, SCREEN_HEIGHT - 6 * box.h, input->dpad & INPUT_DPAD_UP);
  dbg_box(3 * box.w, SCREEN_HEIGHT - 2 * box.h, input->dpad & INPUT_DPAD_DOWN);
  dbg_box(1 * box.w, SCREEN_HEIGHT - 4 * box.h, input->dpad & INPUT_DPAD_LEFT);
  dbg_box(5 * box.w, SCREEN_HEIGHT - 4 * box.h, input->dpad & INPUT_DPAD_RIGHT);
  dbg_box(SCREEN_WIDTH - 4 * box.w, SCREEN_HEIGHT - 2 * box.h, input->buttons & INPUT_BUTTON_PRIMARY);
  dbg_box(SCREEN_WIDTH - 6 * box.w, SCREEN_HEIGHT - 4 * box.h, input->buttons & INPUT_BUTTON_SECONDARY);
  dbg_box(SCREEN_WIDTH - 4 * box.w, SCREEN_HEIGHT - 6 * box.h, input->buttons & INPUT_BUTTON_MENU);
  dbg_box(SCREEN_WIDTH - 2 * box.w, SCREEN_HEIGHT - 4 * box.h, input->buttons & INPUT_BUTTON_PAUSE);
  #undef dbg_box
}

// macros

#ifndef max
#define max(a,b) ((a)>(b)?(b):(a))
#endif

#define kill_texture(t){if(t){SDL_DestroyTexture(t);t=0;}}

r32 get_epoch_time() {
  return (r32)SDL_GetTicks() * 0.001f;
}

char* get_resource_path(char* filename) {
  char* base_path = SDL_GetBasePath();
  if (!base_path) {
    return 0;
  }
  char* p = base_path;
  int base_length = 0;
  while (*p != '\0') {
    base_length++;
    p++;
  }
  p = filename;
  int filename_length = 0;
  while (*p != '\0') {
    filename_length++;
    p++;
  }
  char* path = malloc(sizeof(char) * (2 + filename_length + base_length));
  int x = 0;
  p = base_path;
  while (*p != '\0') {
    path[x] = *p;
    p++;
    x++;
  }
  p = filename + 3;
  while (*p != '\0') {
    path[x] = *p;
    p++;
    x++;
  }
  path[x] = '\0';
  return path;
}

SDL_Texture* load_texture(SDL_Renderer* renderer, char* filename) {
  char* resource_path = get_resource_path(filename);
  // printf("Resource path for %s is\n %s\n", filename, resource_path != 0 ? resource_path : filename);
  SDL_Surface* surface = 0;
  if (resource_path) {
    surface = IMG_Load(resource_path);
    free(resource_path);
    resource_path = 0;
  } else {
    surface = IMG_Load(filename);
  }
  if (!surface) {
    SDL_Log("Error occurred in load_texture()\nUnable to load %s\n> %s\n", filename, IMG_GetError());
    return 0;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_FreeSurface(surface);
    SDL_Log("Error occurred in load_texture()\nUnable to create texture from surface for %s\n> %s\n", filename, SDL_GetError());
    return 0;
  }
  SDL_FreeSurface(surface);
  return texture;
}

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

void tilemap_render_bg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 bg_tile_id = map_data[i];

    tile_dest_rect->x = tile_x * tile_width;
    tile_dest_rect->y = tile_y * tile_height;
    tile_src_rect->x = tile_width * ((bg_tile_id - 1) % tileset_tile_count);
    tile_src_rect->y = tile_height * ((bg_tile_id - 1) / tileset_tile_count);
    SDL_RenderCopy(renderer, tileset, tile_src_rect, tile_dest_rect);
  }
}

void tilemap_render_mg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 mg_tile_id = map_data[(map_size * 1) + i];

    if (mg_tile_id > 0) {
      tile_src_rect->x = tile_width * ((mg_tile_id - 1) % tileset_tile_count);
      tile_src_rect->y = tile_height * ((mg_tile_id - 1) / tileset_tile_count);
      tile_dest_rect->x = tile_x * tile_width;
      tile_dest_rect->y = tile_y * tile_height;
      SDL_RenderCopy(renderer, tileset, tile_src_rect, tile_dest_rect);
    }
  }
}

void tilemap_render_ob_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {

}

void tilemap_render_fg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 fg_tile_id = map_data[(map_size * 2) + i];

    if (fg_tile_id > 0) {
      tile_src_rect->x = tile_width * ((fg_tile_id - 1) % tileset_tile_count);
      tile_src_rect->y = tile_height * ((fg_tile_id - 1) / tileset_tile_count);
      tile_dest_rect->x = tile_x * tile_width;
      tile_dest_rect->y = tile_y * tile_height;
      SDL_RenderCopy(renderer, tileset, tile_src_rect, tile_dest_rect);
    }
  }
}

void tilemap_render_xx_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 xx_tile_id = map_data[(map_size * 3) + i];

    if (xx_tile_id > 0) {
      tile_dest_rect->x = tile_x * tile_width;
      tile_dest_rect->y = tile_y * tile_height;
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0x40);
      SDL_RenderFillRect(renderer, tile_dest_rect);
    }
  }
}

void tilemap_render(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  tilemap_render_bg_layer(ctx, camera);
  tilemap_render_mg_layer(ctx, camera);
  tilemap_render_ob_layer(ctx, camera);
  tilemap_render_fg_layer(ctx, camera);
  tilemap_render_xx_layer(ctx, camera);
}

int main(int argc, char* argv[]) {
  printf("\nUntitled Game - (c) 2019, Rambling Indie Games, LLC\nhttp://ramblingindie.games\nDeveloped for entry to https://itch.io/jam/open-jam-2019\n\nBuild Date: %s\nBuild Time: %s\nFile: %s\nGodspeed...\n", __DATE__, __TIME__, __FILE__);

  int is_running = 1;
  SDL_Window* window = 0;
  SDL_Renderer* renderer = 0;
  SDL_Event event;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Failed to initialize SDL\n> %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    SDL_Log("Failed to initialize SDL2_image\n> %s\n", IMG_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow("openjamgame-2019", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

  if (!window) {
    SDL_Log("Failed to create window\n> %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

  if (!renderer) {
    SDL_Log("Failed to create renderer\n> %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Texture* logo_texture = load_texture(renderer, "../../data/openjam2019_logo_standin.png");
  if (!logo_texture) {
    SDL_Log("Resource load failure\n");
    return EXIT_FAILURE;
  }

  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  // SDL_MaximizeWindow(window);

  // tilemap demo
  u8* map_data = testmap_data;
  u8 map_width = testmap_width;
  u8 map_height = testmap_height;
  u16 map_size = map_width * map_height;
  u8 map_layers = 4; // BG, MG, FG, Collision

  SDL_Texture* testmaptiles_texture = load_texture(renderer, "../../data/testmaptiles.png");
  if (!testmaptiles_texture) {
    SDL_Log("Resource load failure\n");
    return EXIT_FAILURE;
  }

  u8 tile_width = 32;
  u8 tile_height = 32;
  u8 tiles_across_set = 7;

  SDL_Rect tile_src_rect;
  SDL_Rect tile_dest_rect;

  tile_src_rect.w = tile_width;
  tile_src_rect.h = tile_height;
  tile_dest_rect.w = tile_width;
  tile_dest_rect.h = tile_height;

  SDL_Texture* testplayersprite_texture = load_texture(renderer, "../../data/testplayersprite.png");
  if (!testplayersprite_texture) {
    SDL_Log("Resource load failure\n");
    return EXIT_FAILURE;
  }

  r32 player_x = tile_width * 3;
  r32 player_y = tile_height * 6;
  u8 player_direction = 0;
  SDL_Rect player_frame_rect;

  player_frame_rect.w = 32;
  player_frame_rect.h = 64;
  player_frame_rect.x = 0;
  player_frame_rect.y = 0;

  SDL_Rect player_dest_rect;
  player_dest_rect.w = player_frame_rect.w;
  player_dest_rect.h = player_frame_rect.h;

  struct tilemap_context_t tilemap_context;
  struct tilemap_camera_t tilemap_camera;

  tilemap_context.map_data = map_data;
  tilemap_context.map_width = map_width;
  tilemap_context.map_height = map_height;
  tilemap_context.map_size = map_size;
  tilemap_context.renderer = renderer;
  tilemap_context.tile_width = tile_width;
  tilemap_context.tile_height = tile_height;
  tilemap_context.tileset = testmaptiles_texture;
  tilemap_context.tileset_tile_count = tiles_across_set;
  tilemap_context.tile_src_rect = &tile_src_rect;
  tilemap_context.tile_dest_rect = &tile_dest_rect;

  tilemap_camera.x = 0;
  tilemap_camera.y = 0;
  tilemap_camera.width = SCREEN_WIDTH / tile_width;
  tilemap_camera.height = SCREEN_HEIGHT / tile_height;

  while (is_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_running = 0;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
          is_running = 0;
        }
      }
      input_update_state(&input_player_one, &event);
    }
    SDL_SetRenderDrawColor(renderer, 0x30, 0x60, 0x90, 0xFF);
    SDL_RenderClear(renderer);

    // SDL_RenderCopy(renderer, logo_texture, 0, 0);

    tilemap_render(&tilemap_context, &tilemap_camera);

    r32 move_speed = 2.0f;
    if (input_player_one.dpad & INPUT_DPAD_UP) {
      player_y -= move_speed;
      player_direction = 0;
    } else if (input_player_one.dpad & INPUT_DPAD_DOWN) {
      player_direction = 1;
      player_y += move_speed;
    }

    if (input_player_one.dpad & INPUT_DPAD_LEFT) {
      player_direction = 2;
      player_x -= move_speed;
    } else if (input_player_one.dpad & INPUT_DPAD_RIGHT) {
      player_direction = 3;
      player_x += move_speed;
    }

    player_frame_rect.x = player_direction * player_frame_rect.w;

    player_dest_rect.x = (u16)player_x;
    player_dest_rect.y = (u16)player_y;
    player_dest_rect.y -= tile_height;

    SDL_RenderCopy(renderer, testplayersprite_texture, &player_frame_rect, &player_dest_rect);

    input_debug_state(&input_player_one, renderer);

    SDL_RenderPresent(renderer);
  }

  kill_texture(testplayersprite_texture);
  kill_texture(testmaptiles_texture);
  kill_texture(logo_texture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}
