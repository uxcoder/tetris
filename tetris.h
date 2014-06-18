#ifndef __TETRIS_H
#define __TETRIS_H

#define NCOL 10
#define NROW 20
#define NROTATIONS 4
#define NSHAPES 7

typedef enum {I, J, L, O, S, T, Z} shape_type;

int shape[NSHAPES][NROTATIONS][4][4] =
{
	{	  // I-shape 
		{	
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{	
			{0,0,1,0},	
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0}
		}
	},
	{	  // J-shape
		{
			{0,0,0,0},	
			{0,1,1,1},
			{0,0,0,1},
			{0,0,0,0}
		},
		{
			{0,0,1,1},	
			{0,0,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},	
			{0,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,0,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	},
	{	  // L-shape
		{
			{0,0,0,0},	
			{0,1,1,1},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,0,1,0},
			{0,0,1,1},
			{0,0,0,0}
		},
		{
			{0,0,0,1},	
			{0,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,1,0},	
			{1,0,1,0},
			{0,0,1,0},
			{0,0,0,0}
		}
	},
	{	  // O-shape
		{
			{0,0,0,0},	
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},	
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},	
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},	
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	},
	{	  // S-shape
		{
			{0,0,0,0},	
			{0,0,1,1},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,0,1,1},
			{0,0,0,1},
			{0,0,0,0}
		},
		{
			{0,0,0,0},	
			{0,0,1,1},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,0,1,1},
			{0,0,0,1},
			{0,0,0,0}
		}
	},
	{	  // T-shape
		{
			{0,0,0,0},	
			{0,1,1,1},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,0,1,1},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0},	
			{0,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		}
	},
	{	  // Z-shape
		{
			{0,0,0,0},	
			{0,1,1,0},
			{0,0,1,1},
			{0,0,0,0}
		},
		{
			{0,0,0,1},	
			{0,0,1,1},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},	
			{0,1,1,0},
			{0,0,1,1},
			{0,0,0,0}
		},
		{
			{0,0,0,1},	
			{0,0,1,1},
			{0,0,1,0},
			{0,0,0,0}
		}
	}
};


#endif