#ifndef __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GAME_TITLE "Untitled - Open Jam 2019"

struct input_t {
  unsigned char up;
  unsigned char down;
  unsigned char left;
  unsigned char right;
  unsigned char primary_action;
  unsigned char secondary_action;
  unsigned char open_menu;
  unsigned char pause_game;
  unsigned char quit_game;
};

struct game_t {
  int state;
  int paused;
  float time_scale;
  struct input_t input;

  int tileset_columns;
  SDL_Texture* tileset_texture;
};

extern SDL_Window* main_window_ptr;
extern SDL_Renderer* main_renderer_ptr;
extern struct game_t* game_ptr;

extern int init_game(struct game_t* game, int argc, char** argv);
extern int run_game(struct game_t* game);
extern void destroy_game();

extern SDL_Texture* load_texture(char* filename);
extern Mix_Music* load_music(char* filename);
extern Mix_Chunk* load_sfx(char* filename);

#define kill_texture(t){if(t){SDL_DestroyTexture(t);t=0;}}
#define kill_music(m){if(m){Mix_FreeMusic(m);m=0;}}
#define kill_sfx(s){if(s){Mix_FreeChunk(s);s=0;}}

#endif // !__GAME_H__
