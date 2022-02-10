//
// Created by moein111 on 31.01.22.
//

#include "TextureLoader.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
SDL_Texture* loadTexture(SDL_Renderer* renderer,char path[]){
    SDL_Texture* finalTexture = NULL;
    SDL_Surface* tmp = IMG_Load(path);
    if( tmp == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        finalTexture = SDL_CreateTextureFromSurface( renderer, tmp );
        if(finalTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        //Get rid of old loaded surface
        SDL_FreeSurface( tmp);
    }
    return finalTexture;
}

SDL_Texture* textLoader(char string[],SDL_Color text_color, TTF_Font *font,SDL_Renderer* renderer){
    SDL_Texture* finalTexture = NULL;
    SDL_Surface* text_surface = TTF_RenderText_Solid(font,string,text_color);
    finalTexture = SDL_CreateTextureFromSurface(renderer,text_surface);
    SDL_FreeSurface( text_surface);
    return finalTexture;

}
SDL_Texture** button_loader(SDL_Renderer* renderer){
    SDL_Texture** ret_val =(SDL_Texture**) malloc(sizeof(SDL_Texture*)*5);
    ret_val[0] = loadTexture(renderer,"images/custom game.jpg");
    ret_val[2] = loadTexture(renderer,"images/scoreboard.jpg");
    ret_val[3] = loadTexture(renderer,"images/back.jpg");
    ret_val[4] = loadTexture(renderer,"images/off.png");
    return ret_val;
}

SDL_Texture** menu_background_loader(SDL_Renderer* renderer){
    SDL_Texture** ret_val =(SDL_Texture**) malloc(sizeof(SDL_Texture*)*3);
    ret_val[0]= loadTexture( renderer,"images/menu_background.jpg");
    ret_val[1]= loadTexture(renderer,"images/scoreboard_background.jpg");
    ret_val[2]= loadTexture(renderer,"images/custom_game background.jpg");
    return ret_val;
}


SDL_Texture** barracks_loader(SDL_Renderer* renderer){
    SDL_Texture** ret_val =(SDL_Texture**) malloc(sizeof(SDL_Texture*)*5);
    ret_val[0]= loadTexture(renderer,"images/barracks/neutral_b.png");
    ret_val[1]= loadTexture(renderer,"images/barracks/human_b.png");
    ret_val[2]= loadTexture(renderer,"images/barracks/orc_b.png");
    ret_val[3]= loadTexture(renderer,"images/barracks/elf_b.png");
    ret_val[4]= loadTexture(renderer,"images/barracks/undead_b.png");
    return ret_val;
}
SDL_Texture** shape_loader(SDL_Renderer* renderer){
    SDL_Texture** ret_val =(SDL_Texture**) malloc(sizeof(SDL_Texture*)*36);
    ret_val[0]= loadTexture(renderer,"images/shape/1_human.png");
    ret_val[1]= loadTexture(renderer,"images/shape/2_human.png");
    ret_val[2]= loadTexture(renderer,"images/shape/3_human.png");
    ret_val[3]= loadTexture(renderer,"images/shape/4_human.png");
    ret_val[4]= loadTexture(renderer,"images/shape/5_human.png");
    ret_val[5]= loadTexture(renderer,"images/shape/6_human.png");
    ret_val[6]= loadTexture(renderer,"images/shape/7_human.png");
    ret_val[7]= loadTexture(renderer,"images/shape/8_human.png");
    ret_val[8]= loadTexture(renderer,"images/shape/9_human.png");
    ret_val[9]= loadTexture(renderer,"images/shape/1_orc.png");
    ret_val[10]= loadTexture(renderer,"images/shape/2_orc.png");
    ret_val[11]= loadTexture(renderer,"images/shape/3_orc.png");
    ret_val[12]= loadTexture(renderer,"images/shape/4_orc.png");
    ret_val[13]= loadTexture(renderer,"images/shape/5_orc.png");
    ret_val[14]= loadTexture(renderer,"images/shape/6_orc.png");
    ret_val[15]= loadTexture(renderer,"images/shape/7_orc.png");
    ret_val[16]= loadTexture(renderer,"images/shape/8_orc.png");
    ret_val[17]= loadTexture(renderer,"images/shape/9_orc.png");
    ret_val[18]= loadTexture(renderer,"images/shape/1_elf.png");
    ret_val[19]= loadTexture(renderer,"images/shape/2_elf.png");
    ret_val[20]= loadTexture(renderer,"images/shape/3_elf.png");
    ret_val[21]= loadTexture(renderer,"images/shape/4_elf.png");
    ret_val[22]= loadTexture(renderer,"images/shape/5_elf.png");
    ret_val[23]= loadTexture(renderer,"images/shape/6_elf.png");
    ret_val[24]= loadTexture(renderer,"images/shape/7_elf.png");
    ret_val[25]= loadTexture(renderer,"images/shape/8_elf.png");
    ret_val[26]= loadTexture(renderer,"images/shape/9_elf.png");
    ret_val[27]= loadTexture(renderer,"images/shape/1_undead.png");
    ret_val[28]= loadTexture(renderer,"images/shape/2_undead.png");
    ret_val[29]= loadTexture(renderer,"images/shape/3_undead.png");
    ret_val[30]= loadTexture(renderer,"images/shape/4_undead.png");
    ret_val[31]= loadTexture(renderer,"images/shape/5_undead.png");
    ret_val[32]= loadTexture(renderer,"images/shape/6_undead.png");
    ret_val[33]= loadTexture(renderer,"images/shape/7_undead.png");
    ret_val[34]= loadTexture(renderer,"images/shape/8_undead.png");
    ret_val[35]= loadTexture(renderer,"images/shape/9_undead.png");
    return ret_val;
}
Mix_Chunk** load_sound_effect(){
    Mix_Chunk ** ret_val =(Mix_Chunk**) malloc(sizeof(Mix_Chunk*)*5);
    ret_val[0] = Mix_LoadWAV( "sounds/start_game.wav" );
    ret_val[1] = Mix_LoadWAV( "sounds/t_captured.wav" );
    ret_val[2] = Mix_LoadWAV( "sounds/win.wav" );
    ret_val[3] = Mix_LoadWAV( "sounds/loose.wav" );
    ret_val[4] = Mix_LoadWAV( "sounds/potion.wav" );
    return ret_val;
}
SDL_Texture** potion_loader(SDL_Renderer* renderer){
    SDL_Texture** ret_val =(SDL_Texture**) malloc(sizeof(SDL_Texture*)*4);
    ret_val[0]= loadTexture(renderer,"images/potion1.png");
    ret_val[1]= loadTexture(renderer,"images/potion2.png");
    ret_val[2]= loadTexture(renderer,"images/potion3.png");
    ret_val[3]= loadTexture(renderer,"images/potion4.png");
    return ret_val;
}
SDL_Texture** troops_loader(SDL_Renderer* renderer){
    SDL_Texture** t_t =(SDL_Texture**) malloc(sizeof(SDL_Texture*)*4);
    t_t[0]=loadTexture(renderer,"images/t_human.png");
    t_t[1]=loadTexture(renderer,"images/t_orc.png");
    t_t[2]=loadTexture(renderer,"images/t_elf.png");
    t_t[3]=loadTexture(renderer,"images/t_undead.png");
    return t_t;
}
SDL_Texture* is_save_available(SDL_Renderer* renderer ,char name[40],int *is_saved_game ){
    SDL_Texture* button;
    FILE* save_fptr = fopen("map_and_save/save.txt","r");
    char name2[40];
    memset(name2,0,sizeof(name2));
    fscanf(save_fptr,"%s\n",name2);
    if(strcmp(name2,name)==0){
        *is_saved_game=1;
    }
    fclose(save_fptr);
    if(*is_saved_game){
        button = loadTexture(renderer,"images/load.jpg");
    }else{
        button=loadTexture(renderer,"images/load_unavailable.jpg");
    }
    return button;
}

