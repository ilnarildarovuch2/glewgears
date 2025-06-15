// gcc main.c -lGLEW -lglfw -lGL -lm -o gears
// gcc -m32 main.c -o gears.exe -Iinclude -Llib -lglfw3 -lglew32 -lopengl32 -lgdi32

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef _WIN32
#include <windows.h>
#include <process.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define M_PI 3.14159265358979323846
#define ROTATE_SPEED 90.0f

static int window_width  = 500;
static int window_height = 480;

static GLfloat view_rotx = 20.0f, view_roty = 30.0f, view_rotz = 0.0f;
static GLuint  gear1, gear2, gear3;
static GLfloat angle = 0.0f;

// build a single gear into a display list
static void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
                 GLint   teeth,        GLfloat tooth_depth)
{
    GLint   i;
    GLfloat r0 = inner_radius;
    GLfloat r1 = outer_radius - tooth_depth/2.0f;
    GLfloat r2 = outer_radius + tooth_depth/2.0f;
    GLfloat da = 2.0f * M_PI / teeth / 4.0f;
    GLfloat angle0, u, v, len;

    glShadeModel(GL_FLAT);

    // Front face
    glNormal3f(0,0,1);
    glBegin(GL_QUAD_STRIP);
    for(i = 0; i <= teeth; i++) {
        angle0 = i * 2.0f * M_PI / teeth;
        glVertex3f(r0*cos(angle0), r0*sin(angle0),  width*0.5f);
        glVertex3f(r1*cos(angle0), r1*sin(angle0),  width*0.5f);
        glVertex3f(r0*cos(angle0), r0*sin(angle0),  width*0.5f);
        glVertex3f(r1*cos(angle0+3*da), r1*sin(angle0+3*da), width*0.5f);
    }
    glEnd();

    // Front sides of teeth
    glBegin(GL_QUADS);
    for(i = 0; i < teeth; i++) {
        angle0 = i * 2.0f * M_PI / teeth;
        glVertex3f(r1*cos(angle0),    r1*sin(angle0),    width*0.5f);
        glVertex3f(r2*cos(angle0+da), r2*sin(angle0+da), width*0.5f);
        glVertex3f(r2*cos(angle0+2*da), r2*sin(angle0+2*da), width*0.5f);
        glVertex3f(r1*cos(angle0+3*da), r1*sin(angle0+3*da), width*0.5f);
    }
    glEnd();

    // Back face
    glNormal3f(0,0,-1);
    glBegin(GL_QUAD_STRIP);
    for(i = 0; i <= teeth; i++) {
        angle0 = i * 2.0f * M_PI / teeth;
        glVertex3f(r1*cos(angle0), r1*sin(angle0), -width*0.5f);
        glVertex3f(r0*cos(angle0), r0*sin(angle0), -width*0.5f);
        glVertex3f(r1*cos(angle0+3*da), r1*sin(angle0+3*da), -width*0.5f);
        glVertex3f(r0*cos(angle0),      r0*sin(angle0),      -width*0.5f);
    }
    glEnd();

    // Back sides of teeth
    glBegin(GL_QUADS);
    for(i = 0; i < teeth; i++) {
        angle0 = i * 2.0f * M_PI / teeth;
        glVertex3f(r1*cos(angle0+3*da), r1*sin(angle0+3*da), -width*0.5f);
        glVertex3f(r2*cos(angle0+2*da), r2*sin(angle0+2*da), -width*0.5f);
        glVertex3f(r2*cos(angle0+da),   r2*sin(angle0+da),   -width*0.5f);
        glVertex3f(r1*cos(angle0),      r1*sin(angle0),      -width*0.5f);
    }
    glEnd();

    // Outward faces of teeth
    glBegin(GL_QUAD_STRIP);
    for(i = 0; i < teeth; i++) {
        angle0 = i * 2.0f * M_PI / teeth;
        glVertex3f(r1*cos(angle0), r1*sin(angle0),  width*0.5f);
        glVertex3f(r1*cos(angle0), r1*sin(angle0), -width*0.5f);

        u = r2*cos(angle0+da) - r1*cos(angle0);
        v = r2*sin(angle0+da) - r1*sin(angle0);
        len = sqrtf(u*u+v*v);  u/=len;  v/=len;
        glNormal3f(v, -u, 0);
        glVertex3f(r2*cos(angle0+da), r2*sin(angle0+da),  width*0.5f);
        glVertex3f(r2*cos(angle0+da), r2*sin(angle0+da), -width*0.5f);

        glNormal3f(cos(angle0), sin(angle0), 0);
        glVertex3f(r2*cos(angle0+2*da), r2*sin(angle0+2*da),  width*0.5f);
        glVertex3f(r2*cos(angle0+2*da), r2*sin(angle0+2*da), -width*0.5f);

        u = r1*cos(angle0+3*da) - r2*cos(angle0+2*da);
        v = r1*sin(angle0+3*da) - r2*sin(angle0+2*da);
        glNormal3f(v, -u, 0);
        glVertex3f(r1*cos(angle0+3*da), r1*sin(angle0+3*da),  width*0.5f);
        glVertex3f(r1*cos(angle0+3*da), r1*sin(angle0+3*da), -width*0.5f);
        glNormal3f(cos(angle0), sin(angle0), 0);
    }
    // close strip
    glVertex3f(r1*cos(0), r1*sin(0),  width*0.5f);
    glVertex3f(r1*cos(0), r1*sin(0), -width*0.5f);
    glEnd();

    glShadeModel(GL_SMOOTH);

    // Inside radius cylinder
    glBegin(GL_QUAD_STRIP);
    for(i = 0; i <= teeth; i++) {
        angle0 = i * 2.0f * M_PI / teeth;
        glNormal3f(-cos(angle0), -sin(angle0), 0);
        glVertex3f(r0*cos(angle0), r0*sin(angle0), -width*0.5f);
        glVertex3f(r0*cos(angle0), r0*sin(angle0),  width*0.5f);
    }
    glEnd();
}

// Create the three display lists
static void init_gears(void)
{
    GLfloat pos[4]   = { 5.0f,  5.0f, 10.0f, 1.0f };
    GLfloat red[4]   = { 0.8f,  0.1f,  0.0f, 1.0f };
    GLfloat green[4] = { 0.0f,  0.8f,  0.2f, 1.0f };
    GLfloat blue[4]  = { 0.2f,  0.2f,  1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    gear1 = glGenLists(1);
    glNewList(gear1, GL_COMPILE);
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
      gear(1.0f, 4.0f, 1.0f, 20, 0.7f);
    glEndList();

    gear2 = glGenLists(1);
    glNewList(gear2, GL_COMPILE);
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
      gear(0.5f, 2.0f, 2.0f, 10, 0.7f);
    glEndList();

    gear3 = glGenLists(1);
    glNewList(gear3, GL_COMPILE);
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
      gear(1.3f, 2.0f, 0.5f, 10, 0.7f);
    glEndList();
}

// Set up viewport & projection
static void reshape(int width, int height)
{
    window_width  = width;
    window_height = height;
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    {
        float fov    = 45.0f * (M_PI/180.0f);
        float aspect = (float)width/height;
        float zNear  = 1.0f, zFar = 60.0f;
        float f = 1.0f / tanf(fov/2.0f);
        float m[16] = {
            f/aspect, 0, 0,  0,
            0,        f, 0,  0,
            0,        0, (zFar+zNear)/(zNear-zFar), -1,
            0,        0, (2*zFar*zNear)/(zNear-zFar), 0
        };
        glMultMatrixf(m);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-20.0f);
}

// render the scene
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
      glRotatef(view_rotx, 1,0,0);
      glRotatef(view_roty, 0,1,0);
      glRotatef(view_rotz, 0,0,1);

      // Gear 1
      glPushMatrix();
        glTranslatef(-2.0f, -2.0f, 0.0f);
        glRotatef(angle, 0,0,1);
        glCallList(gear1);
      glPopMatrix();

      // Gear 2
      glPushMatrix();
        glTranslatef(4.1f, -2.0f, 0.0f);
        glRotatef(-2.0f*angle - 9.0f, 0,0,1);
        glCallList(gear2);
      glPopMatrix();

      // Gear 3
      glPushMatrix();
        glTranslatef(-2.1f, 4.2f, 0.0f);
        glRotatef(-2.0f*angle - 25.0f, 0,0,1);
        glCallList(gear3);
      glPopMatrix();
    glPopMatrix();
}
#ifndef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(void)
#endif
{
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    // Request an OpenGL 2.1 context (for display lists)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* win = glfwCreateWindow(window_width, window_height, "GlewGears", NULL, NULL);
    if(!win) {
        fprintf(stderr, "Failed to open window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(win);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 1;
    }

    glClearColor(0,0,0,1);
    init_gears();
    reshape(window_width, window_height);

    // vsync
    glfwSwapInterval(1);

    // initialize timing
    double last_time    = glfwGetTime();
    double current_time, delta_time;

    // main loop
    while(!glfwWindowShouldClose(win)) {
        // compute delta
        current_time = glfwGetTime();
        delta_time   = current_time - last_time;
        last_time    = current_time;

        // advance animation
        angle += ROTATE_SPEED * (float)delta_time;

        // draw
        display();
        glfwSwapBuffers(win);
        glfwPollEvents();

        // input
        if(glfwGetKey(win, GLFW_KEY_ESCAPE)==GLFW_PRESS)
            glfwSetWindowShouldClose(win, 1);
        if(glfwGetKey(win, GLFW_KEY_UP)==GLFW_PRESS)    view_rotx += 1.0f;
        if(glfwGetKey(win, GLFW_KEY_DOWN)==GLFW_PRESS)  view_rotx -= 1.0f;
        if(glfwGetKey(win, GLFW_KEY_LEFT)==GLFW_PRESS)  view_roty -= 1.0f;
        if(glfwGetKey(win, GLFW_KEY_RIGHT)==GLFW_PRESS) view_roty += 1.0f;
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}