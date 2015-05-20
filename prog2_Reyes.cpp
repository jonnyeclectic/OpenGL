/*
  Programmer: Jonathan Reyes
  Class: CSCE 4230
  Date: February 19, 2015
  File Description:  OpenGL pendulum with right-click menu and keyboard interface
  Extra Credit:
  * 	Users can increase/decrease speed with 's' and 'S'
  * 	Users can increase/decrease wire length with 'l' and 'L'
*/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#define HEX 6

GLfloat angle= -M_PI/2.0;	//Have bob hanging straight down
GLfloat inc = .05;			//speed of pendulum
GLfloat dir = 1;			//Direction of pendulum swing (clockwise = -1; counterclowise = 1)

int window;
int menu_id;
int submenu_id;
int value;


float r = 1.0;
float l = 8.0;
float a = 1.3*l;
void spinDisplay(void)
{
    angle += (inc * dir);			//Angle is a factor of increment and direction
    glutPostRedisplay();
}

void menu(int num) {
    if (num == 0) {
        glutDestroyWindow(window);
    }
    else {
        value = num;				//Menu choice
    }
    glutPostRedisplay();
}

void createMenu(void) {
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("counterclockwise", 1);
    glutAddMenuEntry("clockwise", 2);
    glutAddMenuEntry("stop", 3);
    glutAddMenuEntry("quit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void hexagon(float swingX, float swingY) // Draw the pendulum hexagon
{
    glBegin(GL_TRIANGLE_FAN);

    for(int i = 0; i < HEX; i++)
    {
        glVertex2f(swingX+cos((i*M_PI)/3.0), swingY + sin((i*M_PI)/3.0));
    }

    glEnd();
}

void wire(double angle) { 				// Draw the pendulum wire
    float swingx = l * cos(angle);
    float swingy = l * sin(angle);
    float pixelWidth = 2.0;
    hexagon(swingx, swingy);
    glPointSize(pixelWidth);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(swingx, swingy);
    glEnd();

}

void display(void)
{
    float pixelWidth = 5.0;
    glMatrixMode (GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glPointSize(pixelWidth);
    glBegin(GL_POINTS);					//Point
    glColor3f(1.0, 1.0, 1.0);			//White
    glVertex2f(0, 0);					//Anchor origin
    glEnd();

    glColor3f(1, 1, 1);
    wire(angle);					//Draw wire and hexagon bob

    glPopMatrix();

    glBegin(GL_LINES);					//Point
    glColor3f(1.0, 0.0, 1.0);			//White
    //glVertex2f(0, 0);					//Anchor origin

    for(int i = 0; i < 1024; i++)
    {
        float theta = 2.0f * 3.1415926f * i / 1024; //the current angle

        float x = (1/1024)*i * cosf(theta); //the x component
        float y = (1/1024)*i * sinf(theta); //the y component

        glVertex2f(x, y);

    }


    glEnd();
    glutSwapBuffers();



    switch(value) {			//menu choice
    case 1:				//counterclockwise
        glutIdleFunc(spinDisplay);
        dir = 1;
        break;
    case 2:				//clockwise
        glutIdleFunc(spinDisplay);
        dir = -1;
        break;
    case 3:				//stop
        value = 0;		//reset menu choice to stop so other inputs aren't overwritten
        glutIdleFunc(NULL);
        break;
    case 4:				//quit
        exit(0);
    default:
        break;
    }

}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}

void reshape(int w, int h)
{
    if (w <= h)
        glViewport (0, (h-w)/2, w, w);
    else
        glViewport ((w-h)/2, 0, h, h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-a, a, -a, a, -1, 1);
    glMatrixMode(GL_MODELVIEW);

}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            glutIdleFunc(spinDisplay);
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
        break;
    default:
        break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27: //escape
        exit(0);
        break;
    case 32: //space
        value = 0;
        glutIdleFunc(NULL);
        break;
    case 45: //minus
        value = 0;
        glutIdleFunc(spinDisplay);
        dir = -1;
        break;
    case 43: //plus
        value = 0;
        glutIdleFunc(spinDisplay);
        dir = 1;
        break;
    case 108: //letter l
        value = 0;
        glutPostRedisplay();
        l += 1;
        break;
    case 76:  //letter L
        value = 0;
        glutPostRedisplay();
        l -= 1;
        break;
    case 115: //letter s
        value = 0;
        glutPostRedisplay();
        inc += .05;
        break;
    case 83: //letter S
        value = 0;
        glutPostRedisplay();
        inc -= .05;
        break;
    default:
        break;

    }
}

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
    glutCreateWindow(" Jonathan Reyes Pendulum");
    //glutFullScreen();
    createMenu();
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
