/*
  OBJ Viewer with Texture

  Author:       Rodrigo Luis de Souza da Silva
  Date:         22/12/2005
  Last Update:  06/12/2017
  Description:  View OBJ files based on the glcWarefrontObject class.
*/

#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "glcWavefrontObject.h"

#define NUM_OBJECTS 7

using namespace std;

char objectFiles[NUM_OBJECTS][50] =
{
    "../data/obj/al.obj",
    "../data/obj/dolphins.obj",
    "../data/obj/f-16.obj",
    "../data/obj/flowers.obj",
    "../data/obj/porsche.obj",
    "../data/obj/rose+vase.obj",
    "../data/obj/soccerball.obj"
};

typedef struct
{
    //GLMmodel* pmodel = NULL;
    glcWavefrontObject *pmodel = NULL;
} object;

object *objectList;

glcWavefrontObject *objectManager = NULL;

int selected = 0;
int selectedShading = SMOOTH_SHADING;
int selectedRender = USE_MATERIAL;
float dist = 20.0;
int width = 800;
int height = 800;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;

// Menu
void showMenu()
{
    cout << "OBJECT VIEWER" << endl;
    cout << "-------------\n" << endl;
    cout << "Mouse\n" << endl;
    cout << "* Click to move the object and scroll to zoom in/out." << endl;
    cout << "Keyboard" << endl;
    cout << "* 1 to " << NUM_OBJECTS << " to change objects" << endl;
    cout << "* 's' to change shading mode (FLAT_SHADING or SMOOTH_SHADING)" << endl;
    cout << "* 'r' to change render mode (USE_MATERIAL or USE_COLOR)" << endl;
    cout << "* ESC - Para sair" << endl;
}

//Reshape Func
void reshape( int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}


void keyboard(unsigned char key, int x, int y)
{
    if(isdigit(key))
    {
        int val = atoi((const char *) &key);
        if(val > 0 && val <= NUM_OBJECTS )
            selected = val-1;
    }

    switch (key)
    {
        case 27 :
            exit(0);
        break;
        case 's':
            selectedShading = (selectedShading == FLAT_SHADING) ? SMOOTH_SHADING : FLAT_SHADING;
        break;
        case 'r':
            selectedRender = (selectedRender == USE_MATERIAL) ? USE_COLOR : USE_MATERIAL;
        break;

    }
    glutPostRedisplay();
}

// Motion callback
void motion(int x, int y )
{
    rotationY += (float) (y - last_y);
    rotationX += (float) (x - last_x);

    last_x = x;
    last_y = y;

    glutPostRedisplay();
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
    if(button == 3) // Scroll up
    {
        if(dist>=5.0)  dist-=1;
    }
    if(button==4)   // Scroll down
    {
        if(dist<=40.0) dist+=1;
    }
}

void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Sistema de Projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
    gluLookAt (0.0, 0.0, dist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Sistema de Visualização
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Movimenta o objeto e camera fica parada
    glRotatef( rotationY, 1.0, 0.0, 0.0 );
    glRotatef( rotationX, 0.0, 1.0, 0.0 );

    objectManager->SelectObject(selected);
    objectManager->SetShadingMode(selectedShading); // Possible values: FLAT_SHADING e SMOOTH_SHADING
    objectManager->SetRenderMode(selectedRender);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
    objectManager->Draw();

    glutSwapBuffers ();
    glutPostRedisplay();
}


void init (void)
{
    // selecionar cor de fundo (preto)
    glClearColor (0.2, 0.2, 0.2, 0.0);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    GLfloat light0_position[] = {-3.0f, 3.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,0.0f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);

    // LOAD OBJECTS
    objectManager = new glcWavefrontObject();
    objectManager->SetNumberOfObjects(NUM_OBJECTS);
    for(int i = 0; i < NUM_OBJECTS; i++)
    {
        objectManager->SelectObject(i);
        objectManager->ReadObject(objectFiles[i]);
        objectManager->Unitize();
        objectManager->FacetNormal();
        objectManager->VertexNormals(90.0);
        objectManager->Scale(5);
    }
}


int main(int argc, char** argv)
{
    showMenu();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Object Viewer");
    init ();
    glutMouseFunc( mouse );
    glutKeyboardFunc(keyboard);
    glutMotionFunc( motion );
    glutReshapeFunc( reshape );
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

