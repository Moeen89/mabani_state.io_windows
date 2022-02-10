#ifndef FILE_FUNCTIONS_H_INCLUDED
#define FILE_FUNCTIONS_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
int scores(SDL_Texture* t[4],SDL_Renderer* renderer,TTF_Font* font,SDL_Color *y);
int player_in_map(int n);
struct map loading_map(int map_number);
void save_game(struct map* game,struct troops_struct* troops_list,struct potion_struct* potion_list,int potion,char name[40]);
void load_saved_game(struct map* game,struct troops_struct* troops_list,struct potion_struct* potion_list,int* potion);
void update_score(char name[40],int score);
#endif // FILE_FUNCTIONS_H_INCLUDED
