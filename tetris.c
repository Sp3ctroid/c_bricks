/*!
    @file tetris.c
    @brief Вся игровая логика
*/

#include "tetris.h"
#include <unistd.h>

/*!
    @brief Проверка на то, что поворот возможно осуществить

    Поворот фигуры невозможен, если она выходит за границы игрового поля при этом повороте
    @param shape Структура с информацией о фигуре
    @param table Игровое поле. Необходим для проверки поворота рядом с уже записанными фигурами
    @return 1 если поворот возможен, иначе 0

     tetris.c check_nonvalid_rotation
*/

int check_nonvalid_rotation(Shape shape, char** table){
    for(int i = 0; i < shape.width; i++){
        for(int j = 0; j < shape.width; j++){
            if(shape.shape_array[i][j] && shape.x + j >= MAX_WIDTH || 
                shape.shape_array[i][j] && shape.y + i >= MAX_HEIGHT || 
                shape.shape_array[i][j] && shape.x + j < 0 || 
                shape.shape_array[i][j] && shape.y + i < 0 || 
                shape.shape_array[i][j] && table[shape.y + i][shape.x + j]){
                return 0;
            }
        }
    }
    return 1;
}


/*!
    @brief Поворот фигуры

    @param shape Указатель на текущую фигуру.
    @param table Игровое поле. Передаётся в check_nonvalid_rotation
     tetris.c rotate_shape

*/

void rotate_shape(Shape *shape, char** table){

    for(int i = 0; i < shape->width; i++){
        for(int j = i + 1; j < shape->width; j++){
            char temp = shape->shape_array[i][j];
            shape->shape_array[i][j] = shape->shape_array[j][i];
            shape->shape_array[j][i] = temp;
        }
    }

    //reverse each row
    for(int i = 0; i < shape->width; i++){
        int start = 0, end = shape->width - 1;
        while(start < end){
            char temp = shape->shape_array[i][start];
            shape->shape_array[i][start] = shape->shape_array[i][end];
            shape->shape_array[i][end] = temp;
            start++;
            end--;
        }
    }
    if (!check_nonvalid_rotation(*shape, table)){
        rotate_shape(shape, table);
    }
}


/*!
    @brief Проверяет пересечение фантома и текущей фигуры

    @param shape Указатель на текущую фигуру.
    @param land_shape Указатель на фантом
    @param buffer Буффер символов игрового поля

    @return int - 1 если пересечение есть, иначе 0

     tetris.c check_colored_intersection
*/

int check_colored_intersection(Shape shape, Shape land_shape, char** buffer){
    for(int i = 0; i < shape.width; i++){
        for(int j = 0; j < shape.width; j++){
            if((shape.x + j == land_shape.x + j) && (shape.y + i == land_shape.y + 1)){
                return 0;
            }
        }
    }
    return 1;
}

//LCOV_EXCL_START

/*!
    @brief Создание временного массива и его заполнение данными игрового поля, текущей фигуры и фантома

    @param current_shape Указатель на текущую фигуру.
    @param table Игровое поле

     tetris.c createandfillbuffer
*/

char **create_and_fill_buffer(Shape current_shape, char** table){
    char **buffer = (char**)calloc(MAX_HEIGHT, sizeof(char*));
    for(int i = 0; i < MAX_HEIGHT; i++){
        buffer[i] = (char*)calloc(MAX_WIDTH, sizeof(char));
    }

    Shape land_point_shape = current_shape;
    while(!check_if_touches_another_shape(land_point_shape, table)){
        move_shape(&land_point_shape, 'd', table);
    }

    for(int i = 0; i < current_shape.width; i++){
        for(int j = 0; j < current_shape.width; j++){
            if(current_shape.shape_array[i][j]) 
                buffer[current_shape.y + i][current_shape.x + j] = current_shape.shape_array[i][j];

            if(land_point_shape.shape_array[i][j] && check_colored_intersection(current_shape, land_point_shape, buffer)) 
                buffer[land_point_shape.y + i][land_point_shape.x + j] = land_point_shape.shape_array[i][j];
        }
    }

    return buffer;
}

/*!
    @brief Очистка памяти игрового поля

    @param table Игровое поле

     tetris.c clear_table_memory
*/

void clear_table_memory(char** table){
    for(int i = 0; i < MAX_HEIGHT; i++){
        free(table[i]);
    }
    free(table);
}



//LCOV_EXCL_STOP

/*!
    @brief Проверка касание с другой фигурой и нижней границей игрового поля

    @param shape Указатель на текущую фигуру.
    @param table Игровое поле

    @return int - 1 если касание есть, иначе 0

     tetris.c check_if_touches_another_shape
*/

int check_if_touches_another_shape(Shape shape, char** table){

    for(int i = 0; i < shape.width; i++){
        for(int j = 0; j < shape.width; j++){
            if((shape.shape_array[i][j] && shape.y + i + 1 >= MAX_HEIGHT)){
                return 1;
            }
            if((shape.shape_array[i][j] && table[shape.y + i + 1][shape.x + j])){
                return 1;
            }
        }
    }

    return 0;
}


/*!
    @brief Проверка касание с другой фигурой и левой границей игрового поля

    @param shape Указатель на текущую фигуру.
    @param table Игровое поле

    @return int - 1 если касание есть, иначе 0

     tetris.c check_if_touches_left_border

*/

int check_if_touches_left_border(Shape shape, char** table){
    for(int i = 0; i < shape.width; i++){
        for(int j = 0; j < shape.width; j++){
            if((shape.shape_array[i][j] && shape.x + j <= 0) || (shape.shape_array[i][j] && table[shape.y + i][shape.x + j - 1])){
                return 1;
            }
        }
    }
    return 0;
}

/*!
    @brief Проверка касание с другой фигурой и правой границей игрового поля

    @param shape Указатель на текущую фигуру.
    @param table Игровое поле

    @return int - 1 если касание есть, иначе 0

     tetris.c check_if_touches_right_border

*/

int check_if_touches_right_border(Shape shape, char** table){
    for(int i = 0; i < shape.width; i++){
        for(int j = 0; j < shape.width; j++){
            if((shape.shape_array[i][j] && shape.x + j + 1 >= MAX_WIDTH) || (shape.shape_array[i][j] && table[shape.y + i][shape.x + j + 1])){
                return 1;
            }
        }
    }
    return 0;
}


/*!
    @brief Перемещение фигуры

    Функция меняет координату фигуры в зависимости от выбраного направления движения.
    @param shape Указатель на текущую фигуру.
    @param direction Направление перемещения
    @param Table Игровое поле

     tetris.c move_shape
*/

void move_shape(Shape *shape, char direction, char** Table){
    if(direction == 'd' && !check_if_touches_another_shape(*shape, Table)){
        shape->y++;
    }
    if(direction == 'l' && !check_if_touches_left_border(*shape, Table)){
        shape->x--;
    }
    if(direction == 'r' && !check_if_touches_right_border(*shape, Table)){
        shape->x++;
    }
    if(direction == 'u' && shape->y > 0){
        shape->y--;
    }
}


/*!
    @brief Функция вписывает фигуру в игровое поле

    @param shape Указатель на текущую фигуру.
    @param table Игровое поле

     tetris.c write_shape_to_table
*/

void write_shape_to_table(Shape shape, char** table){

    for(int i = 0; i < shape.width; i++){
        for(int j = 0; j < shape.width; j++){
            if(shape.shape_array[i][j]) 
                table[shape.y + i][shape.x + j] = shape.shape_array[i][j];
        }
    }
}


/*!
    @brief Функция обращает значения строки в 0

    @param table Игровое поле
    @param line_number Номер строки

     tetris.c clear_line
*/

void clear_line(char **table, int line_number){
    for (int i = 0; i < MAX_WIDTH; i++){
        table[line_number][i] = 0;
    }
}


/*!
    @brief Функция перемещает строки вниз

    @param table Игровое поле
    @param line_number Номер строки с которой начинаем перемещение

     tetris.c move_lines_down
*/

void move_lines_down(char **table, int line_number){
    for(int i = line_number; i > 0; i--){
        for(int j = 0; j < MAX_WIDTH; j++){
            table[i][j] = table[i - 1][j];
        }
    }
}


void increase_level(int *level, int score, int *speed){
    if (*level < 10 && score != 0){
        *level += 1;
        *speed += 1;
    }
        
}

int define_added_score(int consecutive_lines){
    switch(consecutive_lines){
        case 1:
            return 100;
        case 2:
            return 300;
        case 3:
            return 700;
        case 4:
            return 1500;
        default:
            return 0;
    }
}

/*!
    @Функция ищет строку, которая полностью состоит из 1

    В случае нахождения строки, очищает её и обновляет очки
    @param table Игровое поле
    @param score Указатель на очки

     tetris.c check_for_full_line
*/

void check_for_full_line(char **table, int *score, int *level, int *speed){

    int consecituve_lines = 0;
    int counter = 0;
    int line_counter = 0;
    
    for(int i = 0; i < MAX_HEIGHT; i++){
        for(int j = 0; j < MAX_WIDTH; j++){    
            if (table[i][j] == 0){
                counter = 0;
                continue;
            }
            counter++;
            if(counter == MAX_WIDTH){
                line_counter = i;
                clear_line(table, line_counter);
                move_lines_down(table, line_counter);
                consecituve_lines++;
                counter = 0;
            }
        }
    }
    int added_score = define_added_score(consecituve_lines);
    *score += added_score;
    if (added_score != 0 && *score >= 600 * *level){
        increase_level(level, *score, speed);
    }
}

//LCOV_EXCL_START

//LCOV_EXCL_STOP

/*!
    @brief Функция проверяет самую верхнюю строку игрового поля.A_LOW

    @param table Игровое поле

    @return int - 1 если самая верхняя строка заполнена, иначе 0
    
     tetris.c check_for_lose
*/

int check_for_lose(char **table){
    
    for(int i = 0; i < MAX_WIDTH; i++){
        if(table[0][i] == 1){
            return 1;
        }
    }

    return 0;
}


/**
    @brief Генерирует следующую фигуру

    @param[in] ShapesArr Массив фигур
    @param[out] next_shape Указатель на следующую фигуру
    @param[out] flag_generated_next_shape Флаг генерации следующей фигуры.

     tetris.c getnextshape
*/

void get_next_shape(Shape ShapesArr[], Shape *next_shape, int *flag_generated_next_shape){
    if(!*flag_generated_next_shape){
        *next_shape = ShapesArr[rand() % 7];
        next_shape->x = rand() & (MAX_WIDTH - next_shape->width);
        *flag_generated_next_shape = 1;
    }
}



/**
    @brief Обрабатывает ввод с клавиатуры

    @param input Ввод с клавиатуры
    @param current_shape Указатель на текущую фигуру
    @param table Игровое поле
    @param pause_flag Указатель на флаг паузы
    @param next_shape Указатель на следующую фигуру
    @param flag_generated_next_shape Флаг генерации следующей фигуры

    \snippet tetris.c parseinput
*/

void parse_input(int input, Shape *current_shape, char** Table, int *pause_flag, Shape *next_shape, int *flag_generated_next_shape, int *check_for_manual_exit){
    switch(input){
        case KEY_LEFT:
        if(*pause_flag == 1){
            move_shape(current_shape, 'l', Table);
        }
        break;
        case KEY_RIGHT:
        if(*pause_flag == 1){
            move_shape(current_shape, 'r', Table);
        }
        break;
        case KEY_DOWN:
        if(*pause_flag == 1){
            if(!check_if_touches_another_shape(*current_shape, Table)){
                current_shape->y++;
            }

        }
        break;
        case 'p':
        if(*pause_flag == 1 || *pause_flag == -1){
            *pause_flag *= -1;
        }
        break;
        case 'r':
        rotate_shape(current_shape, Table);
        break;
        case 'q':
        *check_for_manual_exit = 1;
        break;
    }
}


//LCOV_EXCL_START


/*!
    @brief Главный цикл игры. 
    
    @param game_status_window Указатель на окно игрового статуса
    @param gamefield Указатель на окно игрового поля
    @param score Указатель на очки

     tetris.c mainloop
*/

int main_loop(WINDOW *game_status_window, WINDOW *gamefield, WINDOW *score) {

    char** Table;
    Table = (char**)calloc(MAX_HEIGHT, sizeof(char*));
    for(int i = 0; i < MAX_HEIGHT; i++){
        Table[i] = (char*)calloc(MAX_WIDTH, sizeof(char));
    }

    Shape ShapesArr[7] = {{0, 0, 2, (char *[]){(char []){1,1},
                                              (char []){1,1}}, 1},

                          {0, 0, 3, (char*[]){(char[]){1,1,0}, 
                                              (char[]){0,1,1}, 
                                              (char[]){0,0,0}}, 2},

                          {0, 0, 3, (char*[]){(char[]){0,1,0},
                                              (char[]){1,1,1},
                                              (char[]){0,0,0}}, 3},

                          {0, 0, 3, (char*[]){(char[]){0,1,1}, 
                                              (char[]){1,1,0}, 
                                              (char[]){0,0,0}}, 4},

                          {0, 0, 3, (char*[]){(char[]){1,0,0}, 
                                              (char[]){1,1,1}, 
                                              (char[]){0,0,0}}, 5},

                          {0, 0, 3, (char*[]){(char[]){0,0,1}, 
                                              (char[]){1,1,1}, 
                                              (char[]){0,0,0}}, 6},

                          {0, 0, 4, (char*[]){(char[]){1,1,1,1},
                                              (char[]){0,0,0,0}, 
                                              (char[]){0,0,0,0},
                                              (char[]){0,0,0,0}}}, 7};
                                              
                                              
    Shape current_shape = ShapesArr[rand() % 7];
    
    current_shape.x = rand() & (MAX_WIDTH - current_shape.width);
    Shape next_shape;
    int flag_generated_next_shape = 0;

    int check_for_manual_exit = 0;
    int score_counter = 0;
    int pause_flag = 1;

    int level = 1;

    struct timeval start_time;
    struct timeval end_time;

    

    double timer = 1000;
    double level_speed_diff;
    int speed = 1;
    double gradual_piece_speed = 15.0;
    gettimeofday(&start_time, NULL);

    while(!check_for_lose(Table) && !check_for_manual_exit){
        
        get_next_shape(ShapesArr, &next_shape, &flag_generated_next_shape);

        char **buffer = create_and_fill_buffer(current_shape, Table);
        print_table(gamefield, current_shape, Table, score, game_status_window, score_counter, next_shape, pause_flag, buffer, speed, level);
        clear_table_memory(buffer);

        int input = getch();
        parse_input(input, &current_shape, Table, &pause_flag, &next_shape, &flag_generated_next_shape, &check_for_manual_exit);

        gettimeofday(&end_time, NULL);
        level_speed_diff = timer - (double)level * 70 - gradual_piece_speed;

        if(end_time.tv_sec * 1000 + end_time.tv_usec / 1000 - start_time.tv_sec * 1000 - start_time.tv_usec / 1000 > level_speed_diff && pause_flag == 1){
            gettimeofday(&start_time, NULL); 
            if(check_if_touches_another_shape(current_shape, Table)){
                write_shape_to_table(current_shape, Table);
                current_shape = next_shape;
                flag_generated_next_shape = 0;
                gradual_piece_speed = 15.0;
            }
            move_shape(&current_shape, 'd', Table);
            gradual_piece_speed += 15.0;
            
        }
        
        check_for_full_line(Table, &score_counter, &level, &speed);
        
        
    }

    clear_table_memory(Table);
    update_highscore(score_counter);
    return 0;
    
}

//LCOV_EXCL_STOP
