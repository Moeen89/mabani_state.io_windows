#ifndef TEXTURELOADER_H_INCLUDED
#define TEXTURELOADER_H_INCLUDED

#include <SDL2/SDL.h>

#include<SDL_ttf.h>
#include<stdbool.h>
#include<SDL_mixer.h>
SDL_Texture* loadTexture(SDL_Renderer* renderer,char path[]);
SDL_Texture* textLoader(char string[],SDL_Color text_color, TTF_Font *font,SDL_Renderer* renderer);
SDL_Texture** barracks_loader(SDL_Renderer* renderer);
SDL_Texture** shape_loader(SDL_Renderer* renderer);
SDL_Texture** potion_loader(SDL_Renderer* renderer);
SDL_Texture** troops_loader(SDL_Renderer* renderer);
SDL_Texture** button_loader(SDL_Renderer* renderer);
SDL_Texture** menu_background_loader(SDL_Renderer* renderer);
SDL_Texture* is_save_available(SDL_Renderer* renderer ,char name[40],int *is_saved_game);
bool name_input(SDL_Event* events,char name[40],int *i,int* shift);
Mix_Chunk** load_sound_effect();

#endif // TEXTURELOADER_H_INCLUDED
