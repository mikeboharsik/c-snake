#include "Grid.h"

Grid *initGrid( int width )
{
	int i, j;

	Grid *newGrid = (Grid*)malloc( sizeof( Grid ) );
	if ( !newGrid )
	{
		fprintf( stderr, "Out of memory\n" );
		return NULL;
	}

	newGrid->width = width + 1; // add 1 more for extra newline
	newGrid->height = (int)( (float)((float)2/(float)3) * (float)(newGrid->width - 1) ); // subtract 1 from width because newline isn't really a grid character

	newGrid->grid = (char**)malloc( sizeof(char*) * newGrid->height );
	for ( i = 0; i < newGrid->height; i++ )
	{
		newGrid->grid[i] = (char*)malloc( sizeof(char) * newGrid->width );
		for ( j = 0; j < newGrid->width; j++ )
		{
			if ( j == newGrid->width - 1 )
				newGrid->grid[i][j] = '\0';
			else
				newGrid->grid[i][j] = SYM_GRID;
		}
	}

	return newGrid;
}

void printGrid( Grid *grid, int score )
{
	char *output;
	int i, t;
	int numTabs = 3;

	output = (char*)malloc( sizeof(char) * ( grid->width * grid->height + grid->height + ( grid->height * numTabs ) ) );
	output[0] = '\0';

	for ( i = 0; i < grid->height; i++ )
	{
		for ( t = 0; t < numTabs; t++ )
			strcat( output, "\t" );

		strcat( output, grid->grid[i] );
		strcat( output, "\n" );
	}

	printf( "\n\t\t\t       ASCII SNAKE\n\n\t\t\tScore: %d\n%s\n\n\t      Copyright 2014 Michael Hennessy, donut steal!!\n\n", score, output );

	free( output );
}

void oldPrintGrid( Grid *grid, int score )
{
	char *output;
	int i, j, charCount = 0;

	for ( i = 0; i < grid->height; i++ )
	{
		for ( j = 0; j < grid->width; j++ )
		{
			charCount++;

			if ( !i && !j ) // if this is the first character, malloc
			{
				output = (char*)malloc( sizeof(char) * ( charCount + 1 ) );
				output[ i * grid->width + j ] = grid->grid[i][j];
			}
			else // if this is not the first character, realloc
			{
				output = (char*)realloc( output, sizeof(char) * ( charCount + 1 ) );
				output[ i * grid->width + j ] = grid->grid[i][j];
			}
		}
	}

	// since we're outputting a manually-built string, we need to manually add a NULL to the end
	output = (char*)realloc( output, sizeof(char) * charCount + 1 );
	output[ charCount ] = '\0';

	printf( "\n\n\n\tScore: %d\n%s\n\n\n\n", score, output );

	free( output );
}

void updateGridAtPos( Grid *grid, int x, int y, char newChar )
{
	grid->grid[y][x] = newChar;
}

int invalidX( Grid *grid, int x )
{
	return x < 0 || x > grid->width - 2;
}

int invalidY( Grid *grid, int y )
{
	return y < 0 || y > grid->height - 1;
}

char getCharAtPos( Grid *grid, int x, int y )
{
	if ( invalidX( grid, x ) )
	{
		//printf( "%d is outside the horizontal bounds of the grid\n", x );
		return '\0';
	}
	else if ( invalidY( grid, y ) )
	{
		//printf( "%d is outside the vertical bounds of the grid\n", y );
		return '\0';
	}
	else
		return grid->grid[y][x];
}

int containsPickup( Grid *grid, char pickupType )
{
	int i, j;
	for ( i = 0; i < grid->height; i++ )
	{
		for ( j = 0; j < grid->width; j++ )
		{
			if ( grid->grid[i][j] == pickupType )
				return 1;
		}
	}

	return 0;
}

int containsFood( Grid *grid )
{
	return containsPickup( grid, PICKUP_FOOD );
}