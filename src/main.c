#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

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
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}
