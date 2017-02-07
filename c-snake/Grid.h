#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

typedef enum
{
	SYM_GRID = 176,
	SYM_SEG = 219,

	PICKUP_FOOD = '+',
	PICKUP_CULL = '/',
	PICKUP_NORM = 'N',
	PICKUP_FAST = 'F',
	PICKUP_SLOW = 'S',

	KEY_ESCAPE = 27,
	KEY_SPACE = 32,

	SPEED_NORMAL = 125,
	SPEED_FAST = 80,
	SPEED_SLOW = 250,

	ERR_OOB_TOP = 1,
	ERR_OOB_BOTTOM,
	ERR_OOB_LEFT,
	ERR_OOB_RIGHT,
	ERR_HIT_ITSELF,
	ERR_KEYP_ESC = 27
};

typedef struct gridtype
{
	char **grid;
	int width, height;
} Grid;

Grid *initGrid( int width );

void printGrid( Grid *grid, int score );

void updateGridAtPos( Grid *grid, int x, int y, char newChar );

char getCharAtPos( Grid *grid, int x, int y );

int invalidX( Grid *grid, int x );

int invalidY( Grid *grid, int y );

int containsFood( Grid *grid );

int containsPickup( Grid *grid, char pickupType );