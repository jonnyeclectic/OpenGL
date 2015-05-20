/*
Programmer:     Jonathan Reyes
Date:           April 9, 2015
Description:    Displays the graph of a bivariate function:
                f(x,y) =    .5*exp[-.04*sqrt((80x-40)^2 + (90y-45)^2)] *
                            cos[0.15*sqrt((80x-40)^2 + (90y-45)^2)]
Compile:        g++ prog5_Reyes.cpp -lglut -lGL -lGLU
Execute:        ./a.out
Controls: WASD  (directions)
          z,Z   (zoom)
          Esc   (exit)
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

#define PI M_PI
#define NUM_VERTICES 100
#define K NUM_VERTICES
#define nt 2*K*K
#define nv (K+1)*(K+1)
static GLint window;
static GLint menu_id;
static GLint submenu_id;
static GLint value = 0;
static GLint W = 0;
static GLint H = 0;
static GLint xrot = 0;
static GLint yrot = 0;
static GLint zoom = 35;
static GLint indv = 0;
static GLfloat fovy = 0;
static GLfloat v[nv][3], vn[nv][3];
static GLint ltri[nt][3];
void menu(int num) {
    if (num == 0)
        glutDestroyWindow(window);
    else
        value = num;
    glutPostRedisplay();
}
void createMenu(void) {
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Zoom In", 1);
    glutAddMenuEntry("Zoom Out", 2);
    glutAddMenuEntry("Rotate Right", 3);
    glutAddMenuEntry("Rotate Left", 4);
    glutAddMenuEntry("Rotate Up", 5);
    glutAddMenuEntry("Rotate Down", 6);
    glutAddMenuEntry("Escape", 7);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void normalize (GLfloat v[]) {
    GLfloat length = 0.0f;
    GLint i, n =3;
    for (i = 0; i < n; ++i) {
        length += v[i] * v[i];
    }
    length = sqrt(length);
    for (i = 0; i < n; ++i) {
        v[i] /= length;
    }
}
void BiVariateVertices() {
    //Store vertices
    GLfloat h = 1.0/K;
    for (int j=0; j<=K; j++) {
        GLfloat y = j*h;
        for (int i=0; i<=K; i++) {
            GLfloat x = i*h;
            // z = f(x,y)
            GLfloat z = 0.5*exp(-.04*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2))) *
                        cos(0.15*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2)));
            v[indv][0] = x;
            v[indv][1] = y;
            v[indv][2] = z;
            indv++;
        }
    }
    // Store triangles (triples of vertex indices)
    int indt = 0;
    for (int j=1; j<=K; j++) {
        for (int i=1; i<=K; i++) {
            indv = j*(K+1) + i;
            // indv indexes the upper right corner of a cell
            ltri[indt][0] = indv-K-2;
            ltri[indt][1] = indv-K-1;
            ltri[indt][2] = indv;
            ltri[indt+1][0] = indv-K-2;
            ltri[indt+1][1] = indv;
            ltri[indt+1][2] = indv-1;
            indt += 2;
        }
    }
    // Initialize normals
    for (indv=0; indv<=nv-1; indv++) {
        vn[indv][0] = 0;
        vn[indv][1] = 0;
        vn[indv][2] = 0;
    }
    // Add triangle normals to vector normals
    for (indt=0; indt<=nt-1; indt++) {
        GLint i1 = ltri[indt][0],
              i2 = ltri[indt][1],
              i3 = ltri[indt][2];
        GLfloat tn[3];
        tn[0] = (v[i2][1]-v[i1][1]) * (v[i3][2]-v[i1][2]) -
                (v[i2][2]-v[i1][2]) * (v[i3][1]-v[i1][1]);
        tn[1] = (v[i2][2]-v[i1][2]) * (v[i3][0]-v[i1][0]) -
                (v[i2][0]-v[i1][0]) * (v[i3][2]-v[i1][2]);
        tn[2] = (v[i2][0]-v[i1][0]) * (v[i3][1]-v[i1][1]) -
                (v[i2][1]-v[i1][1]) * (v[i3][0]-v[i1][0]);
        normalize (tn);
        for (int i=0; i<3; i++) {
            vn[i1][i] += tn[i];
            vn[i2][i] += tn[i];
            vn[i3][i] += tn[i];
        }
    }
    // Normalize vector normals
    for (indv=0; indv<=nv-1; indv++)
        normalize (vn[indv]);
}
void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    //smooth shading and lighting
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    BiVariateVertices();
}
void reshape(int w, int h) {
    W = w;
    H = h;
    GLint nRange = 2;
    //constant aspect ratio
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    if(w<=h) {
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -1, 1);
    } else {
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -1, 1);
    }
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if(fovy > 180)
        fovy = 180;
    else if(fovy <=180)
        fovy = zoom;
    //perspective projection
    gluPerspective(fovy, (GLfloat) w/(GLfloat) h, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -2.0);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glTranslatef(-0.5, -0.5, 0.0);

}
void menuOption(void) {
    if (value == 1) {
        /* Zoom In */
        zoom -= 1;
    } else if (value == 2) {
        /* Zoom Out */
        zoom += 1;
    } else if (value == 3) {
        /* Rotate Right */
        yrot = (yrot + 5) % 360;
    } else if (value == 4) {
        /* Rotate Left */
        yrot = (yrot - 5) % 360;
    } else if (value == 5) {
        /* Rotate Up */
        xrot = (xrot - 5) % 360;
    } else if (value == 6) {
        /* Rotate Down */
        xrot = (xrot + 5) % 360;
    } else if (value == 7) {
        /* Escape */
        exit(0);
    }
    glutPostRedisplay();
    reshape(W, H);
    value = 0;
}
void display(void) {
    menuOption();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    glVertexPointer (3, GL_FLOAT, 0, v);
    glNormalPointer(GL_FLOAT, 0, vn);
    glDrawElements(GL_TRIANGLES, nt*3, GL_UNSIGNED_INT, ltri);
    glPopMatrix();
    //smooth motion
    glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'z':
        /* zooms in */
        zoom -= 1;
        break;
    case 'Z':
        /* zooms out */
        zoom += 1;
        break;
    case 'd':
        /* rotates right */
        yrot = (yrot + 5) % 360;
        break;
    case 'a':
        /* rotates left */
        yrot = (yrot - 5) % 360;
        break;
    case 'w':
        /* rotates up */
        xrot = (xrot - 5) % 360;
        break;
    case 's':
        /* rotates down */
        xrot = (xrot + 5) % 360;
        break;
    case 27:
        /*  quits  */
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    //depth buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jonathan Reyes Bivariate Function");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    createMenu();
    glutMainLoop();
    return 0;
}
