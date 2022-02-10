#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
#include "TextureLoader.h"
#include "other_functions.h"
#include "file_functions.h"




int main(int argc, char* args[]) {

    // init sdl
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    SDL_Window* window = SDL_CreateWindow("State.io", 0, 0, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor( renderer, 250, 250, 250, 255 );
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    TTF_Init();
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

    //loading font and colors
    TTF_Font *game_font = TTF_OpenFont("images/game_font.ttf",20);
    SDL_Color yellow = {204,204,0};
    SDL_Color red = {255,0,0};



    // loading background and music and textures
    SDL_Texture** menu_background= menu_background_loader(renderer);


    // loading button used in menu
    SDL_Texture** button = button_loader(renderer);
    SDL_Texture* text_texture[5];
    text_texture[0]= textLoader("Please enter your name",yellow,game_font, renderer);

    //set rectangles for buttons and text
    SDL_Rect rect[6] ;
    main_menu_rect_init(rect);


    // loading sound effect and music for menu
    Mix_Music *background_music =Mix_LoadMUS( "sounds/music.wav" );
    Mix_Chunk *click_effecct = Mix_LoadWAV( "sounds/click.wav" );
    Mix_Chunk *next_page_effect = Mix_LoadWAV( "sounds/new page.wav" );
    Mix_PlayMusic( background_music, -1 );



    // Menu loop next_menu: 0 = main menu   1 = custom game menu   2 = load saved game   3 = scoreboard
    bool quit= false;
    bool is_name_enter = false;
    bool is_elf=false,is_orc=true,is_undead=false;
    char name[40];
    memset(name,0,40* sizeof(char ));
    char text[100];
    int i=0;
    int is_saved_game = 0;
    int map_checked=1;
    int next_menu=0;
    int button_pushed =0;
    int shift =0;
    int t_for_random_map=0;
    SDL_Event events;
    while(!quit){//this loop handle game menu
        while (SDL_PollEvent(&events)!=0){
            if(events.type == SDL_QUIT){// quit game
                quit = true;
            }

            //enter name
            else if(!is_name_enter && events.type == SDL_KEYDOWN){
                is_name_enter = name_input(&events, name, &i, &shift);
                if (is_name_enter) {
                    Mix_PlayChannel(-1, next_page_effect, 0);
                    SDL_DestroyTexture(text_texture[0]);
                    sprintf(text, "Welcome,%s.", name);
                    text_texture[0] = textLoader(text, yellow, game_font, renderer);
                    //load saved file
                    button[1] = is_save_available(renderer, name, &is_saved_game);
                }
            }

            //handle buttons in main menu
            else if(is_name_enter&& next_menu==0 && events.type == SDL_MOUSEBUTTONDOWN){
                Mix_PlayChannel( -1, click_effecct, 0 );
                int mouse_x,mouse_y;
                SDL_GetMouseState( &mouse_x, &mouse_y );
                next_menu = menu_button(mouse_x,mouse_y,0);
                if(next_menu == 1 || next_menu ==3){
                    Mix_PlayChannel( -1, next_page_effect, 0 );
                }
                // loading file for next menu
                if(next_menu == 3){
                    scores(text_texture+1,renderer,game_font,&red);
                }else if(next_menu == 2){
                    if(is_saved_game){//load game
                        Mix_PauseMusic();
                        game_start(renderer,0,0,0,0,game_font,1,name,0);
                        Mix_ResumeMusic();
                        next_menu=0;
                    }
                    else{
                        next_menu=0;
                    }
                }

            }
            // scoreboard menu buttons
            else if(is_name_enter&& next_menu==3 && events.type == SDL_MOUSEBUTTONDOWN){
                Mix_PlayChannel( -1, click_effecct, 0 );
                int mouse_x,mouse_y;
                SDL_GetMouseState( &mouse_x, &mouse_y );
                next_menu = menu_button(mouse_x,mouse_y,3);
                if(next_menu !=4){
                    next_menu =3;
                }
                if(next_menu == 4){
                    Mix_PlayChannel( -1, next_page_effect, 0 );
                    next_menu = 0;
                    SDL_DestroyTexture(text_texture[1]);
                    SDL_DestroyTexture(text_texture[2]);
                    SDL_DestroyTexture(text_texture[3]);
                    SDL_DestroyTexture(text_texture[4]);
                }
            }

            // custom game menu
            else if(is_name_enter&& next_menu==1 && events.type == SDL_MOUSEBUTTONDOWN){
                Mix_PlayChannel( -1, click_effecct, 0 );
                int mouse_x,mouse_y;
                SDL_GetMouseState( &mouse_x, &mouse_y );
                button_pushed= custom_game_button(mouse_x,mouse_y);
                switch (button_pushed) {
                    case 0:
                        next_menu =0;
                        break;
                    case 1:
                        if((map_checked==0 && is_elf+is_undead+is_orc>0) || is_elf+is_undead+is_orc+1 == player_in_map(map_checked)){
                            Mix_PauseMusic();
                            game_start(renderer,map_checked,is_elf,is_orc,is_undead,game_font,0,name,t_for_random_map);
                            Mix_ResumeMusic();

                        }
                        break;
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        map_checked = button_pushed - 5;
                        break;
                    case 10:
                        is_orc = !is_orc;
                        break;
                    case 11:
                        is_elf = !is_elf;
                        break;
                    case 12:
                        is_undead = !is_undead;
                        break;
                }
            }
            else if(is_name_enter&& next_menu==1 && events.type == SDL_KEYDOWN){
                if(events.key.keysym.sym == SDLK_DOWN){
                    t_for_random_map--;
                }else if(events.key.keysym.sym == SDLK_UP){
                    t_for_random_map++;
                }
            }
        }

        SDL_RenderClear( renderer );

        if(!is_name_enter){// enter name menu
            SDL_RenderCopy(renderer,menu_background[0],NULL,NULL);
            SDL_RenderCopy(renderer,text_texture[0],NULL,&rect[0]);
        }else if(next_menu ==0){// main menu
            SDL_RenderCopy(renderer,menu_background[0],NULL,NULL);
            SDL_RenderCopy(renderer,text_texture[0],NULL,&rect[0]);
            SDL_RenderCopy(renderer,button[0],NULL,&rect[1]);
            SDL_RenderCopy(renderer,button[1],NULL,&rect[2]);
            SDL_RenderCopy(renderer,button[2],NULL,&rect[3]);
        }else if(next_menu == 1){
            SDL_RenderCopy(renderer,menu_background[2],NULL,NULL);
            if(!is_orc){
                rect[5].h =720;
                rect[5].w =1280;
                rect[5].y = 28;
                rect[5].x =0;
                SDL_RenderCopy(renderer,button[4],NULL,&rect[5]);
            }if(!is_elf){
                rect[5].h =720;
                rect[5].w =1280;
                rect[5].y = 60;
                rect[5].x =0;
                SDL_RenderCopy(renderer,button[4],NULL,&rect[5]);
            }if(!is_undead){
                rect[5].h =720;
                rect[5].w =1280;
                rect[5].y = 90;
                rect[5].x =0;
                SDL_RenderCopy(renderer,button[4],NULL,&rect[5]);
            }
            rect[5].h =50;
            rect[5].w =300;
            rect[5].y =40;
            rect[5].x =850;
            if(is_elf+is_undead+is_orc>0){
                if(map_checked==0){
                    // print each map details
                    text_texture[1]= textLoader("Total territory number",yellow,game_font,renderer);
                    SDL_RenderCopy(renderer,text_texture[1],NULL,&rect[5]);
                    SDL_DestroyTexture(text_texture[1]);
                    rect[5].y =100;
                    text_texture[1]= textLoader("must be between",yellow,game_font,renderer);
                    SDL_RenderCopy(renderer,text_texture[1],NULL,&rect[5]);
                    SDL_DestroyTexture(text_texture[1]);
                    sprintf(text,"%d and %d",(is_orc+is_undead+is_elf+1)*3,16);
                    rect[5].y =160;
                    text_texture[1]= textLoader(text,yellow,game_font,renderer);
                    SDL_RenderCopy(renderer,text_texture[1],NULL,&rect[5]);
                    SDL_DestroyTexture(text_texture[1]);
                    if(t_for_random_map<(is_orc+is_undead+is_elf+1)*3){
                        t_for_random_map = (is_orc+is_undead+is_elf+1)*3;
                    }
                    if(t_for_random_map>16){
                        t_for_random_map=16;
                    }
                    rect[5].y =220;
                    sprintf(text,"territory: %d",t_for_random_map);
                    text_texture[1]= textLoader(text,yellow,game_font,renderer);
                    SDL_RenderCopy(renderer,text_texture[1],NULL,&rect[5]);
                    SDL_DestroyTexture(text_texture[1]);
                }else{
                    if(map_checked !=4){
                        sprintf(text,"map %d with %d player",map_checked, player_in_map(map_checked));
                    }else{
                        sprintf(text,"saved map with %d player",player_in_map(map_checked));
                    }
                    text_texture[1]= textLoader(text,yellow,game_font,renderer);
                    SDL_RenderCopy(renderer,text_texture[1],NULL,&rect[5]);
                    SDL_DestroyTexture(text_texture[1]);

                }
            }else{
                text_texture[1]= textLoader("add at least 1 enemy",yellow,game_font,renderer);
                SDL_RenderCopy(renderer,text_texture[1],NULL,&rect[5]);
                SDL_DestroyTexture(text_texture[1]);
            }

        }else if(next_menu == 3){// scoreboard page
            SDL_RenderCopy(renderer,menu_background[1],NULL,NULL);
            SDL_RenderCopy(renderer,text_texture[1],NULL,&rect[0]);
            SDL_RenderCopy(renderer,text_texture[2],NULL,&rect[1]);
            SDL_RenderCopy(renderer,text_texture[3],NULL,&rect[2]);
            SDL_RenderCopy(renderer,text_texture[4],NULL,&rect[3]);
            SDL_RenderCopy(renderer,button[3],NULL,&rect[4]);
        }

        SDL_RenderPresent( renderer );
        SDL_Delay(1000/60);
    }

    // free memory
    for(int d=0;d<5;d++){
        SDL_DestroyTexture(button[d]);
    }
    free(button);
    for(int d=0;d<3;d++){
        SDL_DestroyTexture(menu_background[d]);
    }
    free(menu_background);
    SDL_DestroyTexture(text_texture[0]);
    TTF_CloseFont( game_font );
    game_font = NULL;

    Mix_FreeChunk(click_effecct);
    Mix_FreeChunk(next_page_effect);
    Mix_FreeMusic( background_music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
