#include <grid.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  grid testG = new_grid();
  printf("grid created\n");

  tile testTile = get_tile(testG, 1, 1);
  if(testTile != (tile)0)
    return EXIT_FAILURE;
  else
    printf("tile[1][1] : %d\n", (tile)testTile);

  set_tile(testG, 1, 1, 11);
  printf("tile[1][1] = 2^11\n");

  testTile = get_tile(testG, 1, 1);
  if(testTile != (tile)11)
    return EXIT_FAILURE;
  else
    printf("tile[1][1] : %d\n", (tile)testTile);
    
  delete_grid(testG);
  printf("grid deleted\n");

  return EXIT_SUCCESS;
}
