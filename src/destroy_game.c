#include "game.h"
#include "scene.h"
#include "scenes.h"
#include "dimmer.h"

void destroy_game() {
  if (!game_ptr) {
    fprintf(stderr, "Error occurred in destroy_game()\nUnable to destroy game because: game_ptr is NULL!\n");
    return;
  }

  // cleanup
  destroy_play_scene();

  Mix_CloseAudio();
  while (Mix_Init(0)) {
    Mix_Quit();
  }

  SDL_DestroyRenderer(main_renderer_ptr);
  SDL_DestroyWindow(main_window_ptr);
  SDL_Quit();

  game_ptr = 0;
}
