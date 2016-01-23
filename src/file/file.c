#include <stdio.h>
#include <stdlib.h>
#include <grid.h>
#include <file.h>

struct file
{
  tile t;
  struct file *suivant;
};

File **
creerFile ()
{
  File **f = malloc (sizeof (struct file));
  return f;
}

void
enfiler (File ** f, tile t)
{
  File *f_new = malloc (sizeof *f_new);
  if (f_new != NULL)
    {
      f_new->suivant = NULL;
      f_new->t = t;
      if (*f == NULL)
	{
	  *f = f_new;
	}
      else
	{
	  File *f_tmp = *f;
	  while (f_tmp->suivant != NULL)
	    {
	      f_tmp = f_tmp->suivant;
	    }
	  f_tmp->suivant = f_new;
	}
    }
}

tile
defiler (File ** f)
{
  tile ret;

  if (*f != NULL)
    {
      File *f_tmp = (*f)->suivant;
      ret = (*f)->t;
      free (*f), *f = NULL;
      *f = f_tmp;
    }
  return ret;
}

void
detruireFile (File ** f)
{
  while (*f != NULL)
    {
      defiler (f);
    }
}

tile
valeur (File * f)
{
  tile ret;
  if (f != NULL)
    {
      ret = f->t;
    }
  return ret;
}
