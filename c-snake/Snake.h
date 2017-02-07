#include "Grid.h"

typedef enum
{
	DIR_UP = 'w',
	DIR_DOWN = 's',
	DIR_LEFT = 'a',
	DIR_RIGHT = 'd'
} SnakeDir;

typedef struct segdata
{
	char c;
	int x, y;
} SegData;

typedef struct segmenttype
{
	SegData data;
	struct segmenttype *next, *previous;
} Segment;

typedef struct snaketype
{
	Segment *head, *tail;
	SnakeDir dir;
	Grid *pGrid;
	unsigned size;
	unsigned speed;

	int error;
	unsigned score;
	int skipRemove;
} Snake;

Snake *initSnake( int gridWidth );

void insertAtTail( Snake *snake );

void removeFromHead( Snake *snake );

SegData getFront( Snake *snake );

void printError( Snake *snake );

char getNextChar( Snake *snake );

int snakeHasSegmentAtPos( Snake *snake, int x, int y );

int snakeWillHitItself( Snake *snake );

void generateSpecificPickupAtRandomLocation( Snake *snake, char pickupType );

void generateRandomPickupAtRandomLocation( Snake *snake );