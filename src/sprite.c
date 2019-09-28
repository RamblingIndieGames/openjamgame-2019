#include "main.h"

spr* sprite_create(SDL_Texture* texture, u8 width, u8 height) {
  spr* sprite = (spr*)malloc(sizeof(struct sprite_t));
  memset(sprite, 0, sizeof(struct sprite_t));

  static u32 all_sprite_ids = 0;
  sprite->id = all_sprite_ids++;

  sprite->render_texture = texture;
  sprite->render_alpha = 0xFF;

  int w, h;
  SDL_QueryTexture(texture, 0, 0, &w, &h);
  sprite->render_texture_width = w;
  sprite->render_texture_height = h;

  if (!width || !height) {
    sprite->render_src_rect.w = w;
    sprite->render_src_rect.h = h;
    sprite->render_dest_rect.w = w;
    sprite->render_dest_rect.h = h;

  } else {
    sprite->render_src_rect.w = width;
    sprite->render_src_rect.h = height;
    sprite->render_dest_rect.w = width;
    sprite->render_dest_rect.h = height;
  }

  sprite->animation_loop = 1;

  return sprite;
}

void sprite_debug_animationlist (spr* sprite) {
  if (!sprite) {
    return;
  }

  printf("%d animations:\n", sprite->animation_count);

  for (int i = 0; i < sprite->animation_count; i++) {
    u8 start = sprite->animations[0 + (i * 3)];
    u8 count = sprite->animations[1 + (i * 3)];
    u8 timeout = sprite->animations[2 + (i * 3)];
    printf("\t[%03d] { start: %3d count: %3d timeout: %3d }\n", i, start, count, timeout);
  }
  printf("\n");
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

void sprite_animation_loop(spr* sprite, u8 loop) {
  if (sprite) {
    sprite->animation_loop = loop;
  }
}

void sprite_add_animation(spr* sprite, u8 start, u8 count, u8 timeout) {
  if (!sprite) {
    return;
  }

  size_t anim_alloc_size = sizeof(u8) * ((sprite->animation_count + 1)) * 3;
  sprite->animations = (u8*)realloc(sprite->animations, anim_alloc_size);
  sprite->animations[0 + (sprite->animation_count * 3)] = start;
  sprite->animations[1 + (sprite->animation_count * 3)] = count;
  sprite->animations[2 + (sprite->animation_count * 3)] = timeout;
  sprite->animation_count++;
}

void sprite_select_animation(spr* sprite, u8 index) {
  if (sprite && index < sprite->animation_count) {
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


void sprite_render(spr* sprite, SDL_Renderer* renderer) {
  if (!sprite || !sprite->render_alpha || !renderer) {
    return;
  }

  SDL_SetTextureAlphaMod(sprite->render_texture, sprite->render_alpha);

  sprite->render_src_rect.x = (sprite->animation_frame_index % (sprite->render_texture_width / sprite->render_src_rect.w)) * sprite->render_src_rect.w;
  sprite->render_src_rect.y = (sprite->animation_frame_index / (sprite->render_texture_width / sprite->render_src_rect.w)) * sprite->render_src_rect.h;

  // FIXME: the camera is going to need to come into play to determine if the sprite should be rendered and where
  sprite->render_dest_rect.x = sprite->world_x;
  sprite->render_dest_rect.y = sprite->world_y;

  SDL_RenderCopy(renderer, sprite->render_texture, &sprite->render_src_rect, &sprite->render_dest_rect);
}
