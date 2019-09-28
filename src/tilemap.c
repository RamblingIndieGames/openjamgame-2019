#include "main.h"

void tilemap_render_bg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 bg_tile_id = map_data[i];

    tile_dest_rect->x = tile_x * tile_width;
    tile_dest_rect->y = tile_y * tile_height;
    tile_src_rect->x = tile_width * ((bg_tile_id - 1) % tileset_tile_count);
    tile_src_rect->y = tile_height * ((bg_tile_id - 1) / tileset_tile_count);
    SDL_RenderCopy(renderer, tileset, tile_src_rect, tile_dest_rect);
  }
}

void tilemap_render_mg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 mg_tile_id = map_data[(map_size * 1) + i];

    if (mg_tile_id > 0) {
      tile_src_rect->x = tile_width * ((mg_tile_id - 1) % tileset_tile_count);
      tile_src_rect->y = tile_height * ((mg_tile_id - 1) / tileset_tile_count);
      tile_dest_rect->x = tile_x * tile_width;
      tile_dest_rect->y = tile_y * tile_height;
      SDL_RenderCopy(renderer, tileset, tile_src_rect, tile_dest_rect);
    }
  }
}

void tilemap_render_ob_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {

}

void tilemap_render_fg_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 fg_tile_id = map_data[(map_size * 2) + i];

    if (fg_tile_id > 0) {
      tile_src_rect->x = tile_width * ((fg_tile_id - 1) % tileset_tile_count);
      tile_src_rect->y = tile_height * ((fg_tile_id - 1) / tileset_tile_count);
      tile_dest_rect->x = tile_x * tile_width;
      tile_dest_rect->y = tile_y * tile_height;
      SDL_RenderCopy(renderer, tileset, tile_src_rect, tile_dest_rect);
    }
  }
}

void tilemap_render_xx_layer(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  SDL_Renderer* renderer = ctx->renderer;
  SDL_Texture* tileset = ctx->tileset;
  u8 tileset_tile_count = ctx->tileset_tile_count;
  u8* map_data = ctx->map_data;
  u8 map_width = ctx->map_width;
  u8 map_height = ctx->map_height;
  u16 map_size = ctx->map_size;
  u8 tile_width = ctx->tile_width;
  u8 tile_height = ctx->tile_height;
  SDL_Rect* tile_src_rect = ctx->tile_src_rect;
  SDL_Rect* tile_dest_rect = ctx->tile_dest_rect;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for (u16 i = 0; i < map_size; i++) {
    u8 tile_x = i % map_width;
    u8 tile_y = i / map_width;
    u8 xx_tile_id = map_data[(map_size * 3) + i];

    if (xx_tile_id > 0) {
      tile_dest_rect->x = tile_x * tile_width;
      tile_dest_rect->y = tile_y * tile_height;
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0x40);
      SDL_RenderFillRect(renderer, tile_dest_rect);
    }
  }
}

void tilemap_render(struct tilemap_context_t* ctx, struct tilemap_camera_t* camera) {
  tilemap_render_bg_layer(ctx, camera);
  tilemap_render_mg_layer(ctx, camera);
  tilemap_render_ob_layer(ctx, camera);
  tilemap_render_fg_layer(ctx, camera);
  tilemap_render_xx_layer(ctx, camera);
}
