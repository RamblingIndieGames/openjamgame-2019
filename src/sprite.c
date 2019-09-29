#include "game.h"
#include "sprite.h"
#include "texture_cache.h"
#include "tilemap.h"

static unsigned int sprite_mm_data[] = { 0, 0, 1, 3, 1, 2, 0, 0, 3, 1, 5, 5, 5, 4, 0, 0, 2, 1, 4, 5, 4, 4 };
static float sprite_mt_data[] = { 0, 0.0f, 1.0f, -1.0f, 0.7071067811865475f, -0.7071067811865475f };

unsigned int* sprite_mm;
float* sprite_mt;

spr* sprite_create(int texture_id, int frame_width, int frame_height) {
  if (!sprite_mm) {
    sprite_mm = sprite_mm_data;
    sprite_mt = sprite_mt_data;
  }

  struct cachedtexture_t* cached_texture = get_texture_by_id(texture_id);
  if (!cached_texture) {
    fprintf(stderr, "Unable to create sprite. Failed to find cached texture %d\n", texture_id);
    return 0;
  }

  SDL_Texture* render_texture = (SDL_Texture*)cached_texture->raw_data;

  spr* sprite = (spr*)malloc(sizeof(struct sprite_t));
  memset(sprite, 0, sizeof(struct sprite_t));

  static unsigned int all_sprite_ids = 0;
  sprite->id = all_sprite_ids++;

  sprite->render_alpha = 0xFF;
  sprite->render_texture = (void*)render_texture;
  sprite->render_texture_id = texture_id;
  sprite->render_texture_width = cached_texture->width;
  sprite->render_texture_height = cached_texture->height;
  sprite->render_frame_width = frame_width;
  sprite->render_frame_height = frame_height;
  sprite->animation_loop = 1;
  sprite->animation_index = -1;

  return sprite;
}

void sprite_destroy(spr** sprite) {
  if (!sprite || !*sprite) {
    return;
  }
  spr* s = *sprite;
  if (s->animations) {
    free(s->animations);
  }
  free(s);
  *sprite = 0;
}

void sprite_animation_loop(spr* sprite, int loop) {
  if (sprite) {
    sprite->animation_loop = loop;
  }
}

void sprite_add_animation(spr* sprite, int start, int count, int timeout) {
  if (!sprite) {
    return;
  }

  size_t anim_alloc_size = sizeof(unsigned char) * ((sprite->animation_count + 1)) * 3;
  sprite->animations = (unsigned char*)realloc(sprite->animations, anim_alloc_size);
  sprite->animations[0 + (sprite->animation_count * 3)] = start;
  sprite->animations[1 + (sprite->animation_count * 3)] = count;
  sprite->animations[2 + (sprite->animation_count * 3)] = timeout;
  sprite->animation_count++;
}

void sprite_select_animation(spr* sprite, int index) {
  if (sprite && index < sprite->animation_count && sprite->animation_index != index) {
    sprite->animation_index = index;
    sprite->animation_frame_start = sprite->animations[index * 3];
    sprite->animation_frame_count = sprite->animations[1 + (index * 3)];
    sprite->animation_frame_timeout = sprite->animations[2 + (index * 3)];
    sprite->animation_frame_time = 0;
    sprite->animation_frame_index = sprite->animation_frame_start;
  }
}

void sprite_start_animation(spr* sprite) {
  if (sprite) {
    sprite_debug_animationlist(sprite);
    sprite->animation_started = 1;
    sprite->animation_frame_time = 0;
    sprite->animation_frame_index = sprite->animation_frame_start;
  }
}

void sprite_pause_animation(spr* sprite) {
  if (sprite) {
    sprite->animation_started = 2;
  }
}

void sprite_resume_animation(spr* sprite) {
  if (sprite) {
    sprite->animation_started = 1;
  }
}

void sprite_stop_animation(spr* sprite) {
  if (sprite) {
    sprite->animation_started = 0;
    sprite->animation_frame_time = 0;
    sprite->animation_frame_index = sprite->animation_frame_start;
  }
}

// int sprite_collision_with_map(spr* sprite, int tile_x, int tile_y) {
//   // check
// }

// void sprite_move(spr* sprite, float motion_x, float motion_y) {
//   sprite->world_x += motion_x;
//   sprite->world_y += motion_y;
//   lock_sprite_to_map(sprite);
// }

void sprite_update_animation(spr* sprite) {
  if (!sprite || sprite->animation_started != 1) {
    // printf("not updating sprite\n");
    return;
  }

  sprite->animation_frame_time++;
  // printf("frame time %d/%d\n", sprite->animation_frame_time, sprite->animation_frame_timeout);
  if (sprite->animation_frame_time < sprite->animation_frame_timeout) {
    // printf("sprite update frame timedout\n");
    return;
  }

  sprite->animation_frame_time = 0;

  sprite->animation_frame_index++;

  // printf("frame index %d/%d\n", sprite->animation_frame_index, sprite->animation_frame_start + sprite->animation_frame_count);

  if (sprite->animation_frame_index < sprite->animation_frame_start + sprite->animation_frame_count) {
    // printf("sprite update end of anim\n");
    return;
  }

  if (!sprite->animation_loop) {
    // printf("sprite update stopping anim\n");
    sprite_stop_animation(sprite);
    return;
  }

  // printf("sprite update looping\n");

  sprite->animation_frame_time = 0;
  sprite->animation_frame_index = sprite->animation_frame_start;
}

void sprite_render(spr* sprite) {
  if (!sprite || !sprite->render_alpha) {
    return;
  }

  static SDL_Rect render_src_rect;
  static SDL_Rect render_dest_rect;

  render_src_rect.w = sprite->render_frame_width;
  render_src_rect.h = sprite->render_frame_height;
  render_dest_rect.w = sprite->render_frame_width;
  render_dest_rect.h = sprite->render_frame_height;

  SDL_Texture* render_texture = (SDL_Texture*)sprite->render_texture;

  SDL_SetTextureAlphaMod(render_texture, sprite->render_alpha);

  render_src_rect.x = (sprite->animation_frame_index % (sprite->render_texture_width / render_src_rect.w)) * render_src_rect.w;
  render_src_rect.y = (sprite->animation_frame_index / (sprite->render_texture_width / render_src_rect.w)) * render_src_rect.h;

  // FIXME: the camera is going to need to come into play to determine if the sprite should be rendered and where
  render_dest_rect.x = sprite->world_x - cam_x;
  render_dest_rect.y = sprite->world_y - cam_y;

  SDL_RenderCopy(main_renderer_ptr, render_texture, &render_src_rect, &render_dest_rect);
}

void sprite_render_hitbox(spr* sprite) {
  static SDL_Rect dst;

  dst.x = sprite->hitbox_x + sprite->world_x - cam_x;
  dst.y = sprite->hitbox_y + sprite->world_y - cam_y;
  dst.w = sprite->hitbox_width;
  dst.h = sprite->hitbox_height;

  SDL_SetRenderDrawColor(main_renderer_ptr, 255, 0, 0, 0xF0);
  SDL_RenderDrawRect(main_renderer_ptr, &dst);
}

void sprite_debug_animationlist (spr* sprite) {
  if (!sprite) {
    return;
  }

  printf("%d animations:\n", sprite->animation_count);

  for (int i = 0; i < sprite->animation_count; i++) {
    int start = sprite->animations[0 + (i * 3)];
    int count = sprite->animations[1 + (i * 3)];
    int timeout = sprite->animations[2 + (i * 3)];
    printf("\t[%03d] { start: %3d count: %3d timeout: %3d }\n", i, start, count, timeout);
  }
  printf("\n");
}
