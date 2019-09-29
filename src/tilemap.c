#include "game.h"
#include "tilemap.h"

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

void render_tilemap(int target_x, int target_y) {
  int map_x = target_x / tile_width;
  int map_y = target_y / tile_height;
  int off_x = target_x % tile_width;
  int off_y = target_y % tile_height;

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
      src.x = tile_width * (tile_id % game_ptr->tileset_columns);
      src.y = tile_height * (tile_id / game_ptr->tileset_columns);
      dst.x = column * tile_width - off_x;
      dst.y = row * tile_height - off_y;
      SDL_RenderCopy(main_renderer_ptr, game_ptr->tileset_texture, &src, &dst);
    }
  }

  SDL_SetRenderDrawBlendMode(main_renderer_ptr, SDL_BLENDMODE_BLEND);

  // for (int row = 0; row < map_rows; row++) {
  //   for (int column = 0; column < map_columns; column++) {
  //     int index = (row + map_y) * map_columns + column + map_x;
  //     int tile_id = map_bg_data[index];
  //     if (tile_id > 0) {
  //       src.x = tile_width * (tile_id % game_ptr->tileset_columns);
  //       src.y = tile_height * (tile_id / game_ptr->tileset_columns);
  //       dst.x = column * tile_width - off_x;
  //       dst.y = row * tile_height - off_y;
  //       SDL_RenderCopy(main_renderer_ptr, game_ptr->tileset_texture, &src, &dst);
  //     }
  //   }
  // }
}
