#include <grid.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
  grid testG = new_grid();
  printf("grid created\n");

  add_tile(testG);
  
  bool isGameOver = game_over(testG);
  
  if(isGameOver) // check if an empty grid is a gameover
    {
      return EXIT_FAILURE;
    }
  else
    printf("game not over with an empty grid\n");

  // FILLING THE GRID WITH UNIQUE VALUES
  int k = 1;
  for(int i = 0; i < GRID_SIDE; i++)
    {
      for(int j = 0; j < GRID_SIDE; j++)
	{
	  set_tile(testG, i, j, (tile)k);
	  printf("[%d][%d]:%d\n", i, j, get_tile(testG, i, j));
	  k++;
	}
    }

  isGameOver = game_over(testG);
  if(!isGameOver)
    return EXIT_FAILURE; // check if it's game over with a grid filled with unique values
  else
    printf("gameover with a grid filled with unique values\n");
  
  delete_grid(testG);
  printf("grid deleted\n");

  return EXIT_SUCCESS;
}
