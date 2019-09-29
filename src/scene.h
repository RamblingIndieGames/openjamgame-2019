#ifndef __SCENE_H__

struct scene_t {
  unsigned int id;
  void (*init)();
  void (*destroy)();
  void (*enter)();
  void (*exit)();
  void (*update)(float dt);
  void (*render)();
};

struct scenestacknode_t {
  unsigned int id;
  struct scene_t* scene;
  struct scenestacknode_t* next;
};

struct scenestack_t {
  struct scenestacknode_t* top;
  int num_scenes;
};

extern struct scenestack_t scenestack_ptr;
extern struct scene_t* scene_ptr;

extern void scene_stack_init();
extern void scene_stack_destroy();
extern void scene_stack_push(struct scene_t* scene);
extern struct scene_t* scene_stack_pop();
extern struct scene_t* scene_stack_top();
extern void scene_stack_update(float dt);
extern void scene_stack_render();

#endif // !__SCENE_H__
