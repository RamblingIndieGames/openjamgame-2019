#include "game.h"

char* res_path(char* filename) {
  char* base_path = SDL_GetBasePath();
  if (base_path) {
    int filename_length = 0;
    int base_length = 0;
    char* a = base_path;
    while (*a) { base_length += 1; a += 1; }
    a = filename;
    while (*a) { filename_length += 1; a += 1; }
    char* path = malloc(sizeof(char) * 2 + filename_length + base_length);
    int p = 0;
    a = base_path;
    while (*a) { path[p] = *a; p += 1; a += 1; }
    a = filename + 3;
    while (*a) { path[p] = *a; p += 1; a += 1; }
    path[p] = '\0';
    return path;
  } else {
    return 0;
  }
}

SDL_Texture* load_texture(char* filename) {
  char* resource_path = res_path(filename);
  SDL_Surface* surface = 0;
  if (resource_path) {
    surface = IMG_Load(resource_path);
    free(resource_path);
  } else {
    surface = IMG_Load(filename);
  }
  if (!surface) {
    SDL_Log("Error occurred in load_texture()\nUnable to load %s\n%s\n", filename, SDL_GetError());
    return 0;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(main_renderer_ptr, surface);
  if (!texture) {
    SDL_FreeSurface(surface);
    SDL_Log("Error occurred in load_texture()\nUnable to create texture from surface\n%s\n", SDL_GetError());
    return 0;
  }
  SDL_FreeSurface(surface);
  return texture;
}

Mix_Music* load_music(char* filename) {
  char* resource_path = res_path(filename);
  Mix_Music* music = 0;
  if (resource_path) {
    music = Mix_LoadMUS(resource_path);
    free(resource_path);
  } else {
    music = Mix_LoadMUS(filename);
  }
  if (!music) {
    SDL_Log("Error occurred in load_music()\nUnable to load music %s\n%s\n", filename, Mix_GetError());
    return 0;
  }
  return music;
}

Mix_Chunk* load_sfx(char* filename) {
  char* resource_path = res_path(filename);
  Mix_Chunk* sfx = 0;
  if (resource_path) {
    sfx = Mix_LoadWAV(resource_path);
    free(resource_path);
  } else {
    sfx = Mix_LoadWAV(filename);
  }
  if (!sfx) {
    SDL_Log("Error occurred in load_sfx()\nUnable to load sfx %s\n%s\n", filename, Mix_GetError());
    return 0;
  }
  return sfx;
}
