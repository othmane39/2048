#include <grid.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
  grid testG = new_grid();
  printf("grid created\n");

  /**
   * Filling like this
   *
   *		        X
   *               0  1    ... GRID_SIDE-1
   *		+--+--+- ... -+--+
   *		| k| k|  ...  + k| 0
   *		+--+--+- ... -+--+
   *		| k| k|  ...  | k| 1
   *      	+--+--+- ... -+--+         Y
   *		...      ...       ...
   *		+--+--+- ... -+--+
   *		|  |  |  ...  |  | GRID_SIDE-1
   *		+--+--+- ... -+--+
   * The bottom line is left empty
   */
  int k = 1;
  for(int j = 0; j < GRID_SIDE - 1; j++)
    {
      for(int i = 0; i < GRID_SIDE-1; i++)
	{
	  set_tile(testG, i, j, (tile)k);
	  k++;
	}
    }

  //_move
  
  delete_grid(testG);
  printf("grid deleted\n");

  return EXIT_SUCCESS;
}
