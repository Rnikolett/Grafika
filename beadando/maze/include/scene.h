#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

typedef struct Scene
{
    Camera camera;
    Model cube;
    Material material;
    GLuint texture_id;
    GLuint cube_texture;
    GLuint help_texture;
    GLuint floor_texture;
    GLuint charmap_id;
    int collected_cubes;
    float light_intensity;
    bool help_visible;
} Scene;




/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the maze.
 */
void draw_maze();

void draw_maze_floor();

void draw_maze_ceiling();

/**
 * Check if a ray intersects with a cube
 */
int ray_cube_intersection(const vec3 ray_origin, const vec3 ray_direction, const vec3 cube_position, float cube_size, float* distance);

/**
 * Convert screen coordinates to world coordinates
 */
void screen_to_world_coords(int screen_x, int screen_y, vec3* world_pos);

/**
 * Handle mouse interaction with cubes
 */
void handle_cube_interaction(Scene* scene, int mouse_x, int mouse_y, bool mouse_down);

#endif /* SCENE_H */
