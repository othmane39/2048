#include <grid.h>
#include <stdio.h>

int main()
{
  grid testG = new_grid();
  printf("grid created\n");

  delete_grid(testG);
  printf("grid deleted\n");

  return 0;
}

  
