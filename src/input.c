#include "main.h"

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
