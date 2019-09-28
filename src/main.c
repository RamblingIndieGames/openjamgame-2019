#include "main.h"

#include "testmap.h"

struct input_t input_player_one;

struct debugroom {
  SDL_Texture* animtest_texture;
  spr* animtest_sprite;
};

void debugroom_animtest_init(SDL_Renderer* renderer);
void debugroom_animtest_kill();
void debugroom_animtest_update();
void debugroom_animtest_render(SDL_Renderer* renderer);

struct debugroom debugroom;
void debugroom_animtest_init(SDL_Renderer* renderer) {
  debugroom.animtest_texture = load_texture(renderer, "../../data/animtest.png");

  debugroom.animtest_sprite = sprite_create(debugroom.animtest_texture, 16, 16);

  sprite_add_animation(debugroom.animtest_sprite, 0, 4, 12);
  sprite_add_animation(debugroom.animtest_sprite, 4, 4, 12);
  sprite_add_animation(debugroom.animtest_sprite, 8, 4, 12);
  sprite_add_animation(debugroom.animtest_sprite, 12, 4, 12);

  sprite_select_animation(debugroom.animtest_sprite, 0);

  sprite_start_animation(debugroom.animtest_sprite);
}

void debugroom_animtest_kill() {
  sprite_destroy(&debugroom.animtest_sprite);
  kill_texture(debugroom.animtest_texture);
}

void debugroom_animtest_update() {
  sprite_update_animation(debugroom.animtest_sprite);
}

void debugroom_animtest_render(SDL_Renderer* renderer) {
  sprite_render(debugroom.animtest_sprite, renderer);
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

  debugroom_animtest_init(renderer);

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

    debugroom_animtest_update();
    debugroom_animtest_render(renderer);

    input_debug_state(&input_player_one, renderer);

    SDL_RenderPresent(renderer);
  }

  debugroom_animtest_kill();

  kill_texture(testplayersprite_texture);
  kill_texture(testmaptiles_texture);
  kill_texture(logo_texture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}
