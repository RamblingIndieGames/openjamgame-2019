#include "game.h"
#include "tilemap.h"
#include "sprite.h"

int tile_width = 32;
int tile_height = 32;

int map_columns = 80;
int map_rows = 60;

int* map_bg_data = 0;
int* map_mg_data = 0;
int* map_fg_data = 0;
int* map_xx_data = 0;

int cam_x = 0;
int cam_y = 0;
int cam_width = 21;
int cam_height = 16;

void create_tilemap_from_array(int columns, int rows, int* arr) {
  destroy_tilemap();

  map_columns = columns;
  map_rows = rows;

  int size = map_columns * map_rows;
  size_t map_alloc_size = sizeof(int) * size;

  map_bg_data = (int*)malloc(map_alloc_size);
  map_mg_data = (int*)malloc(map_alloc_size);
  map_fg_data = (int*)malloc(map_alloc_size);
  map_xx_data = (int*)malloc(map_alloc_size);

  for (int i = 0; i < size; i++) {
    map_bg_data[i] = arr[i];
    map_mg_data[i] = arr[(size * 1) + i];
    map_fg_data[i] = arr[(size * 2) + i];
    map_xx_data[i] = arr[(size * 3) + i];
  }
}

int load_tilemap_from_file(char* filename) {
  return EXIT_FAILURE;
}

void destroy_tilemap() {
  if (map_bg_data) { free(map_bg_data); map_bg_data = 0; }
  if (map_mg_data) { free(map_mg_data); map_mg_data = 0; }
  if (map_fg_data) { free(map_fg_data); map_fg_data = 0; }
  if (map_xx_data) { free(map_xx_data); map_xx_data = 0; }
  map_columns = 0;
  map_rows = 0;
}

void render_tilemap_bg() {
  int map_x = cam_x / tile_width;
  int map_y = cam_y / tile_height;
  int off_x = cam_x % tile_width;
  int off_y = cam_y % tile_height;

  SDL_Rect dst;
  dst.w = tile_width;
  dst.h = tile_height;

  SDL_Rect src;
  src.w = tile_width;
  src.h = tile_height;

  int size = cam_width * cam_height;

  SDL_SetRenderDrawBlendMode(main_renderer_ptr, SDL_BLENDMODE_NONE);

  for (int i = 0; i < size; i++) {
    int column = (i % cam_width);
    int row = (i / cam_width);
    int index = (row + map_y) * map_columns + column + map_x;
    int tile_id = map_bg_data[index];
    if (tile_id > 0) {
      tile_id--;
      src.x = tile_width * (tile_id % game_ptr->tileset_columns);
      src.y = tile_height * (tile_id / game_ptr->tileset_columns);
      dst.x = column * tile_width - off_x;
      dst.y = row * tile_height - off_y;
      SDL_RenderCopy(main_renderer_ptr, game_ptr->tileset_texture, &src, &dst);
    }
  }
}

void render_tilemap_mg() {
  int map_x = cam_x / tile_width;
  int map_y = cam_y / tile_height;
  int off_x = cam_x % tile_width;
  int off_y = cam_y % tile_height;

  SDL_Rect dst;
  dst.w = tile_width;
  dst.h = tile_height;

  SDL_Rect src;
  src.w = tile_width;
  src.h = tile_height;

  int size = cam_width * cam_height;

  SDL_SetRenderDrawBlendMode(main_renderer_ptr, SDL_BLENDMODE_BLEND);

  for (int i = 0; i < size; i++) {
    int column = (i % cam_width);
    int row = (i / cam_width);
    int index = (row + map_y) * map_columns + column + map_x;
    int tile_id = map_mg_data[index];
    if (tile_id > 0) {
      tile_id--;
      src.x = tile_width * (tile_id % game_ptr->tileset_columns);
      src.y = tile_height * (tile_id / game_ptr->tileset_columns);
      dst.x = column * tile_width - off_x;
      dst.y = row * tile_height - off_y;
      SDL_RenderCopy(main_renderer_ptr, game_ptr->tileset_texture, &src, &dst);
    }
  }
}

void render_tilemap_fg() {
  int map_x = cam_x / tile_width;
  int map_y = cam_y / tile_height;
  int off_x = cam_x % tile_width;
  int off_y = cam_y % tile_height;

  SDL_Rect dst;
  dst.w = tile_width;
  dst.h = tile_height;

  SDL_Rect src;
  src.w = tile_width;
  src.h = tile_height;

  int size = cam_width * cam_height;

  SDL_SetRenderDrawBlendMode(main_renderer_ptr, SDL_BLENDMODE_BLEND);

  for (int i = 0; i < size; i++) {
    int column = (i % cam_width);
    int row = (i / cam_width);
    int index = (row + map_y) * map_columns + column + map_x;
    int tile_id = map_fg_data[index];
    if (tile_id > 0) {
      tile_id--;
      src.x = tile_width * (tile_id % game_ptr->tileset_columns);
      src.y = tile_height * (tile_id / game_ptr->tileset_columns);
      dst.x = column * tile_width - off_x;
      dst.y = row * tile_height - off_y;
      SDL_RenderCopy(main_renderer_ptr, game_ptr->tileset_texture, &src, &dst);
    }
  }
}

void render_tilemap_xx() {
  int map_x = cam_x / tile_width;
  int map_y = cam_y / tile_height;
  int off_x = cam_x % tile_width;
  int off_y = cam_y % tile_height;

  SDL_Rect dst;
  dst.w = tile_width;
  dst.h = tile_height;

  int size = cam_width * cam_height;

  SDL_SetRenderDrawBlendMode(main_renderer_ptr, SDL_BLENDMODE_BLEND);

  for (int i = 0; i < size; i++) {
    int column = (i % cam_width);
    int row = (i / cam_width);
    int index = (row + map_y) * map_columns + column + map_x;
    int tile_id = map_xx_data[index];
    if (tile_id > 0) {
      dst.x = column * tile_width - off_x;
      dst.y = row * tile_height - off_y;
      SDL_SetRenderDrawColor(main_renderer_ptr, 255, 0, 0, 0x40);
      SDL_RenderFillRect(main_renderer_ptr, &dst);
    }
  }
}

#define clamp(v,lo,hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void lock_camera_to_map() {
  int map_width_in_pixels = map_columns * tile_width;
  int map_height_in_pixels = map_columns * tile_height;
  int cam_width_in_pixels = cam_width * tile_width;
  int cam_height_in_pixels = cam_width * tile_height;

  cam_x = clamp(cam_x, 0, map_width_in_pixels - cam_width_in_pixels);
  cam_y = clamp(cam_y, 0, map_height_in_pixels - cam_height_in_pixels);
}

void lock_sprite_to_map(struct sprite_t* sprite) {
  if (!sprite) {
    return;
  }

  int map_width_in_pixels = map_columns * tile_width;
  int map_height_in_pixels = map_columns * tile_height;

  sprite->world_x = clamp(sprite->world_x, 0, map_width_in_pixels - sprite->render_frame_width);
  sprite->world_y = clamp(sprite->world_y, 0, map_height_in_pixels - sprite->render_frame_height);
}

void lock_camera_to_pos(int x, int y) {
  int cam_width_in_pixels = cam_width * tile_width;
  int cam_height_in_pixels = cam_width * tile_height;
  cam_x = x - (cam_width_in_pixels / 2);
  cam_y = y - (cam_height_in_pixels / 2);
  lock_camera_to_map();
}
