#ifndef OTHER_FUNCTIONS_H_INCLUDED
#define OTHER_FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
#include "TextureLoader.h"
#include "other_functions.h"
#include "file_functions.h"
#include "map_generator.h"
int menu_button(int x, int y,int current);
void main_menu_rect_init(SDL_Rect* rect);
int custom_game_button(int x,int y);
void render_territory(SDL_Renderer* renderer,struct territory_struct* inp,SDL_Texture** shape_t,SDL_Texture** barracks_t,TTF_Font* game_font,struct map* game,SDL_Texture** potions);
void troop_production(struct territory_struct* inp,struct map* game);
int select_barracks(struct map* game_map,int x,int y);
void set_troops(struct territory_struct* inp,struct troops_struct* troops,int from,int to);
void render_troops(struct troops_struct* troops,struct map* game,SDL_Renderer* renderer,int frame,Mix_Chunk** sound_effect,SDL_Texture** t_t);
void AI(struct map* game,struct troops_struct* troops);
void random_potion(struct potion_struct* potions,struct map* game,int *total);
void render_potion(struct potion_struct* potions,struct troops_struct* troops,struct map* game,SDL_Renderer* renderer,int* total,Mix_Chunk** sound_effect,SDL_Texture** t,SDL_Texture** barracks_texture,SDL_Texture** troops_texture);
void set_up_game(int is_elf,int is_orc,int is_undead,struct map* game);
int is_over(struct map* game,struct troops_struct* troops_list,SDL_Renderer* renderer,TTF_Font* font,int is_elf,int is_orc,int is_undead,Mix_Chunk** sound_effect);
void in_game_menu(SDL_Renderer * renderer,struct map* game,int * x,struct troops_struct* troops,struct potion_struct*po,int total_p,char name[40]);


#endif // OTHER_FUNCTIONS_H_INCLUDED
