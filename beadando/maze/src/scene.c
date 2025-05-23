#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>

#define MAZE_W 10
#define MAZE_H 10
#define FOV 60.0
#define NUM_RAYS 120
#define PI 3.1415926535


void init_scene(Scene* scene)
{
	//background color
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	
    //load_model(&(scene->cube), "assets/models/cube.obj");
    //scene->texture_id = load_texture("assets/textures/cube.png");
	load_model(&(scene->cube), "assets/models/cube.obj");
    scene->texture_id = load_texture("assets/textures/brickTexture.png");
    scene->cube_texture = load_texture("assets/textures/cube.jpg");
    scene->help_texture = load_texture("assets/textures/help.png");  // Load help texture
    scene->floor_texture = load_texture("assets/textures/floor.jpg");

	glEnable(GL_TEXTURE_2D);
	//scene->texture_id = load_texture("assets/textures/cube.png");
    // Initialize counter
    scene->collected_cubes = 0;
    
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

    scene->light_intensity = 1.0f;
    scene->help_visible = false;  // Initialize help visibility
}

void set_lighting(Scene* scene)
{
    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 0.0f, 0.0f, 10.0f, 1.0f };

    // Scale the diffuse light by the intensity
    diffuse_light[0] *= scene->light_intensity;
    diffuse_light[1] *= scene->light_intensity;
    diffuse_light[2] *= scene->light_intensity;

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

// Helper function to check if camera is near a cube
int is_camera_near_cube(const Camera* camera, float cube_x, float cube_y, float cube_z, float threshold)
{
    float dx = camera->position.x - cube_x;
    float dy = camera->position.y - cube_y;
    float dz = camera->position.z - cube_z;
    float distance = sqrt(dx*dx + dy*dy + dz*dz);
    return distance < threshold;
}

void update_scene(Scene* scene)
{
    
}

void render_help_image(const Scene* scene)
{
    if (!scene->help_visible) return;

    glPushMatrix();
    
    // Disable lighting for the help image
    glDisable(GL_LIGHTING); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1.0f, 1.0f, 0, -1.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, scene->help_texture);
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(1, 0);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(0, 1); glVertex2f(0, 1);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/*
void render_help_image(const Scene* scene)
{
    //testing on a wall
    if (!scene->help_visible) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->help_texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(1, -1, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1, -1, 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2, -1, 1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2, -1, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);

}*/

void render_scene(const Scene* scene)
{
    glShadeModel(GL_SMOOTH);
    set_material(&(scene->material));
    set_lighting(scene);
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    draw_maze();
    glBindTexture(GL_TEXTURE_2D, scene->floor_texture);
    draw_maze_floor();
    draw_maze_ceiling();
    
    // Apply rotation around the object's center
    double current_time = (double)SDL_GetTicks() / 1000;
    double rotation_angle = current_time * 90.0; // Rotate 90 degrees per second
    
    // Define cube positions
    float cube_positions[][3] = {
        {2.5, -1.5, 0.5},  // Cube 1
        {1.5, 0.5, 0.5},   // Cube 2
        {0.5, -1.5, 0.5}   // Cube 3
    };
    
    // Draw each cube
    for (int i = 0; i < 3; i++) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, scene->cube_texture);
        // Save the current matrix
        glPushMatrix();
        // Move to the cube's position
        glTranslatef(cube_positions[i][0], cube_positions[i][1], cube_positions[i][2]);
        glScalef(0.1, 0.1, 0.1);
        glRotatef(rotation_angle, 1, 1, 0);
        // Draw the model
        draw_model(&(scene->cube));
        
        // Restore the previous matrix
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    // Render help image if visible
    render_help_image(scene);
}


void draw_maze_floor()
{
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3, 2, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3, 2, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3, -2, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3, -2, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);

}

void draw_maze_ceiling()
{
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3, 2, 1);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(3, 2, 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(3, -2, 1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3, -2, 1);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
void draw_maze()
{
    glLineWidth(1.0f); 
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);

glBegin(GL_QUADS);
    // Behind wall
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3, 2, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3, 2, 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-3, -2, 1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-3, -2, 0);

    // Right wall
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-3, -2, 0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-3, -2, 1);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1, -2, 1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1, -2, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, -2, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(3, -2, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(3, -2, 0);

//left
glTexCoord2f(0.0f, 0.0f);
glVertex3f(-1, 2, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(-1, 2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(3, 2, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(3, 2, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(-3, 2, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(-3, 2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(-2, 2, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(-2, 2, 0);

//front
glTexCoord2f(0.0f, 0.0f);
glVertex3f(3, 2, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(3, 2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(3, -2, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(3, -2, 0);

//inside
glTexCoord2f(0.0f, 0.0f);
glVertex3f(1, -2, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(1, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(1, -1, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(1, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(1, -1, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(1, -1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(-1, -1, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(-1, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(-1, -1, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(-1, -1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(-1, 1, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(-1, 1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(-1, 1, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(-1, 1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2, 1, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, 1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, 1, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, 1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2, 0, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, 0, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, 0, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, 0, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(0, 0, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(0, 0, 0);

//small
glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, -2, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2, -1, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2.1, -2, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2.1, -2, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2.1, -1, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2.1, -1, 0);

glTexCoord2f(0.0f, 0.0f);
glVertex3f(2, -1, 0);
glTexCoord2f(0.0f, 1.0f);
glVertex3f(2, -1, 1);
glTexCoord2f(1.0f, 1.0f);
glVertex3f(2.1, -1, 1);
glTexCoord2f(1.0f, 0.0f);
glVertex3f(2.1, -1, 0);

glEnd();

glDisable(GL_COLOR_MATERIAL);
glDisable(GL_TEXTURE_2D);
}

