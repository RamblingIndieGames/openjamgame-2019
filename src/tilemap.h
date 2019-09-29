#ifndef __TILEMAP_H__

extern int tile_width;
extern int tile_height;

extern int map_columns;
extern int map_rows;

extern int* map_bg_data;
extern int* map_mg_data;
extern int* map_fg_data;
extern int* map_xx_data;

extern int cam_x;
extern int cam_y;
extern int cam_width;
extern int cam_height;

extern void create_tilemap_from_array(int columns, int rows, int* arr);
extern int load_tilemap_from_file(char* filename);
extern void destroy_tilemap();
extern void render_tilemap(int target_x, int target_y);

#endif // !__TILEMAP_H__
