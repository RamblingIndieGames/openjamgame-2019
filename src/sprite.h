#ifndef __SPRITE_H__

extern unsigned int* sprite_mm;
extern unsigned int* sprite_mt;

struct sprite_t {
  unsigned int id;

  float world_x;
  float world_y;

  int world_xtile;
  int world_ytile;

  int hitbox_x;
  int hitbox_y;
  int hitbox_width;
  int hitbox_height;

  int animation_started; // 0 stopped, 1 - started, 2 - paused
  int animation_loop; // 0 - play once and stop, 1 - loop to start at end

  /*
    an animation is defined by
      frame_start
      frame_count
      frame_timeout
      animations is an array of 3 * animation_count unsigned char number values
      every 3 values defines one animation
  */
  int animation_index;
  int animation_count;
  unsigned char* animations;

  int animation_frame_index;
  int animation_frame_start;
  int animation_frame_count;
  int animation_frame_time;
  int animation_frame_timeout;

  int render_alpha;
  // size of texture
  int render_texture_width;
  int render_texture_height;
  // size of frame
  int render_frame_width;
  int render_frame_height;
  int render_texture_id;
  void* render_texture;
};

typedef struct sprite_t spr;
extern spr* sprite_create(int texture_id, int frame_width, int frame_height);
extern void sprite_destroy(spr** sprite);
extern void sprite_animation_loop(spr* sprite, int loop);
extern void sprite_add_animation(spr* sprite, int start, int count, int timeout);
extern void sprite_select_animation(spr* sprite, int index);
extern void sprite_start_animation(spr* sprite);
extern void sprite_pause_animation(spr* sprite);
extern void sprite_resume_animation(spr* sprite);
extern void sprite_stop_animation(spr* sprite);
extern void sprite_update_animation(spr* sprite);
extern void sprite_render(spr* sprite);
extern void sprite_debug_animationlist (spr* sprite);

#endif // !__SPRITE_H__
