#include "Snake.h"

// 25 rows per window
// 80 characters per line

void tick( Snake *snake )
{
	if ( !containsFood( snake->pGrid ) )
		generateSpecificPickupAtRandomLocation( snake, PICKUP_FOOD );

	if ( _kbhit() )
	{
		int key = _getch();
		switch ( key )
		{
		case DIR_UP:
			if ( snake->dir != DIR_DOWN ) snake->dir = key;
			break;
		case DIR_DOWN:
			if ( snake->dir != DIR_UP ) snake->dir = key;
			break;
		case DIR_LEFT:
			if ( snake->dir != DIR_RIGHT ) snake->dir = key;
			break;
		case DIR_RIGHT:
			if ( snake->dir != DIR_LEFT ) snake->dir = key;
			break;
		case KEY_ESCAPE:
			snake->error = key;
			return;
		case KEY_SPACE:
			generateRandomPickupAtRandomLocation( snake );
			break;
		}
	}

	if ( snakeWillHitItself( snake ) )
	{
		snake->error = ERR_HIT_ITSELF;
		return;
	}

	if ( snake->head != NULL )
	{
		int i, size, fastOrSlow, chance;
		switch ( getNextChar( snake ) )
		{
		case PICKUP_FOOD:
			snake->skipRemove = 1;
			snake->score++;
			generateSpecificPickupAtRandomLocation( snake, PICKUP_FOOD );
			chance = rand() % 5;
			if ( chance < 1 ) // one in 5 chance of generating random pickup
			{
				int prob;
				prob = rand() % 3;
				if ( !prob )
					generateSpecificPickupAtRandomLocation( snake, PICKUP_CULL );
				else if ( prob == 1 )
					generateSpecificPickupAtRandomLocation( snake, PICKUP_FAST );
				else
					generateSpecificPickupAtRandomLocation( snake, PICKUP_SLOW );
			}
			break;
		case PICKUP_CULL:
			size = snake->size / 2;
			for ( i = 0; i < size; i++ )
				removeFromHead( snake );
			break;
		case PICKUP_NORM:
			snake->speed = SPEED_NORMAL;
			fastOrSlow = rand() % 2;
			if ( fastOrSlow )
				generateSpecificPickupAtRandomLocation( snake, PICKUP_SLOW );
			else
				generateSpecificPickupAtRandomLocation( snake, PICKUP_FAST );
			break;
		case PICKUP_FAST:
			snake->speed = SPEED_FAST;
			generateSpecificPickupAtRandomLocation( snake, PICKUP_NORM );
			break;
		case PICKUP_SLOW:
			snake->speed = SPEED_SLOW;
			generateSpecificPickupAtRandomLocation( snake, PICKUP_NORM );
			break;
		}
	}

	insertAtTail( snake );

	if ( snake->skipRemove )
		snake->skipRemove = 0;
	else
		removeFromHead( snake );

	printGrid( snake->pGrid, snake->score );
}

int main( int argc, char* argv[] )
{
	srand( (unsigned)time(NULL) );

	Snake *snake = initSnake( 25 );

	generateSpecificPickupAtRandomLocation( snake, PICKUP_FOOD );

	while ( !snake->error )
	{
		Sleep( snake->speed );
			
		tick( snake );
	}

	//if ( snake->error )
		//printError( snake );

	printf( "Press any key to continue . . . " );
	_getch();

	return 0;
}