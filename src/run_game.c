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
        if (event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_UP) {
          game->input.up = 1;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_S || event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
          game->input.down = 1;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
          game->input.left = 1;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_D || event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
          game->input.right = 1;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
          game->input.quit_game = 1;
        }
      } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_UP) {
          game->input.up = 0;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_S || event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
          game->input.down = 0;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
          game->input.left = 0;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_D || event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
          game->input.right = 0;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
          game->input.quit_game = 0;
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
