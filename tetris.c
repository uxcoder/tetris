/*
 * A tetris clone
 *
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

#include "tetris.h"

struct block {
	int shape;
	int x;
	int y;
	int angle;
};

static struct {
	int shape;
	int x;
	int y;
	int angle;
	double tick;
} game;


static int last_update;



void init(void)
{
	glClearColor(0, 0, 0, 0); // bg colour
	gluOrtho2D(0, CELL_SIZE * 10, CELL_SIZE * 20, 0); // viewing area
}

bool can_move_left(void)
{
	if (game.x > 1) 
		return true;
	
	return false;
}

bool can_move_right(void)
{
	if (game.x < NCOL-2) 
		return true;
	
	return false;
}

bool can_move_down(void)
{
	if (game.y < 15)
		return true;
	
	return false;
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


void on_idle(void)
{
	
	int now = glutGet(GLUT_ELAPSED_TIME);


	int elapsed = now - last_update;

	if (elapsed > game.tick * 1000) {
		if (can_move_down()) {
			game.y++;
			last_update = now;
		}
		
	}

	glutPostRedisplay();
	
}


void process_normal_keys(unsigned char key, int x, int y) 
{
	if (key == 27)
		exit(0);
}

void process_special_keys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (can_move_right())
			game.x++;
		break;
	case GLUT_KEY_LEFT:
		if (can_move_left())
			game.x--;
		break;
	case GLUT_KEY_UP:
		game.angle = game.angle < 3 ? game.angle + 1 : 0;
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}



void draw_shape(int type, int x, int y)
{
	glColor3f(1, 0, 0);
	
	for (int i = 0; i < 16; i++)
		if (shape[type][game.angle] & (0x8000 >> i))
			draw_cell(x + i%4 - 2, y + i/4 + 1);	

}

void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_shape(game.shape, game.x, game.y);
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	game.angle = 0;
	game.shape = 1;
	game.x = 5;
	game.y = 2;
	game.tick = 1.0;

	// struct timeval t;
	// gettimeofday(&t, NULL);

	// last_update = (t.tv_sec - t.tv_sec) * 1000.0;
	// last_update += (t.tv_usec - t.tv_usec) / 1000.0;


	

	glutInit(&argc, argv);
	glutInitWindowSize(CELL_SIZE * 10, CELL_SIZE * 20);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Tetris");
	init();
	glutDisplayFunc(render);
	glutKeyboardFunc(process_normal_keys);
	glutSpecialFunc(process_special_keys);
	glutIdleFunc(on_idle);


	last_update = glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop();
	
	return 0;
}





