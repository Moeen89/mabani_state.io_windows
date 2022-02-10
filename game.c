//
// Created by Moein111 on 01.02.22.
//

#include "game.h"
int game_start(SDL_Renderer* renderer,int map_checked,int is_elf,int is_orc,int is_undead,TTF_Font* game_font ,int is_loaded,char name[40],int t_for_random){

    // load textures
    SDL_Texture* background = loadTexture(renderer,"images/shape/map.png");
    SDL_Texture** barracks = barracks_loader(renderer);// 0 :
    SDL_Texture** shapes = shape_loader(renderer);
    SDL_Texture** potions = potion_loader(renderer);
    SDL_Texture ** troops = troops_loader(renderer);
    Mix_Chunk** sound_effect = load_sound_effect();
    Mix_PlayChannel( -1,sound_effect[0], 0 );
    struct map* loaded_map = NULL;
    struct map loaded_map_rnd;
    loaded_map = &loaded_map_rnd;

    if(!is_loaded){
        //generate random map
        if(map_checked==0) {
            loaded_map_rnd = random_map(is_orc + is_elf + is_undead + 1,t_for_random);
        //load saved map
        }else{
            loaded_map_rnd = loading_map(map_checked);
        }
        //change enemy in the map
        set_up_game(is_elf,is_orc,is_undead,loaded_map);
    }

    // list of troops out of their barracks
    struct troops_struct moving_troops[1250];
    memset(moving_troops,0,sizeof(moving_troops));
    // list of potions place and types
    struct potion_struct potion_list[3];
    memset(potion_list,0,sizeof(potion_list));
    //variable for limiting potions on the map
    int total_potion=0;

    //load saved game
    if(is_loaded){
        load_saved_game(loaded_map,moving_troops,potion_list,&total_potion);
    }



    SDL_Event events;
    loaded_map->production_rate[0]=0;
    int frame_passed =0;                //to call some functions with more time difference
    int from_to[2]={-1,-1};     //to save source and destination of player moves
    int quit=1;
    while (quit){
        while (SDL_PollEvent(&events)!=0) {

            if (events.type == SDL_QUIT) {
                quit = 0;
            }else if(events.type == SDL_MOUSEBUTTONDOWN){// select barracks and set destination
                int mouse_x,mouse_y;
                SDL_GetMouseState( &mouse_x, &mouse_y );
                int select = select_barracks(loaded_map,mouse_x,mouse_y);
                if(from_to[0] == -1 && select !=-1 && loaded_map->first_tr_ptr[select].owner == 1){
                    from_to[0]=select;
                } else if(select == -1){
                    from_to[0]=-1;
                    from_to[1]=-1;
                }else if(from_to[0] !=-1 && from_to[1]==-1){
                    from_to[1]=select;
                    if(from_to[0]== from_to[1]){
                        from_to[0]=-1;
                        from_to[1]=-1;
                    }else{
                        set_troops(&loaded_map->first_tr_ptr[from_to[0]],moving_troops,from_to[0],from_to[1]);
                        from_to[0]=-1;
                        from_to[1]=-1;
                    }
                }
            }else if(events.type == SDL_KEYDOWN){ // pause game and starting in game menu, user can save game,save map quit or resume
                if(events.key.keysym.sym == SDLK_ESCAPE){
                    in_game_menu(renderer,loaded_map,&quit,moving_troops,potion_list,total_potion,name);
                }

            }
        }


        //speed and game is based on frame rate
        frame_passed %=5000;
        frame_passed++;


        //producing troops in barracks
        if(frame_passed%50==49){
            for(int i=0;i<loaded_map->total_territory;i++) {
                troop_production(&(loaded_map->first_tr_ptr[i]),loaded_map);
            }
        }

        //AI of enemy
       if(frame_passed%30 == 3){
           AI(loaded_map,moving_troops);
        }

       //put random potion in map
       if(total_potion<3 && rand()%200==0){
          random_potion(potion_list,loaded_map,&total_potion);
       }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,background,NULL,NULL);
        // rendering barracks, territories, troops and potions
        for(int i=0;i<loaded_map->total_territory;i++) {
            render_territory(renderer, &(loaded_map->first_tr_ptr[i]), shapes, barracks,game_font,loaded_map,potions);
        }
        render_potion(potion_list,moving_troops,loaded_map,renderer,&total_potion,sound_effect,potions,barracks,troops);
        render_troops(moving_troops,loaded_map,renderer,frame_passed,sound_effect,troops);
        SDL_RenderPresent(renderer);

        //check for game over
        if (frame_passed%100 ==0){
            quit = is_over(loaded_map,moving_troops,renderer,game_font,is_elf,is_orc,is_undead,sound_effect);
        }

        SDL_Delay(15);
    }

    update_score(name, loaded_map->scores[0]);
    //destroy textures
    SDL_DestroyTexture(background);
    for(int i=0;i<5;i++){
        SDL_DestroyTexture(barracks[i]);
    }for(int i=0;i<4;i++){
        SDL_DestroyTexture(potions[i]);
    }for(int i=0;i<4;i++){
        SDL_DestroyTexture(troops[i]);
    }for(int i=0;i<5;i++){
        Mix_FreeChunk(sound_effect[i]);
    }
    for(int i=0;i<36;i++){
        SDL_DestroyTexture(shapes[i]);
    }
    free(sound_effect);
    free(barracks);
    free(potions);
    free(troops);
    free(shapes);
    free(loaded_map->first_tr_ptr);
}
