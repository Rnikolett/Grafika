#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>

#define MAZE_W 10
#define MAZE_H 10
#define FOV 60.0
#define NUM_RAYS 120
#define PI 3.1415926535

int maze[MAZE_H][MAZE_W] = {
    {1,1,0,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,1,0,1,0,1},
    {1,1,1,1,0,1,0,1,0,1},
    {1,0,0,1,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,0,1,1,1}
};


void init_scene(Scene* scene)
{
    // load_model(&(scene->cube), "assets/models/cube.obj");
    // scene->texture_id = load_texture("assets/textures/cube.png");
	
    //load_model(&(scene->cube), "assets/models/cube.obj");
    //scene->texture_id = load_texture("assets/textures/cube.png");

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
}

void set_lighting()
{
    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 0.0f, 0.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_maze();
    //draw_model(&(scene->cube));
}

float player_x = 1.5f, player_y = 1.5f, player_angle = 0.0f;

void draw_maze()
{
glBegin(GL_QUADS);

//glColor3f(1, 0, 0);
//floor
glVertex3f(-3, 2, 0);
glVertex3f(3, 2, 0);
glVertex3f(3, -2, 0);
glVertex3f(-3, -2, 0);

//behind
glVertex3f(-3, 2, 0);
glVertex3f(-3, 2, 1);
glVertex3f(-3, -2, 1);
glVertex3f(-3, -2, 0);

//right
glVertex3f(-3, -2, 0);
glVertex3f(-3, -2, 1);
glVertex3f(1, -2, 1);
glVertex3f(1, -2, 0);

glVertex3f(2, -2, 0);
glVertex3f(2, -2, 1);
glVertex3f(3, -2, 1);
glVertex3f(3, -2, 0);

//left
glVertex3f(-1, 2, 0);
glVertex3f(-1, 2, 1);
glVertex3f(3, 2, 1);
glVertex3f(3, 2, 0);

glVertex3f(-3, 2, 0);
glVertex3f(-3, 2, 1);
glVertex3f(-2, 2, 1);
glVertex3f(-2, 2, 0);

//front
glVertex3f(3, 2, 0);
glVertex3f(3, 2, 1);
glVertex3f(3, -2, 1);
glVertex3f(3, -2, 0);

//inside
glVertex3f(1, -2, 0);
glVertex3f(1, -2, 1);
glVertex3f(1, -1, 1);
glVertex3f(1, -1, 0);

glVertex3f(1, -1, 0);
glVertex3f(1, -1, 1);
glVertex3f(-1, -1, 1);
glVertex3f(-1, -1, 0);

glVertex3f(-1, -1, 0);
glVertex3f(-1, -1, 1);
glVertex3f(-1, 1, 1);
glVertex3f(-1, 1, 0);

glVertex3f(-1, 1, 0);
glVertex3f(-1, 1, 1);
glVertex3f(2, 1, 1);
glVertex3f(2, 1, 0);

glVertex3f(2, 1, 0);
glVertex3f(2, 1, 1);
glVertex3f(2, 0, 1);
glVertex3f(2, 0, 0);

glVertex3f(2, 0, 0);
glVertex3f(2, 0, 1);
glVertex3f(0, 0, 1);
glVertex3f(0, 0, 0);

//small
glVertex3f(2, -2, 0);
glVertex3f(2, -2, 1);
glVertex3f(2, -1, 1);
glVertex3f(2, -1, 0);

glEnd();

}
