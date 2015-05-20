

#include "GL/glut.h"
#include "stdlib.h"
#define WIDTH 500
#define HEIGHT 500
#define POSX 100
#define POSY 100

double x1 = 0.0, y_1 = 0.0;
double x2 = 0.5, y_2 = 1.0;
double x3 = 1.0, y_3 = 0.0;
double x0 = 0.5, y_0 = 0.5;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glVertex2d(x1, y_1);
    glVertex2d(x2, y_2);
    glVertex2d(x3, y_3);

    double i, n = 60000, choice, px = 0.5, py = 0.5;
    for (i = 0; i < n; i++) {
        choice = (rand() % 3) + 1;
        if (choice == 1) {
            px = (x1+px)/2;
            py = (y_1+py)/2;
            glVertex2d(px, py);
        }
        else if (choice == 2) {
            px = (x2 + px) / 2;
            py = (y_2 + py) / 2;
            glVertex2d(px, py);
        }
        else {
            px = (x3 + px) / 2;
            py = (y_3 + py) / 2;
            glVertex2d(px, py);
        }
    }
    glEnd();
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(POSX, POSY);
    glutCreateWindow("Chaos Game");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
