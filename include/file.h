#ifndef _FILE_H_
#define _FILE_H_

typedef struct file File;

tile valeur (File *f);

File** creerFile();

void enfiler (File **f, tile t);

tile defiler (File **f);

void detruireFile(File **f);


#endif
