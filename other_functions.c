
#include "other_functions.h"
struct scoreboard_data{
    int point;
    int type;
};
int compare(const struct scoreboard_data *p, const struct scoreboard_data *q){
    if(q->point> p->point){
        return 1;
    }
    return 0;
}

int menu_button(int x, int y,int current){
    if(x <1100 && x>800){
        if(y<130 && y>80){
            return 1;
        }if(y<200 && y>150){
            return 2;
        }if(y<270 && y>220){
            return 3;
        }
        if(y<650 && y>600){
            return 4;
        }
    }
    return current;
}
int custom_game_button(int x,int y){
    if (y<680 && y>630 && x>965 && x<1240 ){
        return 0;// cancel
    } else if(y<620 && y>570 && x>965 && x<1240){
        return 1; // start
    } else if(y<490 && y>450 && x<1210 && x>905){
        return 5;// random map
    } else if( x<350 && x>70 && y>290 && y<320) {
        return 11;//night elf
    }else if( x<350 && x>70 && y>262 && y<288) {
        return 10;//orc
    }else if( x<350 && x>70 && y>322 && y<352) {
        return 12;//undead
    } else if(x<720 && x>290 && y>40 && y<65){
        return 6;// map1
    }else if(x<720 && x>290 && y>65 && y<90){
        return 7;//map2
    }else if(x<720 && x>290 && y>90&& y<110){
        return 8;//map3
    }else if(x<720 && x>290 && y>110&& y<140){
        return 9;//saved map
    }
    return -1;
}



void render_territory(SDL_Renderer* renderer,struct territory_struct* inp,SDL_Texture** shape_t,SDL_Texture** barracks_t,TTF_Font* game_font,struct map* game,SDL_Texture** potions){
    SDL_Rect rect;
    rect.x = inp->x_center-55;
    rect.y = inp->y_center-55;
    rect.w = 110;
    rect.h = 110;
    if(inp->owner!=0) {
        SDL_RenderCopyEx(renderer, shape_t[(inp->owner-1) * 9 + inp->type], NULL, &rect, inp->rotation, NULL,SDL_FLIP_NONE);
    }
    rect.x += 15;
    rect.y += 15;
    rect.w = 80;
    rect.h = 80;
    SDL_RenderCopy(renderer, barracks_t[inp->owner], NULL, &rect);
    char troops[5];
    sprintf(troops,"%d",inp->troops+inp->waiting_troops);
    SDL_Color green={0,255,100};
    SDL_Texture* text = textLoader(troops,green,game_font,renderer);
    rect.x +=20;
    rect.y +=25;
    rect.w = 40;
    rect.h = 30;
    SDL_RenderCopy(renderer,text,NULL,&rect);
    SDL_DestroyTexture(text);
    if(game->active_poition_time[0]>0){
        rect.x =0;
        rect.y =0;
        rect.w = 60;
        rect.h = 80;
        SDL_RenderCopy(renderer,potions[game->active_poition_type[0]-1],NULL,&rect);
        sprintf(troops,"%d",game->active_poition_time[0]/50);
        text = textLoader(troops,green,game_font,renderer);
        rect.x =10;
        rect.y =30;
        rect.w = 40;
        rect.h = 30;
        SDL_RenderCopy(renderer,text,NULL,&rect);
        SDL_DestroyTexture(text);
    }


}

void troop_production(struct territory_struct* inp,struct map* game){
    int max=25;
    if(inp->owner==0){
         max =12;
    }
    if(inp->troops<max || game->active_poition_type[inp->owner-1]==4){
        inp->troops += game->production_rate[inp->owner];
        if(inp->troops>max && game->active_poition_type[inp->owner-1]!=4){
            inp->troops=max;
        }
    }

}

int select_barracks(struct map* game_map,int x,int y){
    for(int i=0;i< game_map->total_territory;i++){
        if(abs(x-game_map->first_tr_ptr[i].x_center)<55 && abs(y-game_map->first_tr_ptr[i].y_center)<50){
            return i;
        }
    }
    return -1;
}
void set_troops(struct territory_struct* inp,struct troops_struct* troops,int from,int to){
    int x=inp->troops;
    inp->waiting_troops +=x;
    inp->troops=0;
    int i=0;
    while (x>0){
        if(troops[i].to_t == 0){
            troops[i].from_t = from+1;
            troops[i].to_t = to+1;
            troops[i].owner = inp->owner;
            x--;
        }
        i++;
    }
}
void render_troops(struct troops_struct* troops,struct map* game,SDL_Renderer* renderer,int frame,Mix_Chunk** sound_effect,SDL_Texture** t_t){


    int ter[20][20];
    memset(ter,0,sizeof(ter));
    for (int i = 0; i < 1250; i++) {
        if (frame%20 == 0&& troops[i].from_t !=0 && troops[i].to_t !=0 && ter[troops[i].from_t-1][troops[i].to_t-1]==0 && game->troops_speed[troops[i].owner-1]!=0){
            ter[troops[i].from_t-1][troops[i].to_t-1]++;
            troops[i].x=game->first_tr_ptr[troops[i].from_t-1].x_center-10;
            troops[i].y=game->first_tr_ptr[troops[i].from_t-1].y_center-10;
            game->first_tr_ptr[troops[i].from_t-1].waiting_troops--;
            troops[i].from_t = 0;
        } else if (frame%10 == 0 && troops[i].from_t !=0 && troops[i].to_t !=0  && game->first_tr_ptr[troops[i].from_t-1].waiting_troops>5 && ter[troops[i].from_t-1][troops[i].to_t -1 ]==1){
            ter[troops[i].from_t-1][troops[i].to_t-1]++;
            troops[i].x=game->first_tr_ptr[troops[i].from_t-1].x_center+10;
            troops[i].y=game->first_tr_ptr[troops[i].from_t-1].y_center+10;
            game->first_tr_ptr[troops[i].from_t-1].waiting_troops--;
            troops[i].from_t = 0;
        }else if(troops[i].from_t ==0 && troops[i].to_t!=0){
            double v =sqrt(pow(troops[i].x- game->first_tr_ptr[troops[i].to_t-1].x_center,2)+ pow(troops[i].y- game->first_tr_ptr[troops[i].to_t-1].y_center,2));
            troops[i].x+=(game->troops_speed[troops[i].owner-1]/v) * (game->first_tr_ptr[troops[i].to_t-1].x_center- troops[i].x) ;
            troops[i].y+=(game->troops_speed[troops[i].owner-1]/v) * (game->first_tr_ptr[troops[i].to_t-1].y_center- troops[i].y) ;
            SDL_Rect r;
            r.x=(int)troops[i].x -20;
            r.y=(int)troops[i].y-20;
            r.w=40;
            r.h = 40;
            SDL_RenderCopy(renderer,t_t[troops[i].owner-1],NULL,&r);
            if(abs(troops[i].x - game->first_tr_ptr[troops[i].to_t-1].x_center)<15 && abs(troops[i].y - game->first_tr_ptr[troops[i].to_t-1].y_center)<15){
                troops[i].x =0;
                troops[i].y =0;
                if(troops[i].owner ==game->first_tr_ptr[troops[i].to_t-1].owner){
                    game->first_tr_ptr[troops[i].to_t-1].troops++;
                } else{
                    if(game->first_tr_ptr[troops[i].to_t-1].waiting_troops>0){
                        game->first_tr_ptr[troops[i].to_t-1].waiting_troops--;
                        for(int ff=0;ff<1250;ff++){
                            if(troops[ff].from_t-1 ==troops[i].to_t-1){
                                troops[ff].from_t  =0;
                                troops[ff].to_t  =0;
                                troops[ff].owner  =0;
                                break;
                            }
                        }
                    }else{
                        game->first_tr_ptr[troops[i].to_t-1].troops--;
                    }
                    if( game->first_tr_ptr[troops[i].to_t-1].troops<0){
                        game->first_tr_ptr[troops[i].to_t-1].owner = troops[i].owner;
                        game->first_tr_ptr[troops[i].to_t-1].troops = 1;
                        game->scores[troops[i].owner-1] += 3;
                        Mix_PlayChannel( -1, sound_effect[1], 0 );
                    }
                }
                troops[i].to_t =0;
                troops[i].owner =0;
            }
            for(int q=0;q<1250;q++){
                if(troops[i].to_t !=0 && troops[q].owner != troops[i].owner && abs(troops[i].x - troops[q].x)<15 && abs(troops[i].y - troops[q].y)<15){
                    troops[i].x =0;
                    troops[i].y =0;
                    troops[i].owner =0;
                    troops[i].to_t =0;
                    troops[q].x =0;
                    troops[q].y =0;
                    troops[q].owner =0;
                    troops[q].to_t =0;
                    break;
                }
            }
        }
    }
}

void AI(struct map* game,struct troops_struct* troops){
    int total_barracks_troops[4];
    memset(total_barracks_troops,0,sizeof (total_barracks_troops));
    for(int i=0;i<game->total_territory;i++){
        if(game->first_tr_ptr[i].owner !=0){
            total_barracks_troops[game->first_tr_ptr[i].owner-1] += game->first_tr_ptr[i].troops + game->first_tr_ptr[i].waiting_troops;
        }

    }
    for(int i=0;i<game->total_territory;i++){
        int point=0;
        int max_point=-1000;
        int i_max = 0;
        if(game->first_tr_ptr[i].owner >= 2){
            for(int j=0;j<game->total_territory;j++){
                if(game->first_tr_ptr[i].owner ==4){
                    point = (i != j && game->first_tr_ptr[i].troops < game->first_tr_ptr[j].troops + 3)* -100
                            + (game->first_tr_ptr[i].troops - game->first_tr_ptr[j].troops - game->first_tr_ptr[j].waiting_troops/4)
                            + (game->first_tr_ptr[i].troops > 5&& game->first_tr_ptr[j].owner==0 && sqrt(pow(game->first_tr_ptr[i].x_center -game->first_tr_ptr[j].x_center,2)+pow(game->first_tr_ptr[i].y_center -game->first_tr_ptr[j].y_center,2)) < 200)*5 ;
                }else if(game->first_tr_ptr[i].owner ==2){
                    point =(game->first_tr_ptr[i].owner != game->first_tr_ptr[j].owner )* (game->first_tr_ptr[i].troops - game->first_tr_ptr[j].troops - game->first_tr_ptr[j].waiting_troops/3 )*((game->first_tr_ptr[i].troops - sqrt(pow(game->first_tr_ptr[i].x_center -game->first_tr_ptr[j].x_center,2)+pow(game->first_tr_ptr[i].y_center -game->first_tr_ptr[j].y_center,2))/100 > game->first_tr_ptr[j].troops)+1) + (game->first_tr_ptr[i].troops < 20)*(i==j)*5  + (game->first_tr_ptr[i].owner == game->first_tr_ptr[j].owner)*(game->first_tr_ptr[i].troops - game->first_tr_ptr[j].troops)/2 + (game->first_tr_ptr[i].troops>=25||game->first_tr_ptr[i].troops+game->first_tr_ptr[i].waiting_troops>30)*(game->first_tr_ptr[i].owner != game->first_tr_ptr[j].owner )*100;
                }else if(game->first_tr_ptr[i].owner ==3){
                    point = game->first_tr_ptr[i].troops - game->first_tr_ptr[j].troops - (game->first_tr_ptr[j].waiting_troops) * sqrt(pow(game->first_tr_ptr[i].x_center -game->first_tr_ptr[j].x_center,2)+pow(game->first_tr_ptr[i].y_center -game->first_tr_ptr[j].y_center,2))/100 + 2*(game->first_tr_ptr[j].owner <2)+(game->first_tr_ptr[j].owner != game->first_tr_ptr[i].owner) + (game->first_tr_ptr[i].troops>=20 ||game->first_tr_ptr[i].troops+game->first_tr_ptr[i].waiting_troops>30 )*(game->first_tr_ptr[i].owner != game->first_tr_ptr[j].owner)*10;
                    point += 3*(game->first_tr_ptr[j].owner == 0);
                }
                point += (game->first_tr_ptr[i].owner != game->first_tr_ptr[j].owner) * 1000 * (total_barracks_troops[game->first_tr_ptr[i].owner-1]>100);
                if(point>max_point){
                    max_point = point;
                    i_max=j;
                }
            }
            if(i!=i_max){
                set_troops(&game->first_tr_ptr[i],troops,i,i_max);
            }


        }
    }
}

void random_potion(struct potion_struct* potions,struct map* game,int *total){
    for(int i=0;i<3;i++){
        if(potions[i].type==0){
            int a= rand()%game->total_territory;
            int b= rand()%game->total_territory;
            if(a !=b){
                int a_weight=(rand()%5)+2;
                int b_weight=(rand()%5)+2;
                potions[i].x= (a_weight * game->first_tr_ptr[a].x_center + b_weight * game->first_tr_ptr[b].x_center)/(a_weight+b_weight);
                potions[i].y= (a_weight * game->first_tr_ptr[a].y_center + b_weight * game->first_tr_ptr[b].y_center)/(a_weight+b_weight);
                potions[i].type= (rand()%4)+1;
                *total +=1;
            }
            break;
        }
    }
}
void swap_troops_texture(SDL_Texture** troops_texture,SDL_Texture** barracks_texture,int owner,int to,SDL_Renderer* renderer){
    if(to<3) {
        SDL_DestroyTexture(troops_texture[owner - 1]);
        if (owner == 1) {
            if (to == 0) {
                troops_texture[0] = loadTexture(renderer, "images/t_human.png");
            } else if (to == 1) {
                troops_texture[0] = loadTexture(renderer, "images/tp1_human.png");
            } else if (to == 2) {
                troops_texture[0] = loadTexture(renderer, "images/tp2_human.png");
            }

        } else if (owner == 2) {
            if (to == 0) {
                troops_texture[1] = loadTexture(renderer, "images/t_orc.png");
            } else if (to == 1) {
                troops_texture[1] = loadTexture(renderer, "images/tp1_orc.png");
            } else if (to == 2) {
                troops_texture[1] = loadTexture(renderer, "images/tp2_orc.png");
            }
        } else if (owner == 3) {
            if (to == 0) {
                troops_texture[2] = loadTexture(renderer, "images/t_elf.png");
            } else if (to == 1) {
                troops_texture[2] = loadTexture(renderer, "images/tp1_elf.png");
            } else if (to == 2) {
                troops_texture[2] = loadTexture(renderer, "images/tp2_elf.png");
            }
        } else if (owner == 4) {
            if (to == 0) {
                troops_texture[3] = loadTexture(renderer, "images/t_undead.png");
            } else if (to == 1) {
                troops_texture[3] = loadTexture(renderer, "images/tp1_undead.png");
            } else if (to == 2) {
                troops_texture[3] = loadTexture(renderer, "images/tp2_undead.png");
            }
        }
    }else{
        SDL_DestroyTexture(barracks_texture[owner]);
        if (owner == 1) {
            if (to == 5) {
                barracks_texture[1] = loadTexture(renderer, "images/barracks/human_b.png");
            } else if (to == 3) {
                barracks_texture[1] = loadTexture(renderer, "images/barracks/human_bp3.png");
            } else if (to == 4) {
                barracks_texture[1] = loadTexture(renderer, "images/barracks/human_bp4.png");
            }

        } else if (owner == 2) {
            if (to == 5) {
                barracks_texture[2] = loadTexture(renderer, "images/barracks/orc_b.png");
            } else if (to == 3) {
                barracks_texture[2] = loadTexture(renderer, "images/barracks/orc_bp3.png");
            } else if (to == 4) {
                barracks_texture[2] = loadTexture(renderer, "images/barracks/orc_bp4.png");
            }
        } else if (owner == 3) {
            if (to == 5) {
                barracks_texture[3] = loadTexture(renderer, "images/barracks/elf_b.png");
            } else if (to == 3) {
                barracks_texture[3] = loadTexture(renderer, "images/barracks/elf_bp3.png");
            } else if (to == 4) {
                barracks_texture[3] = loadTexture(renderer, "images/barracks/elf_bp4.png");
            }
        } else if (owner == 4) {
            if (to == 5) {
                barracks_texture[4] = loadTexture(renderer, "images/barracks/undead_b.png");
            } else if (to == 3) {
                barracks_texture[4] = loadTexture(renderer, "images/barracks/undead_bp3.png");
            } else if (to == 4) {
                barracks_texture[4] = loadTexture(renderer, "images/barracks/undead_bp4.png");
            }
        }
    }

}

void render_potion(struct potion_struct* potions,struct troops_struct* troops,struct map* game,SDL_Renderer* renderer,int* total,Mix_Chunk** sound_effect,SDL_Texture** t,SDL_Texture** barracks_texture,SDL_Texture** troops_texture) {
    for (int i = 0; i < 3; i++) {
        if (potions[i].type != 0) {
            SDL_Rect r;
            r.x = potions[i].x - 30;
            r.y = potions[i].y - 20;
            r.w = 40;
            r.h = 60;
            SDL_RenderCopy(renderer, t[potions[i].type - 1], NULL, &r);
            for (int j = 0; j < 1250; j++) {
                if (troops[j].owner !=0 && game->active_poition_time[troops[j].owner - 1] == 0 && abs(potions[i].x - troops[j].x) < 40 &&
                    abs(potions[i].y - troops[j].y) < 40) {
                    game->active_poition_type[troops[j].owner - 1] = potions[i].type;
                    if (potions[i].type == 1) {
                        game->troops_speed[troops[j].owner - 1] = 4;
                        game->active_poition_time[troops[j].owner - 1] = 300;
                        swap_troops_texture(troops_texture,barracks_texture,troops[j].owner ,1,renderer);

                    } else if (potions[i].type == 2) {
                        game->troops_speed[0] = 0;
                        game->troops_speed[1] = 0;
                        game->troops_speed[2] = 0;
                        game->troops_speed[3] = 0;
                        game->troops_speed[troops[j].owner - 1] = 2;
                        game->active_poition_time[troops[j].owner - 1] = 200;
                        swap_troops_texture(troops_texture,barracks_texture,1,2,renderer);
                        swap_troops_texture(troops_texture,barracks_texture,2 ,2,renderer);
                        swap_troops_texture(troops_texture,barracks_texture,3 ,2,renderer);
                        swap_troops_texture(troops_texture,barracks_texture,4 ,2,renderer);
                        swap_troops_texture(troops_texture,barracks_texture,troops[j].owner ,0,renderer);
                    } else if (potions[i].type == 3) {
                        game->production_rate[troops[j].owner] = 2;
                        game->active_poition_time[troops[j].owner - 1] = 200;
                        swap_troops_texture(troops_texture,barracks_texture,troops[j].owner ,3,renderer);
                    }else if (potions[i].type == 4) {
                        game->active_poition_time[troops[j].owner - 1] = 400;
                        swap_troops_texture(troops_texture,barracks_texture,troops[j].owner ,4,renderer);
                    }
                    game->scores[troops[j].owner-1] += 1;
                    Mix_PlayChannel( -1, sound_effect[4], 0 );
                    potions[i].type = 0;
                    potions[i].x = 0;
                    potions[i].y = 0;
                }
            }
        }
    }for(int i=0;i<4;i++){
        if(game->active_poition_time[i]>0){
            game->active_poition_time[i]--;
            if(game->active_poition_time[i]==0){
                if(game->active_poition_type[i]==1 && game->troops_speed[i]==4){
                    game->troops_speed[i]=2;
                    swap_troops_texture(troops_texture,barracks_texture,i+1 ,0,renderer);
                }if(game->active_poition_type[i]==2 ){
                    for(int k=0;k<4;k++){
                        if(game->troops_speed[k]==0){
                            game->troops_speed[k]=2;
                            swap_troops_texture(troops_texture,barracks_texture,k+1 ,0,renderer);
                        }
                    }
                }if(game->active_poition_type[i]==3 ){
                    game->production_rate[i+1]=1;
                    swap_troops_texture(troops_texture,barracks_texture,i+1 ,5,renderer);
                }if(game->active_poition_type[i]==4 ){
                    swap_troops_texture(troops_texture,barracks_texture,i+1 ,5,renderer);
                }
                *total-=1;
                game->active_poition_type[i]=0;
            }
        }
    }
}
void set_up_game(int is_elf,int is_orc,int is_undead,struct map* game){
    if(!is_orc){
        if(!is_elf){
            for(int i=0;i<game->total_territory;i++){
                if(game->first_tr_ptr[i].owner==2){
                    game->first_tr_ptr[i].owner=4;
                }
            }
        }else if(!is_undead){
            for(int i=0;i<game->total_territory;i++){
                if(game->first_tr_ptr[i].owner==2){
                    game->first_tr_ptr[i].owner=3;
                }
            }
        }else {
            for(int i=0;i<game->total_territory;i++){
                if(game->first_tr_ptr[i].owner==2){
                    game->first_tr_ptr[i].owner=4;
                }
            }
        }
    }else{
        if(is_undead && !is_elf){
            for(int i=0;i<game->total_territory;i++){
                if(game->first_tr_ptr[i].owner==3){
                    game->first_tr_ptr[i].owner=4;
                }
            }
        }
    }
}
int is_over(struct map* game,struct troops_struct* troops_list,SDL_Renderer* renderer,TTF_Font* font,int is_elf,int is_orc,int is_undead,Mix_Chunk** sound_effect){
    int t[5];
    int ret=0;
    memset(t,0, sizeof(t));
    int troops;
    for(int i=0;i<game->total_territory;i++){
        t[game->first_tr_ptr[i].owner]++;
    }
    if(t[1]==0){
        troops=0;
        for(int k=0;k<1250;k++){
            if(troops_list[k].owner ==1){
                troops=1;
                break;
            }
        }
        if(troops ==0){
            int i=3;
            if(t[2]>=t[3]&&t[2]>=t[4]){
                i=1;
            }else if(t[3]>=t[4]&&t[3]>=t[2]){
                i=2;
            }
            game->scores[i] += (is_elf+is_orc+is_undead)*70;
            game->scores[0] -= (4 - is_elf-is_orc-is_undead)*140;
            ret= 1;
        }
    }else if( t[2]==0 && t[3]==0 && t[4]==0 ){
        troops=0;
        for(int k=0;k<1250;k++){
            if(troops_list[k].owner ==2 ||troops_list[k].owner==3 || troops_list[k].owner==4 ){
                troops=1;
                break;
            }
        }
        if(troops ==0){
            game->scores[0] += (is_elf+is_orc+is_undead)*100;
            ret =2;
        }
    }
    if(ret !=0){
        SDL_RenderClear(renderer);
        SDL_Color yellow = {204,204,0};
        SDL_Rect rect[5] ;
        rect[0].x = 490;
        rect[0].y = 10;
        rect[0].h= 40;
        rect[0].w = 300;
        rect[1].x = 800;
        rect[1].y = 80;
        rect[1].h= 50;
        rect[1].w = 300;
        rect[2].x = 800;
        rect[2].y = 150;
        rect[2].h= 50;
        rect[2].w = 300;
        rect[3].x = 800;
        rect[3].y = 220;
        rect[3].h= 50;
        rect[3].w = 300;
        rect[4].x = 800;
        rect[4].y = 290;
        rect[4].h= 50;
        rect[4].w = 300;
        SDL_Texture *b= loadTexture(renderer,"images/scoreboard_background.jpg");
        SDL_RenderCopy(renderer,b,NULL,NULL);
        SDL_DestroyTexture(b);
        if(ret==2){
            b= textLoader("Victory",yellow,font,renderer);
            Mix_PlayChannel( -1,sound_effect[2], 0 );
            SDL_RenderCopy(renderer,b,NULL,&rect[0]);
            SDL_DestroyTexture(b);
        }else{
            b = textLoader("Defeat",yellow,font,renderer);
            Mix_PlayChannel( -1,sound_effect[3], 0 );
            SDL_RenderCopy(renderer,b,NULL,&rect[0]);
            SDL_DestroyTexture(b);
        }
        struct scoreboard_data a[4];
        for(int h=0;h<4;h++){
            a[h].type=h;
            a[h].point= game->scores[h];
        }
        char string[4][40];
        sprintf(string[0],"Human: %d",game->scores[0]);
        sprintf(string[1],"Orc: %d",game->scores[1]);
        sprintf(string[2],"Elf: %d",game->scores[2]);
        sprintf(string[3],"Undead: %d",game->scores[3]);
        qsort((void *)a,4,sizeof (struct scoreboard_data),compare);
        for(int f=0;f<4;f++){
            b = textLoader(string[a[f].type],yellow,font,renderer);
            SDL_RenderCopy(renderer,b,NULL,&rect[1+f]);
            SDL_DestroyTexture(b);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(5000);
        return 0;

    }

    return 1;
}
int ingame_menu_button(int x, int y){
    if(x <1100 && x>800){
        if(y<130 && y>80){
            return 1;
        }if(y<200 && y>150){
            return 2;
        }if(y<270 && y>220){
            return 3;
        }
        if(y<340 && y>290){
            return 4;
        }
    }
    return -1;
}
void in_game_menu(SDL_Renderer * renderer,struct map* game,int * x,struct troops_struct* troops,struct potion_struct*po,int total_p,char name[40]){
    SDL_Texture* background = loadTexture(renderer,"images/menu_background.jpg");
    SDL_Texture* button[4];
    button[0] = loadTexture(renderer,"images/resume.jpg");
    button[1] = loadTexture(renderer,"images/save_game.jpg");
    button[2] = loadTexture(renderer,"images/save_map.jpg");
    button[3] = loadTexture(renderer,"images/quit.jpg");
    SDL_Rect rect[4];
    rect[0].x = 800;
    rect[0].y = 80;
    rect[0].h= 50;
    rect[0].w = 300;
    rect[1].x = 800;
    rect[1].y = 150;
    rect[1].h= 50;
    rect[1].w = 300;
    rect[2].x = 800;
    rect[2].y = 220;
    rect[2].h= 50;
    rect[2].w = 300;
    rect[3].x = 800;
    rect[3].y = 290;
    rect[3].h= 50;
    rect[3].w = 300;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,background,NULL,NULL);
    for(int i=0;i<4;i++){
        SDL_RenderCopy(renderer,button[i],NULL,&rect[i]);
    }
    for(int i=0;i<4;i++){
        SDL_DestroyTexture(button[i]);
    }
    SDL_DestroyTexture(background);
    SDL_RenderPresent(renderer);
    int p=-1;
    SDL_Event events;
    while(p==-1){
        while (SDL_PollEvent(&events)!=0) {
            if(events.type == SDL_MOUSEBUTTONDOWN){
                int mouse_x,mouse_y;
                SDL_GetMouseState( &mouse_x, &mouse_y );
                p= ingame_menu_button(mouse_x,mouse_y);
            }
        }
    }
    if(p==4){
        *x =0;
        return;
    }else if(p==3){
        save_map();
        return;
    } else if(p==2){
        save_game(game,troops,po,total_p,name);
    } else{
        return;
    }
}


void main_menu_rect_init(SDL_Rect* rect){
    rect[0].x = 800;
    rect[0].y = 10;
    rect[0].h= 40;
    rect[0].w = 300;
    rect[1].x = 800;
    rect[1].y = 80;
    rect[1].h= 50;
    rect[1].w = 300;
    rect[2].x = 800;
    rect[2].y = 150;
    rect[2].h= 50;
    rect[2].w = 300;
    rect[3].x = 800;
    rect[3].y = 220;
    rect[3].h= 50;
    rect[3].w = 300;
    rect[4].x = 800;
    rect[4].y = 600;
    rect[4].h= 50;
    rect[4].w = 300;
}
bool name_input(SDL_Event* events,char name[40],int *i,int* shift){
    if(events->key.keysym.sym == SDLK_RETURN){
        if(name[0]!='\0'){
            name[*i] = 0;
            return true;
        }

    }else if(events->key.keysym.sym == SDLK_BACKSPACE) {
        if (*i > 0) {
            *i-=1;
            name[*i] = 0;
        }
    }else if(events->key.keysym.sym == SDLK_LSHIFT || events->key.keysym.sym == SDLK_RSHIFT){
        *shift=1;
    }else if(events->key.keysym.sym == SDLK_SPACE){

    }else{
        if(*i<38){
            name[*i] = events->key.keysym.sym + (*shift)*('A'-'a');
            *i+=1;
            *shift = 0;
        }

    }
    return false;
}
