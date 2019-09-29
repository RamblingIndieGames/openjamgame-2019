#include "game.h"
#include "dimmer.h"

SDL_Texture* dimmer_texture = 0;
SDL_Rect dimmer_rect;

float dimmer_opacity = 0.0f;
int should_show_dimmer = 0;

void init_dimmer(unsigned int color) {
  // printf("init dimmer 0x%08X\n", color);
  if (!dimmer_texture) {
    dimmer_texture = SDL_CreateTexture(main_renderer_ptr, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, 2, 2);
    if (!dimmer_texture) {
      SDL_Log("Error occurred in init_dimmer()\n%s", SDL_GetError());
    }
  }
  unsigned int dimmer_fill[] = { color, color, color, color };
  SDL_UpdateTexture(dimmer_texture, 0, &dimmer_fill, 2 * sizeof(unsigned int));
  SDL_SetTextureBlendMode(dimmer_texture, SDL_BLENDMODE_BLEND);
  dimmer_rect.x = 0;
  dimmer_rect.y = 0;
  dimmer_rect.w = SCREEN_WIDTH;
  dimmer_rect.h = SCREEN_HEIGHT;
  should_show_dimmer = 0;
}

void destroy_dimmer() {
  kill_texture(dimmer_texture);
}

void show_dimmer(float opacity) {
  should_show_dimmer = 1;
  dimmer_opacity = opacity;
  int alpha = (int)(255.0f * opacity);
  SDL_SetTextureAlphaMod(dimmer_texture, alpha);
}

void hide_dimmer() {
  should_show_dimmer = 0;
}

void render_dimmer() {
  if (should_show_dimmer) {
    SDL_RenderCopy(main_renderer_ptr, dimmer_texture, 0, &dimmer_rect);
  }
}
