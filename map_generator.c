//
// Created by moein111 on 01.02.22.
//

#define territory_types 9
#include "map_generator.h"
int territory_conflict(int x,int y,int placed_territory,struct territory_struct territory[]){
    for(int i =0;i<placed_territory;i++){
        if(abs(x - territory[i].x_center)<110 && abs(y- territory[i].y_center)<90){
            return 1;
        }
    }
    return 0;
}
struct map random_map(int player,int total_t){
    struct map random_map;
    memset(&random_map,0, sizeof(struct map));
    random_map.total_territory = total_t;
    random_map.first_tr_ptr = (struct territory_struct*) malloc(sizeof (struct territory_struct)*random_map.total_territory);
    memset(random_map.first_tr_ptr,0,sizeof (struct territory_struct)*random_map.total_territory);
    random_map.player_alive = player;
    int placed_territory=0,is_conflict,random_x,random_y,each_player_t=0;
    for(int j=0;j<player;j++){
        random_map.players_t[j] = 1+ rand()%2;
        each_player_t =0;
        for(;each_player_t<random_map.players_t[j];placed_territory++,each_player_t++){
            is_conflict = 1;
            while (is_conflict){
                random_x = 60 + (rand()%1190);
                random_y = 60 + (rand()%630);
                is_conflict= territory_conflict(random_x,random_y,placed_territory,random_map.first_tr_ptr);
            }
            random_map.first_tr_ptr[placed_territory].x_center = random_x;
            random_map.first_tr_ptr[placed_territory].y_center = random_y;
            random_map.first_tr_ptr[placed_territory].owner = 1 +j;
            random_map.first_tr_ptr[placed_territory].rotation = rand() %360;
            random_map.first_tr_ptr[placed_territory].type = rand() % territory_types;
            random_map.first_tr_ptr[placed_territory].troops = 3 + (random_map.players_t[j] ==1)*7;
        }
    }
    for(;placed_territory<random_map.total_territory;placed_territory++){
        is_conflict = 1;
        while (is_conflict){
            random_x = 60 + (rand()%1190);
            random_y = 60 + (rand()%630);
            is_conflict= territory_conflict(random_x,random_y,placed_territory,random_map.first_tr_ptr);
        }
        random_map.first_tr_ptr[placed_territory].x_center = random_x;
        random_map.first_tr_ptr[placed_territory].y_center = random_y;
        random_map.first_tr_ptr[placed_territory].type = rand() % territory_types;
        random_map.first_tr_ptr[placed_territory].rotation = rand() %360;
        random_map.first_tr_ptr[placed_territory].owner = 0;
        random_map.first_tr_ptr[placed_territory].troops =8;
    }
    random_map.production_rate[4] = 1;
    random_map.production_rate[0] = 1;
    random_map.production_rate[1] = 1;
    random_map.production_rate[2] = 1;
    random_map.production_rate[3] = 1;
    random_map.troops_speed[0] = 2;
    random_map.troops_speed[1] = 2;
    random_map.troops_speed[2] = 2;
    random_map.troops_speed[3] = 2;
    FILE* file_ptr = fopen("map_and_save/maps/generated map.txt","w");
    fprintf(file_ptr,"%d\n",random_map.player_alive);
    fprintf(file_ptr,"%d\n",random_map.total_territory);
    for(int i=0;i<random_map.total_territory;i++){
        fprintf(file_ptr,"%d %d %d %d %d %d %d\n",random_map.first_tr_ptr[i].x_center,random_map.first_tr_ptr[i].y_center,random_map.first_tr_ptr[i].type,random_map.first_tr_ptr[i].rotation,random_map.first_tr_ptr[i].owner,random_map.first_tr_ptr[i].troops,random_map.first_tr_ptr[i].waiting_troops);
    }
    fclose(file_ptr);
    return random_map;

}
void save_map(){
    FILE* filef_ptr = fopen("map_and_save/maps/generated map.txt","r");
    FILE* filet_ptr = fopen("map_and_save/maps/map_random.txt","w");
    int a;
    fscanf(filef_ptr,"%d\n",&a);
    fprintf(filet_ptr,"%d\n",a);
    fscanf(filef_ptr,"%d\n",&a);
    fprintf(filet_ptr,"%d\n",a);
   int b[7];
   for(int i=0;i<a;i++){
       fscanf(filef_ptr,"%d %d %d %d %d %d %d\n",&b[0],&b[1],&b[2],&b[3],&b[4],&b[5],&b[6]);
       fprintf(filet_ptr,"%d %d %d %d %d %d %d\n",b[0],b[1],b[2],b[3],b[4],b[5],b[6]);
   }
    fclose(filef_ptr);
    fclose(filet_ptr);
}
