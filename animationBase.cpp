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
#include <math.h>
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

float m_yaw, m_pitch;
bool yawLock = false;
bool pitchLock = false;


float dist = 140.0f;
float eyeX = 140.0;
float eyeY = 0.0;
float eyeZ = 140.0;

float eye[3];
float up[3];

// Initial Position
float EyeInitial[3] = {120.0f, 0.0f, 120.0f};
float foc[3]        = {0.0f, 0.0f, 0.0f};
float upInitial[3]  = {0.0f, 0.0f, 1.0f};

int planoZ = 0;
int planoY = 0;

//Bola
int raio = 5.0;

//Gravidade
float constGrav = 5.0;
float velocidade = 0.0;
float posicaoZ = 0.0;
float alturaMaxima = 40.0;


void criaPlano();
int haColisaoPlano();


void RotatePoint(float *in, float *out, float pitch, float yaw)
{
	float xt = in[0], yt = in[1], zt = in[2];
	float x, y, z;

	// Rotation in 'y' axis
    x = zt * sin(yaw) + xt * cos(yaw);
	y = yt;
	z = zt * cos(yaw) - xt * sin(yaw);
	// Rotation in 'x' axis
   out[0] = y * sin(pitch) + x * cos(pitch);
   out[1] = y * cos(pitch) - x * sin(pitch);
   out[2] = z;

   //printf("x - %0.6f \n y - %0.6f \n z - %0.6f", &x, &y, &z);
}

void RotateCamera()
{
   float r_pitch = M_PI * m_pitch / 180;
   float r_yaw   = M_PI * m_yaw / 180;

	// Rotate eye vector
	RotatePoint(EyeInitial, eye, r_pitch, r_yaw);

	// Rotate up vector
	RotatePoint(upInitial, up, r_pitch, r_yaw);
}

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
    if(transBolaZ <= -40 + 5){
        dir *= -1;
        alturaMaxima = 40;
    } else if(transBolaZ >= alturaMaxima - 5){
        dir *= -1;
    }else if( haColisaoPlano() == 1){
        dir *= -1;
        alturaMaxima = planoZ + 80;
    }
    transBolaZ +=5.0*dir; // Variation computed considering the FPS
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
        if( transBolaZ - raio <= planoZ + 5
           && transBolaZ - raio >= planoZ - 5){
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

    RotateCamera();

    gluLookAt (eyeX, eyeY, eyeZ,
               foc[0], foc[1], foc[2],
               up[0],  up[1],   up[2]);

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



void fixAngle(float *angle)
{
	if(*angle > 360) *angle = 1.0f;
	if(*angle < 0)   *angle = 359.0f;
}

// Motion callback
void motion(int x, int y )
{
	if(!yawLock) 	rotationX += (float) (x - last_x);
	if(!pitchLock)	rotationY += (float) (y - last_y);

   last_x = x;
   last_y = y;

   fixAngle(&rotationX);
   fixAngle(&rotationY);

	m_yaw   = rotationX;
   m_pitch = rotationY;

}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
   {
      last_x = x;
      last_y = y;
   }
}

void keyboard (unsigned char key, int x, int y)
{
   switch (tolower(key))
   {
      case '+' :
         dist++;
         if(dist>140) dist=140;
         eyeX = eyeZ =dist;
        break;
      case '-' :
         dist--;
         if(dist<20) dist=20;
         eyeX = eyeZ =dist;
        break;
   }
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
