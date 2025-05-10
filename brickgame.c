#include "tetris.h"

int brick_game() {
    char** Table;
    Table = (char**)calloc(MAX_HEIGHT, sizeof(char*));
    for(int i = 0; i < MAX_HEIGHT; i++){
        Table[i] = (char*)calloc(MAX_WIDTH, sizeof(char));
    }

    Shape current_shape = get_rand_shape();
    current_shape.x = rand() & (MAX_WIDTH - current_shape.width);
    
    while(!check_for_lose(Table)) {
        
    }
}
