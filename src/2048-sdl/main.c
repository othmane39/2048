#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <grid.h>
#include <unistd.h> // getlogin_r
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "score.h"

#include "img/logo.bmp.h"
#include "img/grid.bmp.h"
#include "img/2048-2.bmp.h"
#include "img/2048-4.bmp.h"
#include "img/2048-8.bmp.h"
#include "img/2048-16.bmp.h"
#include "img/2048-32.bmp.h"
#include "img/2048-64.bmp.h"
#include "img/2048-128.bmp.h"
#include "img/2048-256.bmp.h"
#include "img/2048-512.bmp.h"
#include "img/2048-1024.bmp.h"
#include "img/2048-2048.bmp.h"
#include "img/2048-4096.bmp.h"
#include "img/2048-8192.bmp.h"
#include "img/2048-16384.bmp.h"
#include "img/2048-32768.bmp.h"

#include "font/Megalopolis.ttf.h"
#include "font/UbuntuTitle.ttf.h"

#define BASE_H 550
#define BASE_W 305
#define POS_GRID_X 20
#define POS_GRID_Y 180
#define MARGE 5
#define SIZE_CUBE 60
#define PLAY_BUTTON_X 70
#define PLAY_BUTTON_Y 300
#define QUIT_BUTTON_X 50
#define QUIT_BUTTON_Y 400

#define BACK_COLOR_R 244
#define BACK_COLOR_G 242
#define BACK_COLOR_B 204

void print_entry(SDL_Surface* base);
void entry_loop(SDL_Surface* base);
void game_loop(SDL_Surface* base);
void print_elem_grid(grid g, SDL_Surface* base, SDL_Surface** c);
void free_grid_blocks(SDL_Surface** c);
void draw_empty_grid(SDL_Surface* base, SDL_Surface* g);
void print_final(SDL_Surface* base, grid g);
void final_loop(SDL_Surface* base, grid g);
char* convert_score_to_string(int score);
void draw_score_block(grid g, SDL_Surface* base);
SDL_Surface* get_cube_surf(int a);
void set_highscore(int a);
void print_highscore(SDL_Surface* base);


int main(int argc, char* argv[]){
  SDL_Surface* base = NULL;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  base = SDL_SetVideoMode(BASE_W, BASE_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
  SDL_WM_SetCaption("2048 by OTA", NULL);

  entry_loop(base);

  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}

void print_entry(SDL_Surface* base){
  SDL_RWops* megalopolis_font_raw = SDL_RWFromMem(megalopolisFont, megalopolisFontSize);
  TTF_Font* police = TTF_OpenFontRW(megalopolis_font_raw, 1, 34);
  SDL_Color noir = {0, 0, 0};
  SDL_Color rouge = {255, 0, 0};
  SDL_Color bleu = {0, 0, 255};
  SDL_Color arr = {244, 242, 204};
  SDL_Rect pos_t2048, pos_play, pos_quit;
  SDL_Surface* t2048 = TTF_RenderText_Shaded(police, "2048", bleu, arr);
  SDL_Surface* play = TTF_RenderText_Shaded(police, "JOUER", rouge, arr);
  SDL_Surface* quit = TTF_RenderText_Shaded(police, "QUITTER", noir, arr);


  SDL_FillRect(base, NULL, SDL_MapRGB(base->format, BACK_COLOR_R, BACK_COLOR_G, BACK_COLOR_B));
  
  pos_play.x = PLAY_BUTTON_X;
  pos_play.y = PLAY_BUTTON_Y;

  pos_t2048.x = 90;
  pos_t2048.y = 150;

  pos_quit.x = QUIT_BUTTON_X;
  pos_quit.y = QUIT_BUTTON_Y;
  
  SDL_BlitSurface(t2048, NULL, base, &pos_t2048);
  SDL_BlitSurface(play, NULL, base, &pos_play);
  SDL_BlitSurface(quit, NULL, base, &pos_quit);
  print_highscore(base);
  SDL_Flip(base);
  SDL_FreeSurface(t2048);
  SDL_FreeSurface(play);
  SDL_FreeSurface(quit);
  TTF_CloseFont(police);
}

void entry_loop(SDL_Surface* base){
  int c = 1;
  SDL_Event ev;
  
  print_entry(base);


  while (c){
    SDL_WaitEvent(&ev);
    switch(ev.type)
      {
      case SDL_QUIT:
	c = 0;
	break;
      case SDL_KEYDOWN:
	switch(ev.key.keysym.sym)
	  {
	  case SDLK_ESCAPE:
	    c = 0;
	  default:
	    break;
	  }
      case SDL_MOUSEBUTTONUP:
	if(ev.button.x > PLAY_BUTTON_X && ev.button.x < 225 && ev.button.y > PLAY_BUTTON_Y && ev.button.y < 368){
	  c = 0;
	  game_loop(base);
	}
	else if (ev.button.x > QUIT_BUTTON_X && ev.button.x < 256 && ev.button.y > QUIT_BUTTON_Y && ev.button.y < 468)
	  c = 0;
	break;
      default:
	break;

      }
  }
}

void game_loop(SDL_Surface* base){
  SDL_Surface* grille = NULL;
  SDL_Surface* logo = NULL;
  SDL_Rect pos_grille;
  SDL_Rect pos_logo;
  int continuer = 1;
  SDL_Event event;
  SDL_Surface* c[16];

  grid g = new_grid();
  add_tile(g);
  add_tile(g);

  SDL_RWops* gridRaw = SDL_RWFromMem(rawImageGrid, rawImageSizeGrid);
  grille = SDL_LoadBMP_RW(gridRaw, 1);
  SDL_RWops* logoRaw = SDL_RWFromMem(logo_raw_image, logo_raw_image_size);
  logo = SDL_LoadBMP_RW(logoRaw, 1);
  assert(grille != NULL && logo != NULL);

  SDL_SetColorKey(logo, SDL_SRCCOLORKEY, SDL_MapRGB(logo->format, 255, 255, 255));
  SDL_SetColorKey(grille, SDL_SRCCOLORKEY, SDL_MapRGB(grille->format, 255, 255, 255));
  
  pos_grille.x = POS_GRID_X;
  pos_grille.y = POS_GRID_Y;
  
  pos_logo.x = 20;
  pos_logo.y = 20;

  SDL_FillRect(base, NULL, SDL_MapRGB(base->format, BACK_COLOR_R, BACK_COLOR_G, BACK_COLOR_B));
  SDL_BlitSurface(grille, NULL, base, &pos_grille);
  SDL_BlitSurface(logo, NULL, base, &pos_logo);
  print_highscore(base);
  draw_score_block(g, base);
  print_elem_grid(g, base, c);
  free_grid_blocks(c);
  SDL_Flip(base);

  while (continuer && !game_over(g))
    {
      SDL_WaitEvent(&event);
      switch(event.type)
	{
	case SDL_QUIT:
	  continuer =0;
	  break;
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
            {
	    case SDLK_ESCAPE:
	      continuer=0;
	      break;
	    case SDLK_UP:
	      draw_empty_grid(base, grille); // A VERIFIER REMPLACEMENT PAR BLIT
	      play(g, UP);
	      print_elem_grid(g, base, c);
	      draw_score_block(g, base);
	      free_grid_blocks(c);
	      SDL_Flip(base);
	      break;
	    case SDLK_DOWN:
	      draw_empty_grid(base, grille);
	      play(g, DOWN);
	      print_elem_grid(g, base, c);
	      draw_score_block(g, base);
	      free_grid_blocks(c);
	      SDL_Flip(base);
	      break;
	    case SDLK_RIGHT:
	      draw_empty_grid(base, grille);
	      play(g, RIGHT);
	      print_elem_grid(g, base, c);
	      draw_score_block(g, base);
	      free_grid_blocks(c);
	      SDL_Flip(base);
	      break;
	    case SDLK_LEFT:;
	      draw_empty_grid(base, grille);
	      play(g, LEFT);
	      print_elem_grid(g, base, c);
	      draw_score_block(g, base);
	      free_grid_blocks(c);
	      SDL_Flip(base);
	      break;
	    default:
	      break;
            }
	  break;
	}
      
    }
  if (continuer)
    final_loop(base, g);
  
  delete_grid(g);
  SDL_FreeSurface(grille);
  SDL_FreeSurface(logo);
}

void print_elem_grid(grid g, SDL_Surface* base, SDL_Surface** c){
  SDL_Rect pos;
  
  for(int i=0; i<GRID_SIDE; i++){
    for(int j=0; j<GRID_SIDE; j++){
      c[i*4+j] = get_cube_surf(get_tile(g, i, j));
      if(c[i*4+j] != NULL){
	  pos.x = POS_GRID_X + MARGE*(i+1) + SIZE_CUBE*i;
	  pos.y = POS_GRID_Y + MARGE*(j+1) + SIZE_CUBE*j;
	  SDL_BlitSurface(c[i*4+j], NULL, base, &pos);
      }
    }
  }
}

void free_grid_blocks(SDL_Surface** c){
  for(int i =0; i<16; i++){
    if(c[i]!=NULL){
      SDL_FreeSurface(c[i]);
    } 
  }
}

void draw_empty_grid(SDL_Surface* base, SDL_Surface* g){
  SDL_Rect pos_grille;
  pos_grille.x = POS_GRID_X;
  pos_grille.y = POS_GRID_Y;
  SDL_SetColorKey(g, SDL_SRCCOLORKEY, SDL_MapRGB(g->format, 255, 255, 255));
  
  SDL_BlitSurface(g, NULL, base, &pos_grille);
}
  
void print_final(SDL_Surface* base, grid g){
  SDL_RWops* megalopolis_font_raw = SDL_RWFromMem(megalopolisFont, megalopolisFontSize);
  SDL_RWops* megalopolis_font_raw2 = SDL_RWFromMem(megalopolisFont, megalopolisFontSize);
  TTF_Font* police_text = TTF_OpenFontRW(megalopolis_font_raw, 1, 34);
  TTF_Font* police_score = TTF_OpenFontRW(megalopolis_font_raw2, 1, 25);
  SDL_Color noir = {0, 0, 0};
  SDL_Color red = {255, 0 , 0};
  SDL_Color blue = {0, 0, 255};
  SDL_Color arr = {BACK_COLOR_R, BACK_COLOR_G, BACK_COLOR_B};
  SDL_Color arr_score = {192, 192, 192};
  SDL_Rect pos_go, pos_case1, pos_score, pos_score_val, pos_retry;
  SDL_Surface* go = TTF_RenderText_Shaded(police_text, "GAME OVER.", noir, arr);
  SDL_Surface* score = TTF_RenderText_Shaded(police_score, "SCORE", noir, arr_score);
  char* scoreAsString = convert_score_to_string(grid_score(g));
  SDL_Surface* score_value = TTF_RenderText_Shaded(police_score, scoreAsString, red, arr_score);
  SDL_Surface* case1 = SDL_CreateRGBSurface(0, 280, 115, 32, 0, 0, 0, 0);
  SDL_Surface* retry = TTF_RenderText_Shaded(police_text, "REJOUER", blue, arr);
  SDL_FillRect(case1, NULL, SDL_MapRGB(case1->format, 192, 192, 192));

  SDL_FillRect(base, NULL, SDL_MapRGB(base->format, BACK_COLOR_R, BACK_COLOR_G, BACK_COLOR_B));

  pos_go.x = (305/2)-(go->w/2);
  pos_go.y = 60;

  pos_case1.x = (305/2)-(case1->w/2);
  pos_case1.y = 200;

  pos_score.x = (305/2)-(score->w/2);
  pos_score.y = 210;
  
  pos_score_val.x = (BASE_W/2)-(score_value->w/2);
  pos_score_val.y = pos_score.y+ score->h;
  
  pos_retry.x = (BASE_W/2)-(retry->w/2);
  pos_retry.y = 400;

  SDL_BlitSurface(go, NULL, base, &pos_go);
  SDL_BlitSurface(case1, NULL, base, &pos_case1);
  SDL_BlitSurface(score, NULL, base, &pos_score);
  SDL_BlitSurface(score_value, NULL, base, &pos_score_val);
  SDL_BlitSurface(retry, NULL, base, &pos_retry);
  set_highscore(grid_score(g));
  print_highscore(base);
  SDL_Flip(base);
  free(scoreAsString);
  SDL_FreeSurface(go);
  SDL_FreeSurface(score);
  SDL_FreeSurface(score_value);
  SDL_FreeSurface(case1);
  SDL_FreeSurface(retry);
  TTF_CloseFont(police_text);
  TTF_CloseFont(police_score);
}

void final_loop(SDL_Surface* base, grid g){
  int c = 1;
  SDL_Event ev;

  print_final(base, g);

  while(c){
    SDL_WaitEvent(&ev);
    switch(ev.type){
    case SDL_QUIT:
      c = 0;
      break;
    case SDL_KEYDOWN:
      switch(ev.key.keysym.sym){
      case SDLK_r:
	c = 0;
	game_loop(base);
      default:
	break;
      }
    case SDL_MOUSEBUTTONUP:
      if(ev.button.x > 44 && ev.button.x < 260 && ev.button.y > 400 && ev.button.y < 468){
	c = 0;
	game_loop(base);
      }
      break;
    }
  }
}

char* convert_score_to_string(int score)
{
  char* convertedScore = malloc(sizeof(char) * 15);
  sprintf(convertedScore, "%d", score);
  return convertedScore;
}

void draw_score_block(grid g, SDL_Surface* base)
{
   SDL_RWops* ubuntuTitle_font_raw = SDL_RWFromMem(ubuntuTitleFont, ubuntuTitleFontSize);
  TTF_Font* ubuntuTitleFont = TTF_OpenFontRW(ubuntuTitle_font_raw, 1, 45);

  SDL_Color blackColor = {0, 0, 0};
  SDL_Color blue_s = {33, 98, 140};
  SDL_Color backgroundColor = {192, 192, 192};
  
  SDL_Surface *scoreBlockSurface;
  scoreBlockSurface = SDL_CreateRGBSurface(0, 155, 90, 32, 0, 0, 0, 0);
  SDL_FillRect(scoreBlockSurface, NULL, SDL_MapRGB(scoreBlockSurface->format, 192, 192, 192));
  SDL_Rect scoreBlockPosition;
  scoreBlockPosition.x = 130; // left margin + logo size + margin = 20 + 90 + 20
  scoreBlockPosition.y = 20; // top margin
  /*
  scoreBlockSurface->h = 90; // same height of the logo
  scoreBlockSurface->w = 155; // window size - (left margin + logo size + inter margin + right margin) = 305 - 20 + 90 + 20 + 20 = 155
  */
  SDL_BlitSurface(scoreBlockSurface, NULL, base, &scoreBlockPosition);
  
  SDL_Surface* scoreMessageRectangleSurface = TTF_RenderText_Shaded(ubuntuTitleFont, "SCORE", blackColor, backgroundColor);
  SDL_Rect scoreMessageRectanglePosition;
  scoreMessageRectanglePosition.x = 130 + (0.5 * (155 - scoreMessageRectangleSurface->w));
  scoreMessageRectanglePosition.y = 20;
  SDL_BlitSurface(scoreMessageRectangleSurface, NULL, base, &scoreMessageRectanglePosition);
  
  char* scoreAsString = convert_score_to_string(grid_score(g));
  SDL_Surface* scoreRectangleSurface = TTF_RenderText_Shaded(ubuntuTitleFont, scoreAsString, blue_s, backgroundColor);
  free(scoreAsString);
  SDL_Rect scoreRectanglePosition;

  scoreRectanglePosition.x = 130 + (0.5 * (155 - scoreRectangleSurface->w));
  scoreRectanglePosition.y = 110 - scoreRectangleSurface->h;

  SDL_BlitSurface(scoreRectangleSurface, NULL, base, &scoreRectanglePosition);
  SDL_FreeSurface(scoreBlockSurface);
  SDL_FreeSurface(scoreMessageRectangleSurface);
  SDL_FreeSurface(scoreRectangleSurface);
  TTF_CloseFont(ubuntuTitleFont);
}

SDL_Surface* get_cube_surf(int a){
  if(a == 0)
    return NULL; // if the image to be displayed is 2^0
  else
    {
      unsigned char* available_images[16];
      available_images[1] = rawImage2;
      available_images[2] = rawImage4;
      available_images[3] = rawImage8;
      available_images[4] = rawImage16;
      available_images[5] = rawImage32;
      available_images[6] = rawImage64;
      available_images[7] = rawImage128;
      available_images[8] = rawImage256;
      available_images[9] = rawImage512;
      available_images[10] = rawImage1024;
      available_images[11] = rawImage2048;
      available_images[12] = rawImage4096;
      available_images[13] = rawImage8192;
      available_images[14] = rawImage16384;
      available_images[15] = rawImage32768;

      int available_images_size[16];
      available_images_size[1] = rawImageSize2;
      available_images_size[2] = rawImageSize4;
      available_images_size[3] = rawImageSize8;
      available_images_size[4] = rawImageSize16;
      available_images_size[5] = rawImageSize32;
      available_images_size[6] = rawImageSize64;
      available_images_size[7] = rawImageSize128;
      available_images_size[8] = rawImageSize256;
      available_images_size[9] = rawImageSize512;
      available_images_size[10] = rawImageSize1024;
      available_images_size[11] = rawImageSize2048;
      available_images_size[12] = rawImageSize4096;
      available_images_size[13] = rawImageSize8192;
      available_images_size[14] = rawImageSize16384;
      available_images_size[15] = rawImageSize32768;

      SDL_Surface* image_to_be_displayed = NULL;
      SDL_RWops* image_to_be_displayed_raw = SDL_RWFromMem(available_images[a], available_images_size[a]);
      image_to_be_displayed = SDL_LoadBMP_RW(image_to_be_displayed_raw, 1);
      SDL_SetColorKey(image_to_be_displayed, SDL_SRCCOLORKEY, 10);

      return image_to_be_displayed;
    }  
}

void set_highscore(int s){
  char* username = malloc(sizeof(char) * 25);
  username = getlogin();
  if(username != NULL) // if there is no error reading OS username
    {
    submit_score(s, username);
    }
  else
    submit_score(s, "unknown");
  //free(username);
}

void print_highscore(SDL_Surface* base){
  SDL_RWops* ubuntuTitle_font_raw = SDL_RWFromMem(ubuntuTitleFont, ubuntuTitleFontSize);
  TTF_Font* ubuntuTitleFont = TTF_OpenFontRW(ubuntuTitle_font_raw, 1, 25);
  SDL_Surface* highScore_l1 = NULL;
  SDL_Surface* highScore_l2 = NULL;
  SDL_Color arr = {BACK_COLOR_R, BACK_COLOR_G, BACK_COLOR_B};
  SDL_Color col_text = {120, 0, 1};
  SDL_Rect pos1, pos2;

  char* s1 = malloc(sizeof(char)*100);
  char* s2 = malloc(sizeof(char)*50);
  char* username = malloc(sizeof(char) * 20);
  int bestScore;
  strcpy(s1, "MEILLEUR SCORE : ");
  strcpy(s2, "(");
  if(!get_highScore(&bestScore, &username))
    {
      strcat(s1, convert_score_to_string(bestScore));
      strcat(s2, username);
      strcat(s2, ")");
    }
  else
       strcat(s2, "(OFFLINE)");
  highScore_l1 = TTF_RenderText_Shaded(ubuntuTitleFont, s1, col_text, arr);
  highScore_l2 = TTF_RenderText_Shaded(ubuntuTitleFont, s2, col_text, arr);

  free(s1);
  free(s2);
  //free(username);

  pos1.x = (BASE_W/2)- (highScore_l1->w/2);
  pos1.y = 483;
  
  pos2.x = (BASE_W/2)- (highScore_l2->w/2);
  pos2.y = 483 + highScore_l1->h;

  SDL_BlitSurface(highScore_l1, NULL, base, &pos1);
  SDL_BlitSurface(highScore_l2, NULL, base, &pos2);
  SDL_FreeSurface(highScore_l1);
  SDL_FreeSurface(highScore_l2);
  TTF_CloseFont(ubuntuTitleFont);
}
  
