

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/time.h>

#define MAX_HEIGHT 20 ///<Константа, определяющая высоту игрового поля
#define MAX_WIDTH 14 ///<Константа, определяющая ширину игрового поля

/*!
    Структура, определяющая фигуру
*/
typedef struct shape{
    int x; ///<Координата фигуры по x от левой верхней границы
    int y; ///<Координата фигуры по y от левой верхней границы
    int width; ///<Ширина фигуры
    char **shape_array; ///<Массив, в которой записана фигура
    int color; ///<Цвет фигуры
}Shape;


//CLI LOGIC
int handle_menu_option(int choice);
void game_cli();

//GAME LOGIC
int main_loop(WINDOW *game_status_window, WINDOW *gamefield, WINDOW *score);
void print_new_shape(WINDOW *game_status_window, Shape *shape);
void check_for_full_line(char **table, int *score, int *level, int *speed);
void clear_line(char **table, int line_number);
void write_shape_to_table(Shape shape, char** table);
void move_shape(Shape *shape, char direction, char** Table);
int check_if_touches_right_border(Shape shape, char** table);
int check_if_touches_left_border(Shape shape, char** table);
int check_if_touches_another_shape(Shape shape, char** table);
void rotate_shape(Shape *shape, char **table);
int print_table(WINDOW *gamefield, Shape current_shape, char** table, WINDOW *score, WINDOW *game_status_window, int score_counter, Shape next_shape, int pause_flag, char **buffer, int speed, int level);
int check_for_lose(char **table);
char **create_and_fill_buffer(Shape current_shape, char** table);
void clear_table_memory(char** table);

//HIGHSCORE LOGIC

void update_highscore(int score);
int read_highscore();


//BLOOP

Shape get_rand_shape();