#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>



strategy A3_elmassari_lequerec_negueloua_fast(){
    strategy s = malloc(sizeof(struct strategy_s));
    s->mem = NULL;
    s->name = malloc(30*sizeof(char*));
    strcpy(s->name, "A3_elmassari_lequerec_negueloua");
    s->play_move = best_dir;
    s->free_strategy = free_memless_strat;
    return s;
}
void free_memless_strat (strategy strat){
    free(strat);
}


long get_val_dir(grid g, dir d){
    
    long long val = 0;
    grid g_dir = new_grid();
    long long t_coef[16]= {2147483648, 536870912, 33554432, 2097152, 134217728, 8388608, 524288, 131072, 32, 512, 8192, 32768, 2, 8, 128, 2048};
    
    tile t_data[16];
  
    copy_grid(g, g_dir);
    do_move(g_dir, d);
    
    for (int y=0; y<GRID_SIDE; y++) {
        for (int x=0; x<GRID_SIDE; x++) {
            t_data[y*4+x] = get_tile(g_dir, x, y);
        }
    }
    
    for (int i=0; i<16; i++) {
      val += t_coef[i] *  t_data[i] * t_data[i];
    }
    
    return val;
}

dir best_dir(strategy s, grid g){
    long long vals[4] = {0,0,0,0};
    int i=0;
    for (dir d=UP; d<=RIGHT; d++){
        if (can_move(g, d)) {
            vals[i] = get_val_dir(g, d);
        }
        else vals[i] = -1;
        i++;
    }
    
    long long max = vals[0];
   
    for (int i=1; i<4; i++) {
        if (max<vals[i])
            max = vals[i];
        
    }
    
   if (get_tile(g, 3, 0) != 0 && get_tile(g, 2, 1) != 0 && get_tile(g, 1, 2) != 0 && get_tile(g, 0, 3) != 0) {
        if (can_move(g, RIGHT)) {
            return RIGHT;
        }
    }
    
    for (int i=0; i<4; i++) {
        if (max == vals[i]) {
            switch (i) {
                case 0:
                    
                    return UP;
                    
                case 1:
                    
                    return LEFT;
                    
                case 2:
                    
                    return DOWN;
                    
                case 3:
                    
                    return RIGHT;
                    
                default:
                    break;
            }
        }
    }
    return UP;
}


