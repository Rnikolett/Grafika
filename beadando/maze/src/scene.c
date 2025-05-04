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
	//background color
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	
    //load_model(&(scene->cube), "assets/models/cube.obj");
    //scene->texture_id = load_texture("assets/textures/cube.png");
	
	scene->texture_id = load_texture("assets/textures/brickTexture.png");
	
	
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    
    scene->material.ambient.red = 0.5;
    scene->material.ambient.green = 0.4;
    scene->material.ambient.blue = 0.3;

    scene->material.diffuse.red = 0.5;
    scene->material.diffuse.green = 0.5;
    scene->material.diffuse.blue = 0.5;

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
	
    draw_maze(&scene);
    draw_model(&(scene->cube));
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}

void draw_maze(const Scene* scene)
{
    glLineWidth(1.0f); 
	
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

glBegin(GL_QUADS);
//glColor3f(1, 0, 0);
//floor
glVertex3f(-3, 2, 0);
glVertex3f(3, 2, 0);
glVertex3f(3, -2, 0);
glVertex3f(-3, -2, 0);

    // Behind wall
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3, 2, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3, 2, 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3, -2, 1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3, -2, 0);

    // Right wall
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3, -2, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3, -2, 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1, -2, 1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1, -2, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, -2, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(3, -2, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(3, -2, 0);

//left
glTexCoord2f(0.0f, 0.0f);
glVertex3f(-1, 2, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(-1, 2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(3, 2, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(3, 2, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(-3, 2, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(-3, 2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(-2, 2, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(-2, 2, 0);

//front
glTexCoord2f(0.0f, 0.0f);
glVertex3f(3, 2, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(3, 2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(3, -2, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(3, -2, 0);

//inside
glTexCoord2f(0.0f, 0.0f);
glVertex3f(1, -2, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(1, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(1, -1, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(1, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(1, -1, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(1, -1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(-1, -1, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(-1, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(-1, -1, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(-1, -1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(-1, 1, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(-1, 1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(-1, 1, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(-1, 1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2, 1, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, 1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, 1, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, 1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2, 0, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, 0, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, 0, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, 0, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(0, 0, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(0, 0, 0);

//small
glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, -2, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2, -1, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2.1, -2, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2.1, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2.1, -1, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2.1, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, -1, 0);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, -1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2.1, -1, 1);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2.1, -1, 0);

glEnd();
//glDisable(GL_TEXTURE_2D);
}
