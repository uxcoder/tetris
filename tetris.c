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


static struct tetrablock {
	int type;
	int x;
	int y;
	int rot;
};

static struct {
	bool running;
	int tick;
	int last_update;
	int container[NCOLB * NROWB];
	struct tetrablock block;
	int stat[7];
	int score;
} game;




// prototypes
void block(int action);
void draw_cell(int x, int y, int i);
void init(void);
bool check(int x, int y, int angle);
void spawn(void);
void drop(int row);
void settext(float x, float y, const char *string, int color);
void display_gameover(void);


void init(void)
{
	srandomdev(); 
	
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


void settext(float x, float y, const char *string, int color) {
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

	for (int i = 0; i < 7; i++) {
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
	
	game.block.type = random() % 7;
	game.block.x 	= 4;
	game.block.y 	= -1;
	game.block.rot 	= 0;
	game.tick 	= 500;

	game.stat[game.block.type]++;

	if (check(game.block.x, game.block.y, game.block.rot)) {
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
		if ((tblock[game.block.type][rot] & (0x8000 >> i))
			&& game.container[x + i % 4 + NCOLB * (y + i / 4)])
			return false;
	return true;
}


void block(int action /* INSERT or REMOVE */) 
{
	for (int i = 0; i < 16; i++)
		if (tblock[game.block.type][game.block.rot] & (0x8000 >> i))
			game.container[game.block.x + i % 4 + 
				NCOLB * (game.block.y + i / 4)] = 
				(action == INSERT) ? game.block.type + 1 : 0;
}


bool move(int direction)
{
	bool res = true;

	block(REMOVE);
	if (direction == LEFT) {
		if (check(game.block.x-1, game.block.y, game.block.rot)) {
			game.block.x--;
		}
		
	}
	else if (direction == RIGHT) {
		if (check(game.block.x+1, game.block.y, game.block.rot)) {
			game.block.x++;
		}
	}

	else if (direction == DOWN) {
		if (check(game.block.x, game.block.y+1, game.block.rot)) {
			game.block.y++;
		} else {
			res = false;	
		}
	}
	else if (direction == ROTATE) {
		int rot = game.block.rot < 3 ? game.block.rot + 1 : 0;
		if (check(game.block.x, game.block.y, rot)) {
			game.block.rot = rot;
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
	draw_grid();
	display_stat();

	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Tetris");
	glutDisplayFunc(render);
	glutKeyboardFunc(normkeys);
	glutSpecialFunc(speckeys);
	glutIdleFunc(onidle);
	init();
	glutMainLoop();
	
	return EXIT_SUCCESS;
}





