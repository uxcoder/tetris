#ifndef __TETRIS_H
#define __TETRIS_H

#define NCOL 		10
#define NROW 		20
#define NCOLB 		(NCOL + 2) // Add borders left and right
#define NROWB 		(NROW + 1) // Additinal border at the bottom
#define NTYPES		7
#define NCELLS          16
#define CELL_SIZE 	20
#define MARGIN_LEFT	(CELL_SIZE * 10)
#define MARGIN_RIGHT	(CELL_SIZE * 10)
#define MARGIN_TOP	(CELL_SIZE * 2)
#define MARGIN_BOTTOM	(CELL_SIZE * 2)
#define WIDTH		(CELL_SIZE * NCOL + MARGIN_LEFT + MARGIN_RIGHT)
#define HEIGHT	 	(CELL_SIZE * NROW + MARGIN_TOP + MARGIN_BOTTOM)
	
#define LEFT 		1
#define RIGHT 		2
#define DOWN 		3
#define ROTATE 		4

#define REMOVE 		0
#define INSERT 		1


int tblock[7][4] = {
	{0x0F00, 0x2222, 0x0F00, 0x2222}, /* I */
	{0x0710, 0x3220, 0x0470, 0x2260}, /* J */
	{0x0740, 0x2230, 0x1700, 0x6220}, /* L */
	{0x0660, 0x0660, 0x0660, 0x0660}, /* O */
	{0x0360, 0x2310, 0x0360, 0x2310}, /* S */
	{0x0720, 0x2320, 0x2700, 0x2620}, /* T */
	{0x0630, 0x1320, 0x0630, 0x1320}  /* Z */
};

// prototypes
void init(void);
void settext(float x, float y, const char *string, int color);
void display_stat(void);
void spawn(void);
bool full(int row);
void drop(int row);
void draw_grid(void);
void draw_cell(int x, int y, int i);
void display_container(void);
bool check(int x, int y, int angle);
void block(int action);
bool move(int direction);
void check_lines(void);
void onidle(void);
void speckeys(int key, int x, int y);
void render(void); 

#endif


