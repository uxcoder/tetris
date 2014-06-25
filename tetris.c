/*
 * A tetris clone
 * Copyright (C) 2014 Alexander Ariskin
 * Copyright (C) 2014 Igor Archipov
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "tetris.h"


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
	int container[NCOLB * NROWB];
	int stat[NTYPES];
	int score;
} game;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Tetris");
	glutDisplayFunc(render);
	glutSpecialFunc(speckeys);
	glutIdleFunc(onidle);
	init();
	glutMainLoop();
	return EXIT_SUCCESS;
}


void init(void)
{
	srandom(time(NULL)); 
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, WIDTH, HEIGHT, 0);

	game.running = true;
	game.last_update = glutGet(GLUT_ELAPSED_TIME);

	for (int i = 0; i < NCOL; i++)
		game.container[NCOLB * (NROWB - 1) + i + 1] = 1;

	for (int i = 0; i < NROW; i++) {
		game.container[NCOLB * i] = 1;
		game.container[NCOLB * i + NCOL + 1] = 1;
	}
	
	spawn();
}


void settext(float x, float y, const char *string, int color) 
{
	int len = strlen(string);

	glColor3f((color >> 16) / 256.0, 
		  ((color >> 8) & 0xFF) / 256.0, 
		  (color & 0xFF) / 256.0);

	glRasterPos2f(x, y);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[i]);
}


void display_stat(void)
{
	char s[80];
	int x = MARGIN_LEFT + NCOLB * CELL_SIZE;
	int y = MARGIN_TOP + CELL_SIZE;
	char *blockname = "IJLOSTZ";
	int lineheight = 18;

	for (int i = 0; i < NTYPES; i++) {
		sprintf(s, "%c%14d", blockname[i], game.stat[i]);
		settext(x, y + i * lineheight, s, 0x888888);
	}

	x = CELL_SIZE;

	sprintf(s, "%s%10d", "level", 1);
	settext(x, y, s, 0xFFFFFF);

	sprintf(s, "%s%10d", "score", game.score);
	settext(x, y + lineheight, s, 0xFFFFFF);

}


void spawn(void)
{
	
	current.type = random() % 7;
	current.x 	= 4;
	current.y 	= -1;
	current.rot 	= 0;
	game.tick 	= 500;

	game.stat[current.type]++;

	if (check(current.x, current.y, current.rot)) {
		block(INSERT);
		game.score++;
	}
	else {
		game.running = false;
	}
}


bool full(int row)
{
	for (int i = 0; i < NCOL; i++) {
		if (!game.container[1 + i + row * NCOLB])
			return false;
	}
	game.score += 10;
	return true;
}


void drop(int row)
{
	for (int i = 0; i < NCOL; i++)
		game.container[1 + i + row * NCOLB] = 
			game.container[1 + i + (row - 1) * NCOLB];
}


void draw_grid(void)
{
	glColor3f(0.15, 0.15, 0.15);
	
	glPushMatrix();	
	glTranslatef(MARGIN_LEFT, MARGIN_TOP, 0);
	for (int x = 1; x < NCOL; x++) {
		glBegin(GL_LINES);
		glVertex2f(x * CELL_SIZE, 0);
		glVertex2f(x * CELL_SIZE, NROW * CELL_SIZE);
		glEnd();
	}
	for (int y = 1; y < NROW; y++) {
		glBegin(GL_LINES);
		glVertex2f(0, y * CELL_SIZE);
		glVertex2f(NCOL * CELL_SIZE, y * CELL_SIZE);
		glEnd();
	}
	
	glColor3f(0.5, 0.5, 0.5);
	
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 0);
	glVertex2f(NCOL * CELL_SIZE, 0);
	glVertex2f(NCOL * CELL_SIZE, NROW * CELL_SIZE);
	glVertex2f(0, NROW * CELL_SIZE);
	glEnd();

	glPopMatrix();
}


void draw_cell(int x, int y, int color)
{
	unsigned long palette[] = {
		0x000000, 0xF50C54, 0xE29714, 0xCFCFCF, 
		0x3AA663, 0x0C8CF9, 0x18D8E9, 0xCD3BB7
	};
	
	int xoffset = MARGIN_LEFT + (x - 1) * CELL_SIZE;
	int yoffset = MARGIN_TOP + y * CELL_SIZE;
	
	glColor3f((palette[color] >> 16) / 256.0, 
		  (palette[color] >> 8 & 0xFF) / 256.0, 
		  (palette[color] & 0xFF) / 256.0);

	glPushMatrix();	
	glTranslatef(xoffset, yoffset, 0);
	glRectf(0, 0, CELL_SIZE, CELL_SIZE);
	glPopMatrix();
}


void display_container(void)
{
	for (int i = 0; i < NCOL * NROW; i++) {
		if (game.container[1 + i + 2 * (i / NCOL)])
			draw_cell(i % NCOL + 1, i / NCOL, 
				game.container[1 + i + 2 * (i / NCOL)]);
	}
}


bool check(int x, int y, int rot)
{
	for (int i = 0; i < 16; i++)
		if ((tblock[current.type][rot] & (0x8000 >> i))
			&& game.container[x + i % 4 + NCOLB * (y + i / 4)])
			return false;
	return true;
}


void block(int action /* INSERT or REMOVE */) 
{
	for (int i = 0; i < 16; i++)
		if (tblock[current.type][current.rot] & (0x8000 >> i))
			game.container[current.x + i % 4 + 
				NCOLB * (current.y + i / 4)] = 
				(action == INSERT) ? current.type + 1 : 0;
}


bool move(int direction)
{
	bool res = true;

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


void check_lines(void)
{
	for (int row = 0; row < NROW; row++)
		if (full(row))
			for (int i = row; i > 1; i--)
				drop(i);
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
	draw_grid();
	display_stat();
	glutSwapBuffers();
}

