#include <grid.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
  grid testG = new_grid();
  printf("grid created\n");

  set_tile(testG, 0,0, (tile) 1);
  set_tile(testG,1,0,(tile)1);
  //initialized good situation for left mouvement
  if (can_move(testG, LEFT)){
    printf("we can do Left mouvement\n");
    do_move(testG,LEFT);
  }
  else{
    return EXIT_FAILURE;
  }
  //test if the mouvement is good
  if (get_tile(testG,0,0)==(tile)2){
    printf("the mouvement is good\n");
  }
  else{
    return EXIT_FAILURE;
  }
  set_tile(testG,1,0,(tile)2);
  //we'll do a play mouvement
  play (testG,LEFT);
  //test if the play is good
  if (get_tile(testG,0,0)==(tile)3){
    printf("the play is good\n");
  }
  else{
    return EXIT_FAILURE;
  }
  printf("all is allright\n");
  return 0;
}
 
