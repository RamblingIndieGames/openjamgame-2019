#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
  SDL_MaximizeWindow(window);

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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, logo_texture, 0, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    input_debug_state(&input_player_one, renderer);

    SDL_RenderPresent(renderer);
  }

  kill_texture(logo_texture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}
