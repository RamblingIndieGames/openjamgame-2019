#include "game.h"
#include "dimmer.h"
#include "scene.h"
#include "scenes.h"

SDL_Window* main_window_ptr = 0;
SDL_Renderer* main_renderer_ptr = 0;
struct game_t* game_ptr = 0;

struct scene_t* scene_ptr = 0;
struct scene_t current_scene;

int init_game(struct game_t* game, int argc, char** argv) {
  if (!game) {
    fprintf(stderr, "Error occurred in run_game()\nUnable to run game because: game is NULL!\n");
    return EXIT_FAILURE;
  }

  srand(time(0));

  game->state = 1;
  game->paused = 0;
  game->time_scale = 1.0f;
  memset(&game->input, 0, sizeof(struct input_t));

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Error occurred in init_game()\nUnable to initialize SDL2: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    SDL_Log("Failed to initialize SDL2_image\n> %s\n", IMG_GetError());
    return EXIT_FAILURE;
  }

  main_window_ptr = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
#if DEBUG
  SDL_WINDOW_RESIZABLE
#else
  SDL_WINDOW_FULLSCREEN_DESKTOP
#endif
  );

  printf("main_window_ptr: %p\n", main_window_ptr);

  if (!main_window_ptr) {
    SDL_Log("Error occurred in init_game()\nUnable to create the main window: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  main_renderer_ptr = SDL_CreateRenderer(main_window_ptr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

  if (!main_renderer_ptr) {
    SDL_Log("Error occurred in init_game()\nUnable to create the main renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(main_renderer_ptr, SCREEN_WIDTH, SCREEN_HEIGHT);
  // SDL_MaximizeWindow(main_window_ptr);

  game_ptr = game;

  int audioHz = 22050;
  unsigned short audioFormat = MIX_DEFAULT_FORMAT; // AUDIO_S16SYS
  int audioChannels = 2; // STEREO
  int audioChunkSize = 4096;

  if (Mix_OpenAudio(audioHz, audioFormat, audioChannels, audioChunkSize) == -1) {
    SDL_Log("Error occurred in init_game()\nUnable to open SDL mixer: %s\n", Mix_GetError());
    return EXIT_FAILURE;
  }

  int audioFlags = MIX_INIT_OGG;
  int audioInit = Mix_Init(audioFlags);
  if ((audioInit & audioFlags) != audioFlags) {
    SDL_Log("Error occurred in init_game()\nUnable to init SDL mixer: %s\n", Mix_GetError());
    return EXIT_FAILURE;
  }

  // setup scene
  scene_ptr = &current_scene;
  scene_ptr->id = 0;
  scene_ptr->init = &init_play_scene;
  scene_ptr->destroy = &destroy_play_scene;
  scene_ptr->enter = &enter_play_scene;
  scene_ptr->exit = &exit_play_scene;
  scene_ptr->update = &update_play_scene;
  scene_ptr->render = &render_play_scene;

  init_dimmer(0xFF000000);

  init_play_scene();

  scene_ptr->enter();

  return EXIT_SUCCESS;
}
