#include "camera.h"

#include <GL/gl.h>

#include <math.h>

// Wall collision detection
int check_wall_collision(float x, float y) {
    // Define wall boundaries
    float wall_thickness = 0.1f;
    
    // Check if position is outside the maze boundaries + 1
    if (x < -4.0f - wall_thickness || x > 4.0f + wall_thickness ||
        y < -3.0f - wall_thickness || y > 3.0f + wall_thickness) {
        return 1;
    }
    
    // Check specific wall segments (outside)
    if (x < -2.9f && x > -3.0f && y > -2.0f && y < 2.0f) return 1; // Left wall ok
	if (x > 2.9f && x < 3.0f && y > -2.0f && y < 2.0f) return 1; // Right wall ok
    if (x > 2.0f && x < 2.1f && y > -2.0f && y < -1.0f) return 1;   // Right wall ok
	if (x > 2.0f && x < 2.1f && y > -0.0f && y < 1.0f) return 1;   // Right wall ok
    if (y < -1.9f && y > -2.0f && x > -3.0f && x < 1.0f) return 1; // Bottom wall (left) ok
	if (y < -1.9f && y > -2.0f && x > 2.0f && x < 3.0f) return 1; // Bottom wall (right) ok
    if (y > 1.9f && y < 2.0f && x > -1.0f && x < 3.0f) return 1;   // Top wall (right) ok
	if (y > 1.9f && y < 2.0f && x > -3.0f && x < -2.0f) return 1;   // Top wall (right) ok
    
    // Check internal walls
	// Vertical walls
	if (x > -1.1f && x < -1.0f && y > -1.0f && y < 1.0f) return 1;
    if (x > 0.9f && x < 1.0f && y > -2.0f && y < -1.0f) return 1;  
	if (x > 1.9f && x < 2.0f && y > -2.0f && y < -1.0f) return 1;
	if (x > 1.9f && x < 2.0f && y > 0.0f && y < 1.0f) return 1;
	// Horizontal wall
    if (y > -1.1f && y < -1.0f && x > -1.0f && x < 1.0f) return 1; //bottom
	if (y > -0.1f && y < 0.0f && x > 0.0f && x < 2.0f) return 1; //mid
	if (y > 0.9f && y < 1.0f && x > -1.0f && x < 2.0f) return 1; //top
    
    return 0;
}

int check_cube_collision(float x, float y){

    if (x > 2.5 - 0.2 && x < 2.5 + 0.2 && y > -1.5 - 0.2 && y < -1.5 + 0.2) return 1;
    if (x > 1.5 - 0.2 && x < 1.5 + 0.2 && y > 0.5 - 0.2 && y < 0.5 + 0.2) return 1;
    if (x > 0.5  - 0.2 && x < 0.5  + 0.2  && y > -0.5 - 0.2 && y < -0.5 + 0.2) return 1;
    else return 0;
}


void init_camera(Camera* camera)
{
    camera->position.x = 1.5;
    camera->position.y = -1.5;
    camera->position.z = 0.5;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 90.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;

    camera->is_preview_visible = false;
}

void update_camera(Camera* camera, double time)
{
    double angle;
    double side_angle;
    float cam_pos_x, cam_pos_y;

    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    // Calculate new position
    cam_pos_x = camera->position.x + cos(angle) * camera->speed.y * time;
    cam_pos_y = camera->position.y + sin(angle) * camera->speed.y * time;
    cam_pos_x += cos(side_angle) * camera->speed.x * time;
    cam_pos_y += sin(side_angle) * camera->speed.x * time;

    // Check for collisions before updating position
    if (!check_wall_collision(cam_pos_x, cam_pos_y)) {
        camera->position.x = cam_pos_x;
        camera->position.y = cam_pos_y;
    }

    
}

void set_view(const Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x < 0) {
        camera->rotation.x += 360.0;
    }

    if (camera->rotation.x > 360.0) {
        camera->rotation.x -= 360.0;
    }
}

void set_camera_speed(Camera* camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera* camera, double speed)
{
    camera->speed.x = speed;
}

void show_texture_preview()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 1, -3);
    glTexCoord2f(1, 0);
    glVertex3f(1, 1, -3);
    glTexCoord2f(1, 1);
    glVertex3f(1, -1, -3);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -1, -3);
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
