#include "game.h"
#include "texture_cache.h"

struct cachedtexture_t* textures = 0;
int texture_count = 0;

void add_texture_to_cache(int id, char* filename) {
  SDL_Texture* texture = load_texture(filename);
  if (!texture) {
    fprintf(stderr, "Unable to cache texture [%d] -> \"%s\" because it failed to load.\n", id, filename);
    return;
  }

  int texture_width, texture_height;
  SDL_QueryTexture(texture, 0, 0, &texture_width, &texture_height);

  size_t cache_size = sizeof(struct cachedtexture_t) * (texture_count + 1);
  textures = (struct cachedtexture_t*)realloc(textures, cache_size);
  textures[texture_count].id = id;
  textures[texture_count].width = texture_width;
  textures[texture_count].height = texture_height;
  textures[texture_count].raw_data = (void*)texture;
  texture_count++;
}

struct cachedtexture_t* get_texture_by_id(int id) {
  for (int i = 0; i < texture_count; i++) {
    if (textures[i].id == id) {
      return &textures[i];
    }
  }
  return 0;
}

void kill_texture_in_cache_at_index(int index) {
  SDL_Texture* t = (SDL_Texture*)textures[index].raw_data;
  kill_texture(t);
  texture_count--;
}

void unload_texture_by_id(int id) {
  if (texture_count == 1) {
    kill_texture_in_cache_at_index(0);
    return;
  }

  size_t size = sizeof(struct cachedtexture_t);
  int last_index = texture_count - 1;

  for (int i = 0; i < texture_count; i++) {
    if (textures[i].id == id) {
      struct cachedtexture_t t;

      memcpy(&t, &textures[i], size);
      memcpy(&textures[i], &textures[last_index], size);
      memcpy(&textures[last_index], &t, size);
      kill_texture_in_cache_at_index(last_index);
      return;
    }
  }
}

void destroy_texture_cache() {
  if (textures) {
    if (texture_count) {
      while (texture_count > 0) {
        kill_texture_in_cache_at_index(texture_count - 1);
      }
    }
    free(textures);
    textures = 0;
  }
}
