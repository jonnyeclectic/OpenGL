/*
Programmer:     Jonathan Reyes
    Date:           May 5, 2015
        Description:    Displays the graph of a piecewise, cubic bezier curve.
            Compile:        g++ prog7_Reyes.cpp -lglut -lGL -lGLU
                Execute:        ./a.out
                    Controls: WASD  (directions)
                        z,Z   (zoom)
                        Esc   (exit)
                        *Captial Letter of color (control of point)
                        EX: To control the white point, hold "Shift" and press "w"
                            You can move the white point with WASD controls or with the menu.
                        */
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
# else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <math.h>
#include <algorithm>
#define CTRL_PTS 7
static GLfloat mouse_x,mouse_y;
static GLint window, menu_id, submenu_id;
static GLint W, H, value = 0;
static GLfloat fovy = 0, zoom = 1;
static GLfloat
x1 = -4.0, y_1 = -4.0, z1 = 0.0,
x2 = -2.0, y2 = 4.0, z2 = 0.0,
x3 = 0.0, y3 = 4.0, z3 = 0.0,
x4 = 0.0, y4 = 0.0, z4 = 0.0,
x5 = 0.0, y5 = -4.0, z5 = 0.0,
x6 = 2.0, y6 = -4.0, z6 = 0.0,
x7 = 4.0, y7 = 4.0, z7 = 0.0;

static GLfloat ctrlpoints[CTRL_PTS][3] = {
    { x1, y_1, z1}, { x2, y2, z2}, { x3, y3, z3}, { x4, y4, z4}, { x5, y5, z5},
    {x6, y6, z6}, {x7, y7, z7}
};

void init(void)
{
    GLfloat ctrlpointsCOPY[CTRL_PTS][3] = {
        { x1, y_1, z1}, { x2, y2, z2}, { x3, y3, z3}, { x4, y4, z4}, { x5, y5, z5},
        {x6, y6, z6}, {x7, y7, z7}
    };
    std::copy(ctrlpointsCOPY, ctrlpointsCOPY + CTRL_PTS, ctrlpoints);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, CTRL_PTS, &ctrlpointsCOPY[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
}
void menu(int num) {
    if (num == 0)
        glutDestroyWindow(window);
    else
        value = num;
    glutPostRedisplay();
}
void createMenu(void) {
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Temporarily disabled. Take control of the following:", 0);
    glutAddMenuEntry("White", 1);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Yellow", 4);
    glutAddMenuEntry("Blue", 5);
    glutAddMenuEntry("Magenta", 6);
    glutAddMenuEntry("Cyan", 7);
    glutAddMenuEntry("Zoom In", 8);
    glutAddMenuEntry("Zoom Out", 9);
    glutAddMenuEntry("Left", 10);
    glutAddMenuEntry("Up", 11);
    glutAddMenuEntry("Down", 12);
    glutAddMenuEntry("Right", 13);
    glutAddMenuEntry("Escape", 14);
    glutAddMenuEntry("Collinear Mode", 15);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void reshape(int w, int h) {
    W = w;
    H = h;
    GLint nRange = 5;
    //constant aspect ratio
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w<=h) {
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -1, 1);
    } else {
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -1, 1);
    }

    if(fovy > 180)
        fovy = 180;
    else if(fovy <=180)
        fovy = zoom;

    //perspective projection
    gluPerspective(fovy, (GLfloat) w/(GLfloat) h, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef( zoom, zoom, 0);
}
GLfloat *X, *Y, *Z;
bool co = false;
void keyboard(unsigned char key, int x, int y) {
    int yrot,xrot, z;
    switch (key) {
    case 'z':
        /* zooms out */
        zoom -= .05;
        break;
    case 'Z':
        /* zooms in */
        zoom += .05;
        break;
    case 'd':
        /* rotates right */
        if(!co)
            *X += 1;
        else
        {
            x5 += 1;
            x3 += 1;
            x4 += 1;
        }
        break;
    case 'a':
        /* rotates left */
        if(!co)
            *X -= 1;
        else
        {
            x5 -= 1;
            x3 -= 1;
            x4 -= 1;
        }
        break;
    case 'w':
        /* rotates up */
        if(!co)
            *Y += 1;
        else
        {
            y5 += 1;
            y3 += 1;
            y4 += 1;
        }
        break;
    case 's':
        /* rotates down */
        if(!co)
            *Y -= 1;
        else
        {
            y5 -= 1;
            y3 -= 1;
            y4 -= 1;
        }
        break;
    case 'W':
        /* WHITE */
        co = false;
        X = &x1;
        Y = &y_1;
        Z = &z1;
        break;
    case 'C':
        /* CYAN*/
        co = false;
        X = &x7;
        Y = &y7;
        Z = &z7;
        break;
    case 'B':
        /* BLUE */
        co = true;
        X = &x5;
        Y = &y5;
        Z = &z5;
        break;
    case 'G':
        /* GREEN */
        co = true;
        X = &x3;
        Y = &y3;
        Z = &z3;
        break;
    case 'Y':
        /* YELLOW */
        co = true;
        X = &x4;
        Y = &y4;
        Z = &z4;
        break;
    case 'R':
        /* RED */
        co = false;
        X = &x2;
        Y = &y2;
        Z = &z2;
        break;
    case 'M':
        /* MAGENTA */
        co = false;
        X = &x6;
        Y = &y6;
        Z = &z6;
        break;
    case 27:
        exit(0);
    default:
        break;
    }
    init();
    reshape(W, H);
    glutPostRedisplay();
}

int userCo = -1;
void menuOption(int z) {
    int yrot,xrot;
    if( z != 0 && userCo == 1)
        value = z;

    switch (value) {
    case 9:
        /* zooms out */
        zoom -= .05;
        break;
    case 8:
        /* zooms in */
        zoom += .05;
        break;
    case 13:
        /* rotates right */
        if(!co)
            *X += 1;
        else
        {
            x5 += 1;
            x3 += 1;
            x4 += 1;
            co = false;
        }
        break;
    case 10:
        /* rotates left */
        if(!co)
            *X -= 1;
        else
        {
            x5 -= 1;
            x3 -= 1;
            x4 -= 1;
            co = false;
        }
        break;
    case 11:
        /* rotates up */
        if(!co)
            *Y += 1;
        else
        {
            y5 += 1;
            y3 += 1;
            y4 += 1;
            co = false;
        }
        break;
    case 12:
        /* rotates down */
        if(!co)
            *Y -= 1;
        else
        {
            y5 -= 1;
            y3 -= 1;
            y4 -= 1;
            co = false;
        }
        break;
    case 1:
        /* WHITE */
        X = &x1;
        Y = &y_1;
        Z = &z1;
        break;
    case 7:
        /* CYAN*/
        X = &x7;
        Y = &y7;
        Z = &z7;
        break;
    case 5:
        /* BLUE */
        co = true;
        X = &x5;
        Y = &y5;
        Z = &z5;
        break;
    case 3:
        /* GREEN */
        co = true;
        X = &x3;
        Y = &y3;
        Z = &z3;
        break;
    case 4:
        /* YELLOW */
        co = true;
        X = &x4;
        Y = &y4;
        Z = &z4;
        break;
    case 2:
        /* RED */
        X = &x2;
        Y = &y2;
        Z = &z2;
        break;
    case 6:
        /* MAGENTA */
        X = &x6;
        Y = &y6;
        Z = &z6;
        break;
    case 14:
        exit(0);
    case 15:
        userCo *= -1;
    default:
        break;
    }
    co = true;
    init();
    reshape(W, H);
    glutPostRedisplay();
    glutPostRedisplay();
    reshape(W, H);
    value = 0;
}
void display(void)
{
    //menuOption(0);
    GLint alias = 1000;
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= alias; i++)
        glEvalCoord1f((GLfloat) i/alias);
    glEnd();
    glPointSize(10.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < CTRL_PTS; i++) {
        glVertex3fv(&ctrlpoints[i][0]);
        glColor3f(cos(mouse_x),sin(mouse_y),cos(sin(mouse_x)));/*
        switch (false) {
            case 0:
            glColor3f(1.0, 0.0, 0.0);           //red
            break;
            case 1:
            glColor3f(0.0, 1.0, 0.0);           //green
            break;
            case 2:
            glColor3f(1.0, 1.0, 0.0);           //yellow
            break;
            case 3:
            glColor3f(0.0, 0.0, 1.0);           //blue
            break;
            case 4:
            glColor3f(1.0, 0.0, 1.0);           //magenta
            break;
            case 5:
            glColor3f(0.0, 1.0, 1.0);           //cyan
            break;
            case 6:
            glColor3f(1.0, 1.0, 1.0);           //white
            break;
            default:
            break;
        }*/
    }
    glEnd();
    glFlush();
}
void My_mouse_routine(int x, int y)
{

    mouse_x = x; //place current mouse pos in mouse_x
    mouse_y = y;

}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Bezier Space Curve");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc (keyboard);
    createMenu();
    glutPassiveMotionFunc( My_mouse_routine );
    glutMainLoop();
    return 0;
}
