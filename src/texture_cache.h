#ifndef __TEXTURE_H__

struct cachedtexture_t {
  int id;
  int width;
  int height;
  void* raw_data;
};

extern struct cachedtexture_t* textures;
extern int texture_count;

extern void add_texture_to_cache(int id, char* filename);
extern struct cachedtexture_t* get_texture_by_id(int id);
extern void unload_texture_by_id(int id);
extern void destroy_texture_cache();

#endif // !__TEXTURE_H__
