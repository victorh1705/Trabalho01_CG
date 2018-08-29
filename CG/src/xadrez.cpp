/*
  Name:        quad_transform.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        16/09/2004
  Description: Transformations using of OpenGL commands
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0, scale = 1.0;
float xtrans = 0, ytrans = 0, ztrans = 0;
int enableMenu = 0;

//idle
float desiredFPS = 60;

void display(void);
void init (void);
void idle();
void desenhaEixos();
void showMenu();
void mouse(int button, int state, int x, int y);
void keyboardPress(unsigned char key, int x, int y);
void specialKeysRelease(int key, int x, int y);

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
   glutInitWindowSize (300, 300);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("hello");

   glutMouseFunc( mouse );
   glutKeyboardFunc( keyboardPress );
   glutSpecialUpFunc( specialKeysRelease );
   glutIdleFunc(idle);

   init ();
   printf("Posicione as janelas e clique na janela do OpenGL para habilitar o menu.\n");
   glutDisplayFunc(display);
   glutMainLoop();

   return 0;
}

void idle()
{
    float t, desiredFrameTime, frameTime;
    static float tLast = 0.0;

    // Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME);
    // convert milliseconds to seconds
    t /= 1000.0;

    // Calculate frame time
    frameTime = t - tLast;
    // Calculate desired frame time
    desiredFrameTime = 1.0 / (float) (desiredFPS);

    xtrans = 20;
    ytrans = 20;
    // Check if the desired frame time was achieved. If not, skip animation.
    if( frameTime <= desiredFrameTime)
        return;
    /* Update tLast for next time, using static local variable */
    tLast = t;

    glutPostRedisplay();

}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      enableMenu = 1;
}

void showMenu()
{
   int op;
   system("cls");
   printf("\n=== MENU ===");
   printf("\n1 - Translacao");
   printf("\n2 - Rotacao");
   printf("\n3 - Escala");
   printf("\n4 - Sair");
   printf("\nOpcao: ");

   //scanf("%d", &op);
}

// Keyboard callback
void keyboardPress(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(0);
		break;
		case '+':
			scale *= 1.1;
		break;
		case '-':
			scale *= 0.9;
		break;
		case 'r':
			angle += 10;
		break;
		case 'R':
			angle -= 10;
		break;
		default:
			printf("\nUma tecla não mapeada foi pressionada");
		break;
	}
}

// Special Keys callback
void specialKeysRelease(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT :
		    xtrans -= 10;
		break;
		case GLUT_KEY_RIGHT :
			xtrans += 10;
		break;
		case GLUT_KEY_UP :
			ytrans += 10;
		break;
		case GLUT_KEY_DOWN :
			ytrans -= 10;
		break;
	}
	glutPostRedisplay();
}

void desenhaEixos()
{
   glColor3f (0.0, 1.0, 0.0);
   glBegin(GL_LINES);
      glVertex2f (0.0, -100.0);
      glVertex2f (0.0, 100.0);
      glVertex2f (-100.0, 0.0);
      glVertex2f (100.0, 0.0);
   glEnd();
}

void desenhaBranco(int x, int y)
{
   glColor3f (1.0, 1.0, 1.0);
   glBegin(GL_POLYGON);
      glVertex2f (x, y);
      glVertex2f (x, y + 1);
      glVertex2f (x + 1, y + 1);
      glVertex2f (x+ 1, y);
   glEnd();
}

void desenhaPreto(int x, int y)
{
   glColor3f (0.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
      glVertex2f (x, y);
      glVertex2f (x, y + 1);
      glVertex2f (x + 1, y + 1);
      glVertex2f (x+ 1, y);
   glEnd();
}

void display(void)
{
    int y_origem = -3;
    int x_origem = -3;

   // Limpar todos os pixels
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glLoadIdentity(); // Inicializa com matriz identidade

   int i = x_origem;
   int j = y_origem;
   //desenhaEixos();
    for( i = x_origem; i <= 3; i++){
        for( j = y_origem; j <= 3; j++){
            if( i%2 == 0){
                if(j%2 == 0){
                    desenhaBranco(i,j);
                }else{
                    desenhaPreto(i,j);
                }
            }else{
                if(j%2 == 0){
                    desenhaPreto(i,j);
                }else{
                    desenhaBranco(i,j);
                }
            }
        }
    }


   glutSwapBuffers ();
   glutPostRedisplay();

   if(enableMenu) showMenu();
}

void init (void)
{
   // selecionar cor de fundo (preto)
   glClearColor (0.0, 0.0, 0.0, 0.0);

   // inicializar sistema de viz.
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-3.0, 3.0, -3.0, 3.0, -3.0, 3.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
