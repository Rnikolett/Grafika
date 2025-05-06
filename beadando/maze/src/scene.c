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
    scene->cube_texture = load_texture("assets/textures/cube.png");

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


void render_scene(const Scene* scene)
{

    
    glShadeModel(GL_SMOOTH);
    set_material(&(scene->material));
    set_lighting();
    draw_origin();
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    draw_maze(&scene);
    
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
    // Display the counter
    char counter_text[32];
    sprintf(counter_text, "Collected Cubes: %d", scene->collected_cubes);
    
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
    // Enable texturing and bind the texture
    //glBindTexture(GL_TEXTURE_2D, scene->texture_id);
    glEnable(GL_TEXTURE_2D);

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

