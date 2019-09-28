#include "main.h"

r32 get_epoch_time() {
  return (r32)SDL_GetTicks() * 0.001f;
}

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
