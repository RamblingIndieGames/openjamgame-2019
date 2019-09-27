#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

#define kill_texture(t){if(t){SDL_DestroyTexture(t);t=0;}}

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

  window = SDL_CreateWindow("openjamgame-2019", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

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

  while (is_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_running = 0;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          is_running = 0;
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0x30, 0x60, 0x90, 0xFF);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, logo_texture, 0, 0);

    SDL_RenderPresent(renderer);
  }

  kill_texture(logo_texture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}
