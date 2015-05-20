/*
  Programmer: Jonathan Reyes (run with g++ instead of gcc)
  Class: CSCE 4230
  Date: March 5, 2015
  File Description:  OpenGL Pacman rave with right-click menu and keyboard interface
  Extra Credit:
  * 	Users can change speed with numbers 1,2, and 3 (slowest to fastest)
  * 	Users can stop/start the animation with the spacebar
  * 	Users can move "The Pacman" with W,S,A,D (up, down, left, right)
  * 	Users can reverse the direction of orbit by pressing the minus key ( - )
  * 	Pacman will play itself with 10 moves (duration variable) and then reverse those 10 moves to complete choreography
  * 	Users can press number 0 to toggle on/off automatic Pacman play.
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
#include <stdio.h>
#include <math.h>
#include <time.h>

#define HEX 6
#define duration 10

float Width, Height;
int confirm = 1, temp = 9, first = 0, override = 0, oldValue[duration], mutex = 0, counter = 0;
float swingX = 0;
float swingY = 0;
float pacX = 0;
float pacY = 0;
int frame = 0, alternate = 0, move = 0;
GLUquadricObj *qobj;

GLfloat angle= 0;			//Have bob hanging straight down
GLfloat inc = .1;			//speed of pendulum
GLfloat dir = 1;			//Direction of pendulum swing (clockwise = -1; counterclowise = 1)

int window;
int menu_id;
int submenu_id;
int value;



float l = 8.0;
float a = 1.3*15.0;
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
    glutAddMenuEntry("manual Pacman", 1);
    glutAddMenuEntry("reverse", 2);
    glutAddMenuEntry("start/stop", 3);
    glutAddMenuEntry("slow", 4);
    glutAddMenuEntry("normal", 5);
    glutAddMenuEntry("fast", 6);
    glutAddMenuEntry("left", 7);
    glutAddMenuEntry("up", 8);
    glutAddMenuEntry("right", 9);
    glutAddMenuEntry("down", 10);
    glutAddMenuEntry("quit", 11);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int flash(void)
{
    if(frame%30==0)
    {
        return 0;
    }
    else
        return 1;
}

void wedge(void)
{
    srand((float)time(NULL)+rand());
    if(flash()==0) {
        alternate = frame + 10;//duration;
    }

    if((alternate > frame && dir != 0 ) && (pacX != 0|| pacY != 0)) {
        glColor3f(0, 0, 0);
        if(pacY < 0)
            glTranslatef (6*pacX, 4*pacY, 0);
        else if(pacY > 0)
            glTranslatef (6*pacX, 6*pacY, 0);
        else
            glTranslatef (6*pacX, .15, 0);
        gluSphere(  qobj, 0.91, 3, 10 );
    }

    glColor3f(0, 1, 0);
    if(dir == 0)
        glTranslatef (0, 0, 0);
    else if(first == 0)
        glTranslatef (pacX+ .6,pacY+.1, 0);
    else
        glTranslatef (pacX,pacY, 0);

    first = 1;
    glPopMatrix();
}

void Pacmove()
{
    glColor3f(1, 1, 0);
    if(dir == 0)
        glTranslatef (0, 0, 0);
    else if(first == 0)
        glTranslatef (pacX+ .6,pacY+.1, 0);
    else
        glTranslatef (pacX,pacY, 0);
    gluSphere( qobj, 0.75, 15, 10 );
    glPopMatrix();
    glPushMatrix();
    wedge();

}

void morphPoly(float angle) // Draw the morphing polygon
{
    float swingx = (l/2) * cos(angle) + swingX;
    float swingy = (l/2) * sin(angle) + swingY;
    glColor3f(1-cos(angle), 1-sin(angle), 1-(sin(angle)+cos(angle)));
    glBegin(GL_TRIANGLE_FAN);
    int i;
    for(i = 0; i < HEX; i++)
    {
        glVertex2f(swingx+cos((i*M_PI)/3.0), swingy + sin((i*M_PI)/3.0));
    }

    glEnd();
    spinDisplay();
}

void hexagon(float angle) // Draw the pendulum hexagon
{
    swingX = (l) * cos(angle);
    swingY = (l) * sin(angle);
    glColor3f(1-sin(angle), 1-cos(angle), 1-(sin(angle)+cos(angle)));
    glBegin(GL_POLYGON);
    int i;
    for(i = 0; i < HEX*4; i++)
    {
        glVertex2f(swingX+cos((i*angle)/HEX), swingY + sin((i*angle)/HEX));
    }

    glEnd();
    spinDisplay();
}

void display(void)
{

    if(frame%40==0)
        move = 1;

    if(move == 1)
    {
        move = 0;

        if(oldValue[counter-1] == temp && override != 1)			//If the last move is a duplicate, change move.
            temp = ((temp - 6)%4) + 7;

        if(mutex != 1 && override != 1 && counter >= duration)	//If we have waited enough time
        {
            mutex = 1;
        }

        if(mutex == 1 && override != 1)
        {
            temp = oldValue[counter-1];
            if(temp == 7) //left
                temp = 9;
            else if(temp == 8) //up
                temp = 10;
            else if(temp == 9) //right
                temp = 7;
            else if(temp == 10) //down
                temp = 8;

            oldValue[counter-1] = -1;
            value = temp;
            if(counter-1 <= 0)
                mutex = 0;
            else
                mutex = 1;

            counter-=1;
        }
        else if(mutex == 0 && override != 1 && counter <= duration && confirm == 1)		//If we've waited, and the game has been in play..
        {
            oldValue[counter] = temp;
            mutex = 0;
            counter+=1;
            value = 0;

        }
        else
            confirm = 2;

        if(confirm != 2)
            value = temp;
        confirm = 0;
    }


    float pixelWidth = 5.0;
    frame++;
    glClear(GL_COLOR_BUFFER_BIT);
    qobj = gluNewQuadric();
    gluQuadricDrawStyle( qobj, GLU_FILL );
    glMatrixMode (GL_MODELVIEW);

    glPushMatrix();
    glRotatef(.5, 0, 0, 1);
    hexagon(angle/8);								//This is the hexagon in double orbit
    glPopMatrix();

    glPushMatrix();
    morphPoly(angle);								//This is the morphing polygon in double orbit
    glPopMatrix();

    glPushMatrix();									//This is the disc in the center
    glLoadIdentity();
    gluDisk(qobj, 0.75, 1, 100, 100);
    glColor3f(0, 0, 1);
    glTranslatef(0, 0, 1);
    gluDisk(qobj, 0.5, 0.75, 15, 15);
    glPopMatrix();
    gluDeleteQuadric(qobj);

    glPushMatrix();
    glLoadIdentity();
    glColor3f(1, 1, 1);
    glPointSize(pixelWidth);
    int i, spiral = -200;
    for(i = 0; i <= 200; i++)							//These are the spiral/spinning dotted lines in the background
    {
        glBegin(GL_POINTS);
        glVertex2d(spiral*cos(angle), spiral*sin(angle));
        glPopMatrix();
        glPushMatrix();
        glColor3f(0, .5, 1);
        glVertex2d(spiral*(cos(angle+1)), spiral*(sin(angle+1)));
        glPopMatrix();
        glPushMatrix();
        glColor3f(1, 1, 1);
        glVertex2d(spiral*(cos(angle+1.5)), spiral*(sin(angle+1.5)));
        spiral += 3;
        glEnd();
    }

    glPopMatrix();
    glPushMatrix();
    Pacmove();				//Pacman
    glPopMatrix();

    glutSwapBuffers();

    switch(value) {			//menu choice
    case 1: //zero
        value = 0;
        if(override == 0)
            override = 1;
        else {
            override = 0;
            value = 9;
        }
        break;
    case 2: //minus
        value = 0;
        if(dir == -1)
            dir = 1;
        else
            dir = -1;
        break;
    case 3: //space
        value = 0;
        if(dir == 0)
            dir = 1;
        else
            dir = 0;
        break;
    case 4: //number 1
        value = 0;
        inc = .05;
        break;
    case 5: //number 2
        value = 0;
        inc = .2;
        break;
    case 6: //number 3
        value = 0;
        inc = 1;
        break;
    case 7: //letter a
        confirm = 1;
        value = 0;
        pacX = -.1;
        pacY = 0;
        glutIdleFunc(Pacmove);
        break;
    case 8: //letter w
        confirm = 1;
        value = 0;
        pacY = .1;
        pacX = 0;
        glutIdleFunc(Pacmove);
        break;
    case 9: //letter d
        confirm = 1;
        value = 0;
        pacX = .1;
        pacY = 0;
        glutIdleFunc(Pacmove);
        break;
    case 10: //letter s
        confirm = 1;
        value = 0;
        pacY = -.1;
        pacX = 0;
        glutIdleFunc(Pacmove);
        break;
    case 11:				//quit
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
    Width = w;
    Height = h;
    glMatrixMode(GL_MODELVIEW);


    if(w <= h)
        glViewport(0, ((h-w)/2), (GLsizei)w, (GLsizei)w);
    else
        glViewport(((w-h)/2),0, (GLsizei)h, (GLsizei)h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (w <= h)
        gluOrtho2D (-a, a, -a, a * (GLfloat) w/(GLfloat) h);
    else
        gluOrtho2D (-a * (GLfloat) h/(GLfloat) w,a,-a,a);

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
    case 45: //minus
        value = 0;
        if(dir == -1)
            dir = 1;
        else
            dir = -1;
        break;
    case 27: //escape
        exit(0);
        break;
    case 32: //space
        value = 0;
        if(dir == 0)
            dir = 1;
        else
            dir = 0;
        break;
    case 48: //number 0
        value = 0;
        if(override == 0)
            override = 1;
        else {
            override = 0;
            value = 9;
        }
        break;
    case 49: //number 1
        value = 0;
        inc = .05;
        break;
    case 50: //number 2
        value = 0;
        inc = .2;
        break;
    case 51: //number 3
        value = 0;
        inc = 1;
        break;
    case 97: //letter a
        value = 0;
        pacX = -.1;
        pacY = 0;
        glutIdleFunc(Pacmove);
        break;
    case 119: //letter w
        value = 0;
        pacY = .1;
        pacX = 0;
        glutIdleFunc(Pacmove);
        break;
    case 100: //letter d
        value = 0;
        pacX = .1;
        pacY = 0;
        glutIdleFunc(Pacmove);
        break;
    case 115: //letter s
        value = 0;
        pacY = -.1;
        pacX = 0;
        glutIdleFunc(Pacmove);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv) {
    srand((float)time(NULL)+rand());
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(500, 500);
    glutCreateWindow(" Jonathan Reyes Pacman");
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
