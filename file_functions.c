//
// Created by moein111 on 31.01.22.
//
#include <string.h>
struct user{
    char username[40];
    int score
};
#include "file_functions.h"
int scores(SDL_Texture* t[4],SDL_Renderer* renderer,TTF_Font* font,SDL_Color *y){
    FILE* f_ptr = fopen("map_and_save/data.txt","r");
    char text[50];
    struct user list[4];
    for(int i=0;i<4;i++){
        fscanf(f_ptr,"%s %d\n",&list[i].username,&list[i].score);
    }
    for(int i=0;i<4;i++){
        sprintf(text,"%s %d",list[i].username, list[i].score);
        t[i] = textLoader(text,*y,font,renderer);
    }
    fclose(f_ptr);
}
int player_in_map(int n){
    FILE *ptr = NULL;
    int a=0;
    if(n==4){
        ptr = fopen("map_and_save/maps/map_random.txt","r");
    } else if(n==1){
        ptr = fopen("map_and_save/maps/map1.txt","r");
    }else if(n==2){
        ptr = fopen("map_and_save/maps/map2.txt","r");
    }else if(n==3){
        ptr = fopen("map_and_save/maps/map3.txt","r");
    }else {
        return 0;
    }
    fscanf(ptr,"%d\n",&a);
    fclose(ptr);
    return a;
}
struct map loading_map(int map_number){
    struct map game;
    memset(&game,0, sizeof(struct map));
    FILE* file_ptr;
    switch (map_number) {
        case 1:
            file_ptr = fopen("map_and_save/maps/map1.txt","r");
            break;
        case 2:
            file_ptr = fopen("map_and_save/maps/map2.txt","r");
            break;
        case 3:
            file_ptr = fopen("map_and_save/maps/map3.txt","r");
            break;
        case 4:
            file_ptr = fopen("map_and_save/maps/map_random.txt","r");
            break;
    }
    fscanf(file_ptr,"%d\n",&game.player_alive);
    fscanf(file_ptr,"%d\n",&game.total_territory);
    game.first_tr_ptr = (struct territory_struct*) malloc(sizeof (struct territory_struct)*game.total_territory);
    for(int i=0;i<game.total_territory;i++){
        fscanf(file_ptr,"%d %d %d %d %d %d %d\n",&game.first_tr_ptr[i].x_center,&game.first_tr_ptr[i].y_center,&game.first_tr_ptr[i].type,&game.first_tr_ptr[i].rotation,&game.first_tr_ptr[i].owner,&game.first_tr_ptr[i].troops,&game.first_tr_ptr[i].waiting_troops);
    }
    game.production_rate[4] = 1;
    game.production_rate[0] = 1;
    game.production_rate[1] = 1;
    game.production_rate[2] = 1;
    game.production_rate[3] = 1;
    game.troops_speed[0] = 2;
    game.troops_speed[1] = 2;
    game.troops_speed[2] = 2;
    game.troops_speed[3] = 2;
    fclose(file_ptr);
    return game;
}
void save_game(struct map* game,struct troops_struct* troops_list,struct potion_struct* potion_list,int potion,char name[40]){
    FILE * f_ptr = fopen("map_and_save/save.txt","w");
    fprintf(f_ptr,"%s\n",name);
    fprintf(f_ptr,"%d\n",game->player_alive);
    fprintf(f_ptr,"%d\n",game->total_territory);
    fprintf(f_ptr,"%d %d %d %d %d\n",game->production_rate[0],game->production_rate[1],game->production_rate[2],game->production_rate[3],game->production_rate[4]);
    fprintf(f_ptr,"%d %d %d %d\n",game->troops_speed[0],game->troops_speed[1],game->troops_speed[2],game->troops_speed[3]);
    fprintf(f_ptr,"%d %d %d %d\n",game->active_poition_type[0],game->active_poition_type[1],game->active_poition_type[2],game->active_poition_type[3]);
    fprintf(f_ptr,"%d %d %d %d\n",game->active_poition_time[0],game->active_poition_time[1],game->active_poition_time[2],game->active_poition_time[3]);
    fprintf(f_ptr,"%d %d %d %d\n",game->scores[0],game->scores[1],game->scores[2],game->scores[3]);
    for(int i=0;i< game->total_territory;i++){
        fprintf(f_ptr,"%d %d %d %d %d %d %d\n",game->first_tr_ptr[i].x_center,game->first_tr_ptr[i].y_center,game->first_tr_ptr[i].type,game->first_tr_ptr[i].rotation,game->first_tr_ptr[i].owner,game->first_tr_ptr[i].troops,game->first_tr_ptr[i].waiting_troops);
    }
    for(int i=0;i<1250;i++){
        fprintf(f_ptr,"%f %f %d %d %d\n",troops_list[i].x,troops_list[i].y,troops_list[i].from_t,troops_list[i].to_t,troops_list[i].owner);
    }
    fprintf(f_ptr,"%d\n",potion);
    for (int i=0;i<3;i++){
        fprintf(f_ptr,"%d %d %d\n",potion_list[i].x,potion_list[i].y,potion_list[i].type);
    }
    fclose(f_ptr);
}
void load_saved_game(struct map* game,struct troops_struct* troops_list,struct potion_struct* potion_list,int* potion){
    FILE * f_ptr = fopen("map_and_save/save.txt","r");
    char tmp[40];
    fscanf(f_ptr,"%s\n",tmp);
    fscanf(f_ptr,"%d\n",&game->player_alive);
    fscanf(f_ptr,"%d\n",&game->total_territory);
    fscanf(f_ptr,"%d %d %d %d %d\n",&game->production_rate[0],&game->production_rate[1],&game->production_rate[2],&game->production_rate[3],&game->production_rate[4]);
    fscanf(f_ptr,"%d %d %d %d\n",&game->troops_speed[0],&game->troops_speed[1],&game->troops_speed[2],&game->troops_speed[3]);
    fscanf(f_ptr,"%d %d %d %d\n",&game->active_poition_type[0],&game->active_poition_type[1],&game->active_poition_type[2],&game->active_poition_type[3]);
    fscanf(f_ptr,"%d %d %d %d\n",&game->active_poition_time[0],&game->active_poition_time[1],&game->active_poition_time[2],&game->active_poition_time[3]);
    fscanf(f_ptr,"%d %d %d %d\n",&game->scores[0],&game->scores[1],&game->scores[2],&game->scores[3]);
    game->first_tr_ptr = (struct territory_struct*) malloc(sizeof (struct territory_struct)*game->total_territory);
    for(int i=0;i< game->total_territory;i++){
        fscanf(f_ptr,"%d %d %d %d %d %d %d\n",&game->first_tr_ptr[i].x_center,&game->first_tr_ptr[i].y_center,&game->first_tr_ptr[i].type,&game->first_tr_ptr[i].rotation,&game->first_tr_ptr[i].owner,&game->first_tr_ptr[i].troops,&game->first_tr_ptr[i].waiting_troops);
    }

    for(int i=0;i<1250;i++){
        fscanf(f_ptr,"%f %f %d %d %d\n",&troops_list[i].x,&troops_list[i].y,&troops_list[i].from_t,&troops_list[i].to_t,&troops_list[i].owner);
    }
    fscanf(f_ptr,"%d\n",potion);
    for (int i=0;i<3;i++){
        fscanf(f_ptr,"%d %d %d\n",&potion_list[i].x,&potion_list[i].y,&potion_list[i].type);
    }
    fclose(f_ptr);

}
int compare_score(const struct user* p,const struct user*q){
    if(q->score > p->score){
        return 1;
    }
    return 0;
}
void update_score(char name[40],int score){
    FILE* f_ptr = fopen("map_and_save/data.txt","r");
    struct user list[10];
    memset(list,0,sizeof(list));
    for(int i=0;i<10;i++){
        fscanf(f_ptr,"%s %d\n",&list[i].username,&list[i].score);
    }
    int f=0;
    for(int i=0;i<10;i++){
        if(strcmp(name,list[i].username)==0){
            list[i].score +=score;
            if(list[i].score<0){
                list[i].score=0;
            }
            f=1;
            break;
        }
    }
    if(f==0){
        for(int i=0;i<10;i++){
            if(strcmp("NULL",list[i].username)==0){
                list[i].score =score;
                if(list[i].score<0){
                    list[i].score=0;
                }
                for(int j=0;j<40;j++){
                    list[i].username[j]=name[j];
                }
                f=2;
                break;
            }
        }
        if(f==0){
            list[9].score =score;
            if(list[9].score<0){
                list[9].score=0;
            }
            for(int j=0;j<40;j++){
                list[9].username[j]=name[j];
            }
        }
    }
    qsort(list,10,sizeof(struct user),compare_score);
    fclose(f_ptr);
    f_ptr = fopen("map_and_save/data.txt","w");
    for(int i=0;i<10;i++){
        fprintf(f_ptr,"%s %d\n",list[i].username,list[i].score);
    }
    fclose(f_ptr);

}
