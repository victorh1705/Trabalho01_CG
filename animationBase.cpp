/*
  Name:        animationBase.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update:	23/08/2016
  Date:        23/08/2016
  Description: Arquivo base para o exercício da auila de animação
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "extras.h"

// Globals
float desiredFPS = 30;
int width  = 1000;
int height = 500;

float dir = 1.0f;
int transBolaZ = 0.0f;
float rotationX = 0.0, rotationY = 0.0;
int last_x, last_y;

float eyeX = 150.0;
float eyeY = 0.0;
float eyeZ = 80.0;


float eye[3];
float foc[3] = {0.0f, 500.0f, 0.0f};
float up[3];

int planoZ = 0;
int planoY = 0;

//Bola
int raio = 5.0;

void criaPlano();
int haColisaoPlano();

void idle()
{
float t, desiredFrameTime, frameTime;
    static float tLast = 0.0;

    /// Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME);

    /// convert milliseconds to seconds
    t /= 1000.0;

    /// Calculate frame time
    frameTime = t - tLast;
    /// Calculate desired frame time
    desiredFrameTime = 1.0 / (float) (desiredFPS);

    // Check if the desired frame time was achieved. If not, skip animation.
    if( frameTime <= desiredFrameTime)
        return;

    // **  UPDATE ANIMATION VARIABLES ** //
    float step = 1; // Speed of the animation
    if(transBolaZ <= -40 || transBolaZ >= 40.0) dir *= -1;
    transBolaZ +=5.0*dir; // Variation computed considering the FPS

    if( haColisaoPlano() == 1) printf("\n ha colisao");

    // Update tLast for next time, using static local variable
    tLast = t;

    glutPostRedisplay();
}

void init(void)
{
   glClearColor (0.5, 0.5, 0.5, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);               /// Habilita Z-buffer
	initLight(width, height);
}

void criaPlano(){
    glPushMatrix();
         glTranslatef(0.0, planoY, planoZ);
        glScalef(20.0 , 20.0, 5.0);
        glutSolidCube(1.0);
    glPopMatrix();

}

int haColisaoPlano(){
    if( planoY + 20 > 0 && planoY - 20 < 0){
        if( transBolaZ - raio <= planoZ){
            return 1;
        }
    }
    return 0;
}

void display(void)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(40.0f,(GLfloat)width/(GLfloat)height, 0.1 ,2500);
    gluLookAt (eyeX, eyeY, eyeZ,
              0.0, 0.0, 0.0,
               0.0,  0.0, 1.0);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();

    glPushMatrix();
        setMaterial();
        glTranslatef(0.0, 0.0, transBolaZ); /// Posicionamento inicial da esfera
        glutSolidSphere(raio, 10.0, 10.0);
    glPopMatrix();

    criaPlano();

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   width = w;
   height = h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard (unsigned char key, int x, int y)
{
   if(tolower(key) == 27) exit(0);
}

void specialKeyRelease(int key, int x, int y){
    switch(key){
        case GLUT_KEY_RIGHT:
            planoY += 10;
            break;

        case GLUT_KEY_LEFT:
            planoY -= 10;
            break;

        case GLUT_KEY_DOWN:
            planoZ -= 10;
            break;

        case GLUT_KEY_UP:
            planoZ += 10;
            break;
    }
}

/// Motion callback
void motion(int x, int y )
{
   eyeZ =  ((float)-y + eyeZ);
   eyeY =  ((float)-x + eyeY);

   printf("\n x : %f.2  e y: %f.2", &x, &y);
   printf("\n  eyeX - %f.2 eyeY - %f.2 eyeZ - %f.2", &eyeX, &eyeY, &eyeZ);

   glutPostRedisplay();
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (width, height);
   glutInitWindowPosition (100, 100);
   glutCreateWindow("Animation Base");
   glutIdleFunc( idle);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMotionFunc( motion );
   glutKeyboardFunc(keyboard);
   glutSpecialUpFunc( specialKeyRelease);
   glutMainLoop();
   return 0;
}
