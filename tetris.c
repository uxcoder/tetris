/*
 * A tetris clone
 *
 */

#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <time.h>
#include "tetris.h"


int angle = 0;

void initDisplay(void)
{
	glClearColor(0, 0, 0, 0); // bg colour
	gluOrtho2D(0, CELL_SIZE * 10, CELL_SIZE * 20, 0); // viewing area
}

void draw_cell(int x, int y)
{
	glPushMatrix();	
	glTranslatef(x * CELL_SIZE, y * CELL_SIZE, 0);
	glBegin(GL_QUADS);                      
		glVertex2f(0, 0);
		glVertex2f(0, CELL_SIZE);
		glVertex2f(CELL_SIZE, CELL_SIZE);
		glVertex2f(CELL_SIZE, 0);
    	glEnd(); 	
	glPopMatrix();
}

void process_special_keys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		angle = angle < 3 ? angle + 1 : 0;
		break;
	case GLUT_KEY_LEFT:
		angle = angle > 0 ? angle - 1 : 3;
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}

void draw_shape(int type, int angle, int x0, int y0)
{
	glColor3f(1, 0, 0);
	
	for (int i = 0; i < 16; i++)
		if (shape[type][angle] & (0x8000 >> i))
			draw_cell(x0 + i%4 - 2, y0 + i/4 + 1);	

}

void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_shape(2, angle, 5, 8);
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(CELL_SIZE * 10, CELL_SIZE * 20);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Tetris");
	initDisplay();
	glutDisplayFunc(render);
	glutSpecialFunc(process_special_keys);
	glutMainLoop();
	
	return 0;
}





