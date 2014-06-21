/*
 * A tetris clone
 *
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

#include "tetris.h"


#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define ROTATE 4

#define INSERT 1
#define REMOVE 0



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

static int container[10*20];


// prototypes
void block(int action);
void draw_cell(int x, int y, int color);
void init(void);


void init(void)
{
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, CELL_SIZE * NCOL, CELL_SIZE * NROW, 0); // viewing area


	game.angle = 0;
	game.shape = 1;
	game.x = 5;
	game.y = 2;
	game.tick = 1.0;
	block(INSERT);
}


void draw_cell(int x, int y, int color)
{
	switch (color) {
	case 0: 
		glColor3f(0, 0, 0);
		break;
	case 1: 
		glColor3f(0, 1, 0);
		break;	
	default:
		glColor3f(1, 1, 1);
	}

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

void display_container(void)
{
	for (int i = 0; i < NCOL * NROW; i++) {
		draw_cell(i%NCOL, i/NCOL, container[i]);
	}
}

bool check(int direction)
{
	return true;
}

void block(int action /* INSERT or REMOVE */) 
{
	int x = game.x;
	int y = game.y;

	int color = (action == INSERT) ? game.shape + 1 : 0;

	for (int i = 0; i < 16; i++)
		if (shape[game.shape][game.angle] & (0x8000 >> i))
			container[x + y*10 + i%4 + (i/4)*10] = color;
}




void move(int direction)
{
	if (check(direction)) {
		block(REMOVE);
		if (direction == LEFT)
			game.x--;
		else if (direction == RIGHT)
			game.x++;
		else if (direction == DOWN)
			game.y++;
		else if (direction == ROTATE)
			game.angle = game.angle < 3 ? game.angle + 1 : 0;

		block(INSERT);
	}
}







void on_idle(void)
{
	
	int now = glutGet(GLUT_ELAPSED_TIME);


	int elapsed = now - last_update;

	if (elapsed > game.tick * 1000) {
		move(DOWN);
		last_update = now;
		
		
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
		move(RIGHT);
		break;
	case GLUT_KEY_LEFT:
		move(LEFT);
		break;
	case GLUT_KEY_UP:
		move(ROTATE);
		break;
	case GLUT_KEY_DOWN:
		break;
	}
	glutPostRedisplay();
}






void draw_shape(int type, int x, int y)
{
	
	
	// for (int i = 0; i < 16; i++)
	// 	if (shape[type][game.angle] & (0x8000 >> i))
	// 		draw_cell(x + i%4 - 2, y + i/4 + 1);	

}

void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw_shape(game.shape, game.x, game.y);

	
	display_container();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	

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





