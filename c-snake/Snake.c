#include "Snake.h"

Snake *initSnake( int gridWith )
{
	Snake *newSnake = (Snake*)malloc( sizeof( Snake ) );
	if ( !newSnake )
	{
		fprintf( stderr, "Out of memory\n" );
		return NULL;
	}

	newSnake->pGrid = initGrid( 25 );

	newSnake->head = NULL;
	newSnake->tail = NULL;
	newSnake->dir = DIR_RIGHT;
	newSnake->size = 0;
	newSnake->speed = SPEED_NORMAL;

	newSnake->error = 0;
	newSnake->score = 0;
	newSnake->skipRemove = 0;

	return newSnake;
}

void insertAtTail( Snake *snake )
{
	Segment *newSeg = (Segment*)malloc( sizeof( Segment ) );
	if ( !newSeg )
	{
		fprintf( stderr, "Out of memory\n" );
		return;
	}

	newSeg->next = NULL;
	newSeg->previous = NULL;

	newSeg->data.c = SYM_SEG;

	if ( snake->head == NULL )
	{
		snake->head = newSeg;
		snake->tail = newSeg;
		newSeg->data.x = (int)( (float)(snake->pGrid->width / 2) );
		newSeg->data.y = (int)( (float)(snake->pGrid->height / 2) );
	}
	else
	{
		newSeg->previous = snake->tail;
		snake->tail->next = newSeg;
		snake->tail = newSeg;

		switch ( snake->dir )
		{
		case DIR_UP:
			newSeg->data.x = newSeg->previous->data.x;
			newSeg->data.y = newSeg->previous->data.y - 1;
			if ( newSeg->data.y < 0 )
			{
				snake->error = ERR_OOB_TOP;
				return;
			}
			break;
		case DIR_DOWN:
			newSeg->data.x = newSeg->previous->data.x;
			newSeg->data.y = newSeg->previous->data.y + 1;
			if ( newSeg->data.y > snake->pGrid->height - 1 )
			{
				snake->error = ERR_OOB_BOTTOM;
				return;
			}
			break;
		case DIR_LEFT:
			newSeg->data.x = newSeg->previous->data.x - 1;
			newSeg->data.y = newSeg->previous->data.y;
			if ( newSeg->data.x < 0 )
			{
				snake->error = ERR_OOB_LEFT;
				return;
			}
			break;
		case DIR_RIGHT:
			newSeg->data.x = newSeg->previous->data.x + 1;
			newSeg->data.y = newSeg->previous->data.y;
			if ( newSeg->data.x > snake->pGrid->width - 2 )
			{
				snake->error = ERR_OOB_RIGHT;
				return;
			}
			break;
		}
	}

	snake->size++;

	updateGridAtPos( snake->pGrid, newSeg->data.x, newSeg->data.y, SYM_SEG );
}

void removeFromHead( Snake *snake )
{
	if ( snake->head == NULL )
	{
		//fprintf( stderr, "No head to remove\n" );
		return;
	}
	else if ( snake->head == snake->tail )
	{
		//fprintf( stderr, "Cannot of 0 segments\n" );
		return;
	}
	else
	{
		updateGridAtPos( snake->pGrid, snake->head->data.x, snake->head->data.y, SYM_GRID );

		Segment *temp = snake->head;
		snake->head = snake->head->next;
		snake->head->previous = NULL;

		//printf( "Freed head\n" );

		free( temp );
	}

	snake->size--;
}

SegData getFront( Snake *snake )
{
	return snake->head->data;
}

void printError( Snake *snake )
{
	char *output;
	int i, j, charCount = 0;

	for ( i = 0; i < snake->pGrid->height; i++ )
	{
		for ( j = 0; j < snake->pGrid->width; j++ )
		{
			charCount++;

			if ( !i && !j ) // if this is the first character malloc
			{
				output = (char*)malloc( sizeof(char) * ( charCount + 1 ) );
				output[ i * snake->pGrid->width + j ] = snake->pGrid->grid[i][j];
			}
			else // if this is not the first character, realloc
			{
				output = (char*)realloc( output, sizeof(char) * ( charCount + 1 ) );
				output[ i * snake->pGrid->width + j ] = snake->pGrid->grid[i][j];
			}
		}
	}

	// since we're outputting a manually-built string, we need to manually add a NULL to the end
	output = (char*)realloc( output, sizeof(char) * charCount + 1 );
	output[ charCount ] = '\0';

	printf( "\n\n\n\tScore: %d\n%s\n\n", snake->score, output );

	switch ( snake->error )
	{
	case ERR_OOB_TOP:
		printf( "ERR_OOB_TOP: Snake tried going off the top of the grid\n" );
		break;
	case ERR_OOB_BOTTOM:
		printf( "ERR_OOB_BOTTOM: Snake tried going off the bottom of the grid\n" );
		break;
	case ERR_OOB_LEFT:
		printf( "ERR_OOB_LEFT: Snake tried going off the left of the grid\n" );
		break;
	case ERR_OOB_RIGHT:
		printf( "ERR_OOB_RIGHT: Snake tried going off the right of the grid\n" );
		break;
	case ERR_KEYP_ESC:
		printf( "ERR_KEYP_ESC: User pressed escape, quitting...\n" );
		break;
	case ERR_HIT_ITSELF:
		printf( "ERR_HIT_ITSELF: Snake tried moving over one of its own segments\n" );
		break;
	default:
		printf( "An error has occurred for which there is no error message\n" );
		break;
	}

	printf( "\n" );

	free( output );
}

char getNextChar( Snake *snake )
{
	if ( !snake->tail )
	{
		fprintf( stderr, "Tail is NULL, cannot retrieve next character\n" );
		return '\0';
	}
	else
	{
		switch ( snake->dir )
		{
		case DIR_UP:
			return getCharAtPos( snake->pGrid, snake->tail->data.x, snake->tail->data.y - 1 );
		case DIR_DOWN:
			return getCharAtPos( snake->pGrid, snake->tail->data.x, snake->tail->data.y + 1 );
		case DIR_LEFT:
			return getCharAtPos( snake->pGrid, snake->tail->data.x - 1, snake->tail->data.y );
		case DIR_RIGHT:
			return getCharAtPos( snake->pGrid, snake->tail->data.x + 1, snake->tail->data.y );
		default:
			return '\0';
		}
	}
}

int snakeHasSegmentAtPos( Snake *snake, int x, int y )
{
	Segment *temp = snake->head;
	while ( temp != NULL )
	{
		if ( temp->data.x == x && temp->data.y == y )
			return 1;
		temp = temp->next;
	}

	return 0;
}

int snakeWillHitItself( Snake *snake )
{
	if ( !snake->tail )
	{
		fprintf( stderr, "Tail is NULL, cannot determine if snake will hit itself\n" );
		return 0;
	}
	else
	{
		switch ( snake->dir )
		{
		case DIR_UP:
			return snakeHasSegmentAtPos( snake, snake->tail->data.x, snake->tail->data.y - 1 );
		case DIR_DOWN:
			return snakeHasSegmentAtPos( snake, snake->tail->data.x, snake->tail->data.y + 1 );
		case DIR_LEFT:
			return snakeHasSegmentAtPos( snake, snake->tail->data.x - 1, snake->tail->data.y );
		case DIR_RIGHT:
			return snakeHasSegmentAtPos( snake, snake->tail->data.x + 1, snake->tail->data.y );
		default:
			return 1;
		}
	}
}

void generateSpecificPickupAtRandomLocation( Snake *snake, char pickupType )
{
	if ( containsPickup( snake->pGrid, pickupType ) )
		return;

	unsigned randX = -1, randY = -1;
	while ( ( invalidX( snake->pGrid, randX ) && invalidY( snake->pGrid, randY ) ) || snakeHasSegmentAtPos( snake, randX, randY ) )
	{
		randX = rand() % snake->pGrid->width - 1;
		randY = rand() % snake->pGrid->height;
	}

	updateGridAtPos( snake->pGrid, randX, randY, pickupType );
}

void generateRandomPickupAtRandomLocation( Snake *snake )
{
	unsigned randX = -1, randY = -1;
	char randomChar, randC;
	while ( ( invalidX( snake->pGrid, randX ) && invalidY( snake->pGrid, randY ) ) || snakeHasSegmentAtPos( snake, randX, randY ) )
	{
		randX = rand() % snake->pGrid->width - 1;
		randY = rand() % snake->pGrid->height;
		randC = rand() % 5;
	}

	switch ( randC )
	{
	case 0:
		randomChar = PICKUP_FOOD;
		break;
	case 1:
		randomChar = PICKUP_CULL;
		break;
	case 2:
		randomChar = PICKUP_NORM;
		break;
	case 3:
		randomChar = PICKUP_FAST;
		break;
	case 4:
		randomChar = PICKUP_SLOW;
		break;
	}

	if ( containsPickup( snake->pGrid, randomChar ) )
		return;

	updateGridAtPos( snake->pGrid, randX, randY, randomChar );
}