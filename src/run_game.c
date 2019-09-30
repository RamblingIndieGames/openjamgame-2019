#include "game.h"
#include "scene.h"
#include "dimmer.h"

#ifndef max
#define max(a,b) ((a) > (b) ? (b) : (a))
#endif

static float get_epoch_time() {
  return (float)SDL_GetTicks() * 0.001f;
}

void quit_game() {
  game_ptr->state = 0;
}

int run_game(struct game_t* game) {
  if (!game) {
    fprintf(stderr, "Error occurred in run_game()\nUnable to run game because: game is NULL!\n");
    return EXIT_FAILURE;
  }

  unsigned char hidden = 0;
  float last_time = get_epoch_time();
  float new_time = 0;
  float delta_time = 0.0f;

  SDL_Event event;

  while (game->state) {
    if (hidden) {
      SDL_Delay(max(0, (int)(16 - (get_epoch_time() - last_time) * 1000)));
    }

    if (!game->paused) {
      new_time = get_epoch_time();
      if (new_time - last_time < 1) {
        delta_time = (new_time - last_time) * game->time_scale;
        scene_ptr->update(delta_time);
      }
      last_time = new_time;
    }

    if (hidden == 0) {
      SDL_SetRenderDrawColor(main_renderer_ptr, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderClear(main_renderer_ptr);
      scene_ptr->render();
      render_dimmer();
      SDL_RenderPresent(main_renderer_ptr);
    }

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit_game();
      } else if (event.type == SDL_KEYDOWN) {
        SDL_Scancode code = event.key.keysym.scancode;
        int value = 1;
        if (code == SDL_SCANCODE_W || code == SDL_SCANCODE_UP) {
          game->input.up = value;
        } else if (code == SDL_SCANCODE_S || code == SDL_SCANCODE_DOWN) {
          game->input.down = value;
        } else if (code == SDL_SCANCODE_A || code == SDL_SCANCODE_LEFT) {
          game->input.left = value;
        } else if (code == SDL_SCANCODE_D || code == SDL_SCANCODE_RIGHT) {
          game->input.right = value;
        } else if (code == SDL_SCANCODE_Q) {
          game->input.quit_game = value;
        } else if (code == SDL_SCANCODE_SPACE || code == SDL_SCANCODE_Z) {
          game->input.primary_action = value;
        }
      } else if (event.type == SDL_KEYUP) {
        SDL_Scancode code = event.key.keysym.scancode;
        int value = 0;
        if (code == SDL_SCANCODE_W || code == SDL_SCANCODE_UP) {
          game->input.up = value;
        } else if (code == SDL_SCANCODE_S || code == SDL_SCANCODE_DOWN) {
          game->input.down = value;
        } else if (code == SDL_SCANCODE_A || code == SDL_SCANCODE_LEFT) {
          game->input.left = value;
        } else if (code == SDL_SCANCODE_D || code == SDL_SCANCODE_RIGHT) {
          game->input.right = value;
        } else if (code == SDL_SCANCODE_Q) {
          game->input.quit_game = value;
        } else if (code == SDL_SCANCODE_SPACE || code == SDL_SCANCODE_Z) {
          game->input.primary_action = value;
        }
      } else if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_HIDDEN || event.window.event == SDL_WINDOWEVENT_MINIMIZED) {
          hidden = 1;
        } else if (event.window.event == SDL_WINDOWEVENT_SHOWN || event.window.event == SDL_WINDOWEVENT_RESTORED) {
          hidden = 0;
        }
      }
    }

    if (game->input.quit_game) {
      quit_game();
    }
  }

  return EXIT_SUCCESS;
}
