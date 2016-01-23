#include <grid.h>
#include <ncurses.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef enum key_e
{ ARROW_UP, ARROW_DOWN, ARROW_LEFT, ARROW_RIGHT, QUIT, OTHER } key;

void print_welcome_message ();
/* Prints the welcome message */

void print_grid (grid g);
/* Prints the grid, param is the grid to be displayed d. */

key get_keyboard_key ();
/* Waits for a key to be pressed, if it's a directional arrow, it returnr a dir
   It can return QUITf if the quit key is pressed. */

void print_end_message (unsigned int);
/* Prints the gameover message. The parameter is the final score to be displayed on it */

bool is_directional_key (key k);
/* Checks if the pressed and then given key is a directional (arrow) key. Returns true if so. */

dir convert_directional_key_to_direction (key k);
/* Convert the key k to a dir. The parameter k is the key to be converted */

char *sprintf_5_block (unsigned long int a);
/* return a char with a such as it take 5 location */

int
main (int argc, char *argv[])
{
  WINDOW *mainwin;
  mainwin = initscr ();		/* Start curses mode            */
  start_color ();
  keypad (mainwin, TRUE);
  noecho ();

  print_welcome_message ();

  grid g = new_grid ();		/* Init the grid                  */

  srand (time (NULL));
  add_tile (g);
  add_tile (g);

  while (!game_over (g))
    {
      print_grid (g);

      key pressedKey = get_keyboard_key ();
      if (is_directional_key (pressedKey))
	{

	  dir movement = convert_directional_key_to_direction (pressedKey);
	  play (g, movement);
	}
      else
	{
	  if (pressedKey == QUIT)
	    {
	      break;
	    }
	}

    }

  unsigned int finalScore = grid_score (g);
  print_end_message (finalScore);

  delete_grid (g);		/* Destroy the grid               */

  delwin (mainwin);
  endwin();			/* End curses mode                */

  return EXIT_SUCCESS;
}

char *
sprintf_5_block (unsigned long int a)
{
  int tmp;
  char *s = malloc (5 * sizeof (char *));
  char *s1 = malloc (5 * sizeof (char *));

  assert(s != NULL);
  assert(s1 != NULL);

  strcpy (s, "");
  strcpy (s1, "");

  for (int i = 10000; i > 1; i /= 10)
    {
      tmp = a / i;
      if (tmp == 0)
	{
	  strcat (s, " ");
	}
      else
	{
	  break;
	}
    }
  sprintf (s1, "%ld", a);
  strcat (s, s1);

  free (s1);
  return s;
}


void
print_grid (grid g)
{
  clear ();

  int c;
  init_pair (0, COLOR_RED, COLOR_BLACK);
  init_pair (1, COLOR_YELLOW, COLOR_BLACK);
  init_pair (2, COLOR_GREEN, COLOR_BLACK);
  init_pair (3, COLOR_WHITE, COLOR_BLACK);
  init_pair (4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair (5, COLOR_CYAN, COLOR_BLACK);
  init_pair (6, COLOR_RED, COLOR_BLACK);
  init_pair (7, COLOR_BLUE, COLOR_BLACK);
  init_pair (8, COLOR_YELLOW, COLOR_BLACK);
  init_pair (9, COLOR_GREEN, COLOR_BLACK);
  init_pair (10, COLOR_YELLOW, COLOR_BLACK);
  init_pair (11, COLOR_WHITE, COLOR_BLACK);

  for (int y = 0; y < GRID_SIDE; y++)
    {
      for (int x = 0; x < GRID_SIDE; x++)
	{
	  printw ("+-----");
	}
      printw ("+\n");
      for (int x = 0; x < GRID_SIDE; x++)
	{
	  if (get_tile (g, x, y) == 0)
	    {
	      attron (COLOR_PAIR (11));
	      printw ("|     ");
	    }
	  else
	    {
	      printw ("|");
	      c = get_tile (g, x, y);
	      attron (COLOR_PAIR ((c%11)));
	      char *formatedNumber =
		sprintf_5_block ((unsigned long int)
				 pow (2, get_tile (g, x, y)));
	      printw ("%s", formatedNumber);
	      free (formatedNumber);
	      attron (COLOR_PAIR (11));
	    }
	}
      printw ("|\n");
    }
  for (int x = 0; x < GRID_SIDE; x++)
    {
      printw ("+-----");
    }
  printw ("+\n");
  printw ("score: %d\n", grid_score (g));

  refresh ();
}

key
get_keyboard_key ()
{
  switch (getch ())
    {
    case KEY_UP:
      return ARROW_UP;
    case KEY_DOWN:
      return ARROW_DOWN;
    case KEY_RIGHT:
      return ARROW_RIGHT;
    case KEY_LEFT:
      return ARROW_LEFT;
    case 'q':
      return QUIT;
    default:
      return get_keyboard_key ();
    }
}

bool
is_directional_key (key k)
{
  if (k == ARROW_UP || k == ARROW_DOWN || k == ARROW_LEFT || k == ARROW_RIGHT)
    return true;
  else
    return false;
}

dir
convert_directional_key_to_direction (key k)
{
  switch (k)
    {
    case ARROW_UP:
      return UP;
    case ARROW_DOWN:
      return DOWN;
    case ARROW_LEFT:
      return LEFT;
    case ARROW_RIGHT:
      return RIGHT;
    default:
      return DOWN;
    }
}

void
print_welcome_message ()
{
  
  printw ("2048 - Press any key to start.\nYou can quit the game by pressing 'q'.");
  refresh ();			/* Print it on to the real screen */
  getch ();			/* Wait for user input            */
}

void
print_end_message (unsigned int finalScore)
{
  clear ();
  printw ("GAME OVER!!\nScore: %d\nCopyright OTA(c).", finalScore);
  refresh ();
  getch ();
}
