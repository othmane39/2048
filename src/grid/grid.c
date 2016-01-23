#include <grid.h>
#include <file.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
//CHERQI
struct grid_s
{
  tile matrix[GRID_SIDE][GRID_SIDE];
  unsigned long int score;
};

grid
new_grid ()
//initializing a new grid with size GRID_SIDE and score 0
{
  grid g = malloc (sizeof (struct grid_s));

  assert(g != NULL);

  for (int i = 0; i < GRID_SIDE; i++)
    {
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  set_tile (g, i, j, 0);
	}
    }

  g->score = 0;

  return g;
}

void
delete_grid (grid g)
//freeing memory allocated during the creation of the grid put in parameter g
{
  free (g);
}

void
copy_grid (grid src, grid dst)
{
  for (int i = 0; i < GRID_SIDE; i++)
    {
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  set_tile (dst, i, j, get_tile (src, i, j));
	}
    }
  dst->score = src->score;
}

unsigned long int
grid_score (grid g)
//return score of the grid
{
  return g->score;
}

tile
get_tile (grid g, int x, int y)
//returns the value of the coordinate box (x, y) of the grid g
{
  return g->matrix[x][y];
}

void
set_tile (grid g, int x, int y, tile t)
//attributed the value t to the box coordinate (x, y) of the grid G
{
  g->matrix[x][y] = t;
}

bool
can_move (grid g, dir d)
//returns a Boolean indicating whether the movement direction is possible on the grid g
{
  grid gt = new_grid ();
  copy_grid (g, gt);
  do_move (gt, d);

  bool canMove = false;

  for (int i = 0; i < GRID_SIDE; i++)
    {
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  if (get_tile (g, i, j) != get_tile (gt, i, j))
	    {
	      canMove = true;
	      break;
	    }
	}
    }

  free (gt);

  return canMove;
}


bool
game_over (grid g)
//returns true if no movement is possible on the grid g
{

  for (dir d = UP; d <= RIGHT; d++)
    {
      if (can_move (g, d))
	return false;
    }
  return true;
}

void
add_tile (grid g)
//adds a box at random value 2 or 4 on an empty square of the grid g
{
  int tab[GRID_SIDE * GRID_SIDE];
  int cp = 0;
  for (int i = 0; i < GRID_SIDE; i++)
    {
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  if (get_tile (g, i, j) == 0)
	    {
	      tab[cp] = i * 10 + j;
	      cp++;
	    }
	}
    }

  int b = random_inf_a (cp);
  int block = tab[b];
  int numb = random_inf_a (10);

  if (numb == 0)
    {				//probability 1/10
      set_tile (g, block / 10, block % 10, 2);	//Add new tile with value 4
    }
  else
    {
      set_tile (g, block / 10, block % 10, 1);	//Add new tile with value 2
    }

}

int
random_inf_a (int a)
//returns a random value less than a
{
  return rand () % a;
}

void
play (grid g, dir d)
//performs the movement direction D on the grid g, checking if this movement is possible and adding a random box after movement
{
  if (can_move (g, d))
    {
      do_move (g, d);
      add_tile (g);
    }
}

void
do_move (grid g, dir d)
//performs the movement direction D on the grid g
{
  File *f1 = NULL;
  File **f = &f1;
  tile tmp;
  int axe;
  switch (d)
    {
    case UP:

      for (int i = 0; i < GRID_SIDE; i++)
	{			//extraction
	  for (int j = 0; j < GRID_SIDE; j++)
	    {
	      if (get_tile (g, i, j) != 0)
		{
		  enfiler (f, get_tile (g, i, j));
		  set_tile (g, i, j, 0);
		}

	    }
	  enfiler (f, 0);
	}
      for (int i = 0; i < GRID_SIDE; i++)
	{			//filling
	  axe = 0;
	  while ((tmp = defiler (f)) != 0)
	    {
	      if (tmp == valeur (*f))
		{
		  set_tile (g, i, axe, defiler (f) + 1);
		  g->score +=
		    (unsigned long int) pow (2, get_tile (g, i, axe));
		  axe++;
		}
	      else
		{
		  set_tile (g, i, axe, tmp);
		  axe++;
		}
	    }
	}
      detruireFile (f);
      break;

    case LEFT:
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  for (int i = 0; i < GRID_SIDE; i++)
	    {
	      if (get_tile (g, i, j) != 0)
		{
		  enfiler (f, get_tile (g, i, j));
		  set_tile (g, i, j, 0);
		}
	    }
	  enfiler (f, 0);
	}
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  axe = 0;
	  while ((tmp = defiler (f)) != 0)
	    {
	      if (tmp == valeur (*f))
		{
		  set_tile (g, axe, j, defiler (f) + 1);
		  g->score +=
		    (unsigned long int) pow (2, get_tile (g, axe, j));
		  axe++;
		}
	      else
		{
		  set_tile (g, axe, j, tmp);
		  axe++;
		}
	    }
	}
      detruireFile (f);
      break;

    case DOWN:
      for (int i = 0; i < GRID_SIDE; i++)
	{
	  for (int j = GRID_SIDE - 1; j > -1; j--)
	    {
	      if (get_tile (g, i, j) != 0)
		{
		  enfiler (f, get_tile (g, i, j));
		  set_tile (g, i, j, 0);
		}
	    }
	  enfiler (f, 0);
	}
      for (int i = 0; i < GRID_SIDE; i++)
	{
	  axe = GRID_SIDE - 1;
	  while ((tmp = defiler (f)) != 0)
	    {
	      if (tmp == valeur (*f))
		{
		  set_tile (g, i, axe, defiler (f) + 1);
		  g->score +=
		    (unsigned long int) pow (2, get_tile (g, i, axe));
		  axe--;
		}
	      else
		{
		  set_tile (g, i, axe, tmp);
		  axe--;
		}
	    }
	}
      detruireFile (f);
      break;

    case RIGHT:
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  for (int i = GRID_SIDE - 1; i > -1; i--)
	    {
	      if (get_tile (g, i, j) != 0)
		{
		  enfiler (f, get_tile (g, i, j));
		  set_tile (g, i, j, 0);
		}
	    }
	  enfiler (f, 0);
	}
      for (int j = 0; j < GRID_SIDE; j++)
	{
	  axe = GRID_SIDE - 1;
	  while ((tmp = defiler (f)) != 0)
	    {
	      if (tmp == valeur (*f))
		{
		  set_tile (g, axe, j, defiler (f) + 1);
		  g->score +=
		    (unsigned long int) pow (2, get_tile (g, axe, j));
		  axe--;
		}
	      else
		{
		  set_tile (g, axe, j, tmp);
		  axe--;
		}
	    }
	}
      detruireFile (f);
      break;
    }
}
