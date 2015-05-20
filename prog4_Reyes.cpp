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
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "iostream"
using namespace std;

static int window;
static int menu_id;
static int submenu_id;
static int value = 0;

bool solid = false;
static bool proj = true;
static int shoulder = 0, elbow = 0, wrist = 0;
int xrot = 0, yrot = 0;


void menu(int num) {
    if (num == 0) {
        glutDestroyWindow(window);
    }
    else {
        value = num;
    }
    glutPostRedisplay();
}

void createMenu(void) {
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("w", 1);
    glutAddMenuEntry("W", 2);
    glutAddMenuEntry("h", 3);
    glutAddMenuEntry("p", 4);
    glutAddMenuEntry("x", 5);
    glutAddMenuEntry("X", 6);
    glutAddMenuEntry("y", 7);
    glutAddMenuEntry("Y", 8);
    glutAddMenuEntry("s", 9);
    glutAddMenuEntry("S", 10);
    glutAddMenuEntry("quit", 11);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)

            break;
    default:
        break;
    }
}



void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
    float a = 25;

    if (w <= h)
        glViewport (0, (h-w)/2, w, w);
    else
        glViewport ((w-h)/2, 0, h, h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-a, a, -a, a, -1, 1);
    glMatrixMode(GL_MODELVIEW);


    /*
    	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();


    	if (w <= h)
    		 glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, 100*nRange);
    	else
    		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, 100*nRange);

    	glMatrixMode(GL_MODELVIEW);*/
    glLoadIdentity();

}

void reshape2(int w, int h)
{
    float nRange = 25;

    if (w <= h)
        glViewport (0, (h-w)/2, w, w);
    else
        glViewport ((w-h)/2, 0, h, h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-nRange, nRange, -nRange * h / w, nRange*h / w, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float x = xrot, y = yrot, z = 0, centerx = -2*xrot, centery = -yrot, centerz = -1000000000, upx = 0, upy = 1, upz = 0;

    gluLookAt(x, y, z, centerx, centery, centerz, upx, upy, upz);

}

void display(void)
{
    if (solid) {                           //toggle solid/wireframe
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
        glClear(GL_COLOR_BUFFER_BIT);

    if (proj) {                            //toggle ortho/frustum
        reshape(500, 500);
    }
    else
        reshape(500, 500);


    glPushMatrix();
    glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);   //Shoulder
    glTranslatef(2.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(5.0, 1.0, 1.0);
    //blue
    glColor3d(0, 0, 1);
    if (!solid)
        glutWireCube(1.0);
    else
        glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(3.0, 0.0, 0.0);
    glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);      //Elbow
    glTranslatef(1.5, 0.0, 0.0);
    glPushMatrix();
    glScalef(4.0, 1.0, 1.0);
    //green
    glColor3d(0, 1, 0);
    if (!solid)
        glutWireCube(1.0);
    else
        glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(2.5, 0.0, 0.0);
    glRotatef((GLfloat)wrist, 0.0, 0.0, 1.0);      //Wrist
    glTranslatef(.50, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 1.0, 1.0);
    //red
    glColor3d(1, 0, 0);
    if (!solid)
        glutWireCube(1.0);
    else
        glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(1, -.35, 0.0);
    glRotatef(0, 0.0, 0.0, 1.0);       //Finger 1
    glTranslatef(.50, 0.0, 0.0);
    glPushMatrix();
    glScalef(1, .25, 1.0);
    //white
    glColor3d(1, 1, 1);
    if (!solid)
        glutWireCube(1.0);
    else
        glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0, .6, 0.0);
    glRotatef(0, 0.0, 0.0, 1.0);       //Finger 2
    glTranslatef(0, 0.0, 0.0);
    glPushMatrix();
    glScalef(1.0, .25, 1.0);
    //white
    glColor3d(1, 1, 1);
    if (!solid)
        glutWireCube(1.0);
    else
        glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(-1, .7, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);       //Finger 3
    glTranslatef(0, 0.0, 0.0);
    glPushMatrix();
    glScalef(.8, .25, 1.0);
    //white
    glColor3d(1, 1, 1);
    if (!solid)
        glutWireCube(1.0);
    else
        glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();

    //glClear(GL_COLOR_BUFFER_BIT);
    if (value == 1) {
        wrist = (wrist + 5) % 360;
        glutPostRedisplay();
    }
    else if (value == 2) {
        wrist = (wrist - 5) % 360;
        glutPostRedisplay();
    }
    else if (value == 3) {
        if (solid)
            solid = false;
        else
            solid = true;
        glutPostRedisplay();
    }
    else if (value == 4) {
        if (proj)
            proj = false;
        else
            proj = true;
        glutPostRedisplay();
    }
    else if (value == 5) {
        xrot = (xrot + 2) % 360;
        glutPostRedisplay();
    }
    else if (value == 6) {
        xrot = ((xrot - 2) % 360);
        glutPostRedisplay();
    }
    else if (value == 7) {
        yrot = (yrot + 1) % 360;
        glutPostRedisplay();
    }
    else if (value == 8) {
        yrot = (yrot - 1) % 360;
        glutPostRedisplay();
    }
    else if (value == 9) {
        shoulder = (shoulder + 5) % 360;
        glutPostRedisplay();
    }
    else if (value == 10) {
        shoulder = (shoulder + 5) % 360;
        glutPostRedisplay();
    }
    else if (value == 11) {
        exit(0);
    }

    value = 0;
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 's': /* s key rotates at shoulder */
        shoulder = (shoulder + 5) % 360;
        glutPostRedisplay();
        break;
    case 'S':
        shoulder = (shoulder - 5) % 360;
        glutPostRedisplay();
        break;
    case 'e': /* e key rotates at elbow */
        elbow = (elbow + 5) % 360;
        glutPostRedisplay();
        break;
    case 'E':
        elbow = (elbow - 5) % 360;
        glutPostRedisplay();
        break;
    case 'w': /* e key rotates at elbow */
        wrist = (wrist + 5) % 360;
        glutPostRedisplay();
        break;
    case 'W':
        wrist = (wrist - 5) % 360;
        glutPostRedisplay();
        break;
    case 'h':
        if (solid)
            solid = false;
        else
            solid = true;
        glutPostRedisplay();
        break;
    case 'p':
        if (proj)
            proj = false;
        else
            proj = true;
        glutPostRedisplay();
        break;
    case 'x':
        xrot = (xrot + 2) % 360;
        glutPostRedisplay();
        break;
    case 'X':
        xrot = ((xrot - 2) % 360);
        glutPostRedisplay();
        break;
    case 'y':
        yrot = (yrot + 1) % 360;
        glutPostRedisplay();
        break;
    case 'Y':
        yrot = (yrot - 1) % 360;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jonathan Reyes Robot");
    createMenu();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
