#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct memory_s *memory;

dir play_move(strategy, grid);
/* The main part of the algorithm, makes the decision for the movement to choose*/
void free_strategy(strategy);
/* Frees the memory of all the strategy's module*/
memory create_memory();
/* Creates the memory with autimatic memory gestion */
void save_stage(memory, grid, dir);
/* Saves a stage to memory. Saves the actual grid and the choosen direction */
void free_memory(memory);
/* Frees all the saved stages*/

// DEBUG
void print_grid(grid g);

typedef struct tour_s *tour;

struct tour_s
{
  grid g; // the grid state at this stage
  dir decision; // the decision taken for this stage 
};

struct memory_s
{
  int nbStages;
  int maxStages;
  tour* container;
};
  
// Returns the strategy object to tournament program
strategy A3_elmassari_lequerec_negueloua_efficient()
{
  srand(time(NULL));
  strategy test = malloc(sizeof(struct strategy_s));
  test->mem = create_memory();
  test->name = "strategy1";
  test->play_move = &(play_move);
  test->free_strategy = &(free_strategy);
  return test;
}

dir play_move(strategy s, grid g)
{
  // If precedent move was right, we put all left again
  /*if(((memory)(s->mem))->nbStages > 0 && (((((memory)(s->mem))->container)[((memory)(s->mem))->nbStages])->decision) == RIGHT && can_move(g, LEFT))
    {
      printf("test\n");
      save_stage(s->mem, g, LEFT);
      print_grid(g);
      return LEFT;
      }*/

  if(((memory)(s->mem))->nbStages >=0)
    {
      //int temp = ((memory)(s->mem))->nbStages;
      //printf("Test : [%d]%p\n", temp, (((memory)(s->mem))->container[5]));
      printf("Base memory adress : %p.\n", (memory)(s->mem));
      printf("Base container adress : %p.\n", ((memory)(s->mem))->container);
      printf("Element at [%d] : %p.\n", ((memory)(s->mem))->nbStages, (((memory)(s->mem))->container)[(int)(((memory)(s->mem))->nbStages)]);
    }
  //printf("Test 2 : %d\n", ((memory)(s->mem))->nbStages);

  if(can_move(g, DOWN))
    {
      save_stage(s->mem, g, DOWN);
      print_grid(g);
      return DOWN;
    }
  if(can_move(g, LEFT))
    {
      save_stage(s->mem, g, LEFT);
      print_grid(g);
      return LEFT;
    }
  if(can_move(g, RIGHT))
    {
      save_stage(s->mem, g, RIGHT);
      print_grid(g);
      return RIGHT;
    }
  if(can_move(g, UP))
    {
      save_stage(s->mem, g, UP);
      print_grid(g);
      return UP;
    }
}

void free_strategy(strategy s)
{
  printf("Freeing allocated memory.\n");
  free(s);
}

memory create_memory()
{
  memory new = malloc(sizeof(struct memory_s));
  tour* container = malloc(sizeof(struct tour_s) * 1000);
  if(container != NULL) // the memory allcoation worked
    {
      new->container = container;
      new->maxStages = 1000;
      new->nbStages = 0;
    }
}
 
void save_stage(memory mem, grid g, dir d)
{
  tour newStage = malloc(sizeof(struct tour_s));
  if(newStage != NULL) // the allocation worked
    {
      grid gridToSave = new_grid();
      copy_grid(g, gridToSave);
      newStage->g = gridToSave;
      newStage->decision = d;
      if(mem->nbStages < mem->maxStages)
	{
	  (mem->container)[mem->nbStages] = newStage;
	  (mem->nbStages)++;
	}
    }
}

void free_memory(memory mem)
{
  int i;
  for(i = 0; i < mem->nbStages; i++)
    {
      delete_grid(((mem->container)[i])->g);
      free((mem->container)[i]);
    }
  for(i = mem->nbStages; i < mem->maxStages; i++)
    {
      free((mem->container)[i]);
    }
  free(mem);
}
