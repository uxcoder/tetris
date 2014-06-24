/*
 * A tetris clone
 *
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "tetris.h"


#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define ROTATE 4

#define REMOVE 0
#define INSERT 1

#define BORDER 8

#define WIDTH (NCOL + 2)
#define HEIGHT (NROW + 1)

#define W (CELL_SIZE*(NCOL+10))
#define H (CELL_SIZE*(NROW+4))

static struct {
	int type;
	int x;
	int y;
	int rot;
} current;

static struct {
	bool running;
	int tick;
	int last_update;
	int container[WIDTH * HEIGHT];
} game;




// prototypes
void block(int action);
void draw_cell(int x, int y, int i);
void init(void);
bool check(int x, int y, int angle);
void spawn(void);
void shift(int row);


void init(void)
{
	srandomdev(); 
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0, W, H, 0);

	game.running = true;
	game.last_update = glutGet(GLUT_ELAPSED_TIME);


	for (int i = 0; i < NCOL; i++) {
		game.container[WIDTH*(HEIGHT-1)+i+1] = BORDER;
	}

	for (int i = 0; i < NROW; i++) {
		game.container[WIDTH*i] = BORDER;
		game.container[WIDTH*i+NCOL+1] = BORDER;
	}

	spawn();

}


void spawn(void)
{
	current.type = random() % 7;
	current.x = 4;
	current.y = -1;
	current.rot = 0;
	game.tick = 500;

	if (check(current.x, current.y, current.rot))
		block(INSERT);
	else
		game.running = false;
	
}


bool full(int row)
{
	for (int i = 0; i < NCOL; i++) {
		if (!game.container[1 + i + row * WIDTH])
			return false;
	}
	return true;
}


void shift(int row)
{
	for (int i = 0; i < NCOL; i++)
		game.container[i+1 + row * WIDTH] = game.container[i+1+(row-1)*WIDTH];
}

void draw_grid(void)
{
	glColor3f(0.1, 0.1, 0.1);
	for (int x = 0; x < W; x++)
		for (int y = 0; y < H; y++) {
			glBegin(GL_LINES);
			glVertex2f(x*CELL_SIZE, 0);
			glVertex2f(x*CELL_SIZE, H*CELL_SIZE);
			glVertex2f(0, y*CELL_SIZE);
			glVertex2f(W*CELL_SIZE, y*CELL_SIZE);
			glEnd();
		}
}


void draw_cell(int x, int y, int color)
{
	unsigned long palette[] = {
		0x000000, 0xFF0000, 0xFFFF00, 0xFFFFFF, 0x00FF00, 
		0x0000FF, 0x00FFFF, 0xFF00FF, 0x222222
	};
	
	int xoffset = (x+4) * CELL_SIZE;
	int yoffset = (y+1) * CELL_SIZE;

	glColor3f((palette[color] >> 16) / 256.0, 
		  (palette[color] >> 8 & 0xFF) / 256.0, 
		  (palette[color] & 0xFF) / 256.0);

	glPushMatrix();	
	glTranslatef(xoffset, yoffset, 0);
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
	for (int i = 0; i < WIDTH * HEIGHT; i++)
		draw_cell(i%WIDTH, i/WIDTH, game.container[i]);
}


bool check(int x, int y, int rot)
{
	for (int i = 0; i < 16; i++)
		if ((tetra_block[current.type][rot] & (0x8000 >> i))
			&& game.container[x + i%4 + WIDTH * (y + i/4)])
			return false;
	return true;
}


void block(int action /* INSERT or REMOVE */) 
{
	for (int i = 0; i < 16; i++)
		if (tetra_block[current.type][current.rot] & (0x8000 >> i))
			game.container[current.x + i%4 + WIDTH * (current.y + i/4)] = 
				(action == INSERT) ? current.type + 1 : 0;
}


bool move(int direction)
{
	bool res = true;

	// TODO use switch case block
	block(REMOVE);
	if (direction == LEFT) {
		if (check(current.x-1, current.y, current.rot)) {
			current.x--;
		}
		
	}
	else if (direction == RIGHT) {
		if (check(current.x+1, current.y, current.rot)) {
			current.x++;
		}
	}

	else if (direction == DOWN) {
		if (check(current.x, current.y+1, current.rot)) {
			current.y++;
		} else {
			res = false;	
		}
	}
	else if (direction == ROTATE) {
		int rot = current.rot < 3 ? current.rot + 1 : 0;
		if (check(current.x, current.y, rot)) {
			current.rot = rot;
		}
	}	
	
	block(INSERT);
	return res;
}


int check_lines(void)
{
	int lines = 0;

	for (int row = 0; row <= NROW - 1; row++) {
		if (full(row)) {
			lines++;
			for (int i = row; i > 1; i--)
				shift(i);
		}
	}
	return lines;
}


void onidle(void)
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	int elapsed = now - game.last_update;

	if (elapsed > game.tick && game.running) {
		if (move(DOWN)) {
			game.last_update = now;
		} else {
			
			check_lines();
			spawn();
		}
	}
	glutPostRedisplay();
}


void normkeys(unsigned char key, int x, int y) 
{
	if (key == 27)
		exit(0);
}


void speckeys(int key, int x, int y)
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
		game.tick = 1;
		break;
	}
	glutPostRedisplay();
}


void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display_container();
	// draw_grid();
	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(W, H);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Tetris");
	glutDisplayFunc(render);
	glutKeyboardFunc(normkeys);
	glutSpecialFunc(speckeys);
	glutIdleFunc(onidle);
	init();
	glutMainLoop();
	
	return 0;
}





