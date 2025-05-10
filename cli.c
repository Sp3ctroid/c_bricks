/*!
    @file cli.c
    @brief Логика пользовательского интерфейса
*/

#include "tetris.h"
#include <locale.h>

/*!
    \brief Функция печатает новую фигуру в окно игрового статута

    \param game_status_window Указатель на окно игрового статуса
    \param shape Указатель на текущую фигуру
    
     tetris.c print_new_shape
*/
 
void print_new_shape(WINDOW *game_status_window, Shape *shape){
    
    for(int i = 0; i < shape->width; i++){
        for(int j = 0; j < shape->width; j++){
            mvwaddstr(game_status_window, i + shape->width, 2* j + shape->width + 3, shape->shape_array[i][j] ? "\u2588\u2588" : "  ");
        }
    }

}
 


/*!
    @brief Печатает игровое поле

    Функция осуществляет вывод игрвого поля в терминал. Отрисовывает текущую фигуру и фантом фигуры.
    @param gamefield Указатель на игровое поле
    @param current_shape Указатель на текущую фигуру
    @param table Игровое поле
    @param score Окно для вывода набранных очков
    @param game_status_window Окно для вывода информации по игре (следующая фигура, пауза)
    @param score_counter Счетчик очков
    @param next_shape Указатель на следующую фигуру
    @param pause_flag Флаг паузы
    @param buffer Временный предзаполненный массив, который вписывается в поле

     cli.c printtable
*/

 

int print_table(WINDOW *gamefield, Shape current_shape, char** table, WINDOW *score, WINDOW *game_status_window, int score_counter, Shape next_shape, int pause_flag, char **buffer, int speed, int level) {

    for (int i = 0; i < MAX_HEIGHT; i++){
        
        for (int j = 0; j < MAX_WIDTH; j++){
            wattron(gamefield, COLOR_PAIR(8));
            if(table[i][j] + buffer[i][j]){
                wattron(gamefield, COLOR_PAIR(6));
            }
            if(buffer[i][j]){

                wattron(gamefield, COLOR_PAIR(4));


            }
            mvwaddstr(gamefield, i + 1, 2*j + 1, table[i][j] + buffer[i][j] ? "  ":"  ");

            wattroff(gamefield, A_COLOR);
        }
        wprintw(gamefield, "\n");
    }
    
    box(game_status_window, 0, 0);
    mvwprintw(game_status_window, 1, 1, "Next Shape:");
    print_new_shape(game_status_window, &next_shape);
    mvwprintw(game_status_window, 7, 5, "SPEED: %d", speed);
    mvwprintw(game_status_window, 9, 5, "LEVEL: %d", level);    
    if(pause_flag == -1){
        mvwprintw(game_status_window, 7, 5, "PAUSED");
    }
    wrefresh(game_status_window);
    mvwprintw(score,1,1, "Score: %d", score_counter);
    box(score, 0, 0);
    wrefresh(score);
    box(gamefield, 0, 0);
    wrefresh(gamefield);
    refresh();
    werase(gamefield);
    werase(score);
    werase(game_status_window);

    
}
 
/*!
    @brief Создаёт окна для игры, запускает игру

    @param[out] game_status_window Окно для вывода информации по игре (следующая фигура, пауза)
    @param[out] gamefield Окно для вывода игрового поля
    @param[out] score Окно для вывода набранных очков 

     cli.c game_cli
*/

 
void game_cli() {
    clear();

    initscr();
    start_color();
    init_pair(4, COLOR_CYAN, COLOR_CYAN);
    init_pair(6, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);
    nodelay(stdscr, true);
    curs_set(0);
    keypad(stdscr, true);
    cbreak();
    noecho();

    WINDOW *score = newwin(3, 50, 7, 20);
    WINDOW *game_status_window = newwin(MAX_HEIGHT + 2, 20, 10, 50);
    WINDOW *gamefield = newwin(MAX_HEIGHT + 2, (MAX_WIDTH + 1)*2, 10, 20);

    refresh();
    main_loop(game_status_window, gamefield, score);
    delwin(score);
    delwin(game_status_window);
    delwin(gamefield);
    clear();
    endwin();
}
 

/*!
    @brief Обрабатывает выбор пользователя в меню

    @param choice Выбор пользователя. 0 - играть, 1 - выход

     cli.c handle_menu_option
*/
 
int handle_menu_option(int choice) {
    if(choice == 0){
        clear();
        game_cli();
    }

    if(choice == 1){
        clear();
        delwin(stdscr);
        endwin();
        exit(0);
    }
}
 

int main() {
    setlocale(LC_CTYPE, "en_US.UTF-8");
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, true);
    keypad(stdscr, true);

    WINDOW *title_win = newwin(3, 21, LINES/2 - 3, COLS/2 - 15);
    WINDOW *menu_win = newwin(5, 21, LINES/2, COLS/2 - 15);
    WINDOW *controls_win = newwin(5, 50, LINES/2 + 3, COLS/2 - 15);
    refresh();
    

    int choice = 0;
    int old_lines = LINES;
    int old_cols = COLS;

    while(true){
        
        refresh();
        
        mvwin(title_win, LINES/2 - 3, COLS/2 - 15);
        mvwprintw(title_win, 1, 3, "BRICK GAME 1.0");
        box(title_win, 0, 0);
        wrefresh(title_win);

        mvwin(menu_win, LINES/2, COLS/2 - 15);
        mvwprintw(menu_win, 1, 5, "Start Game");
        mvwprintw(menu_win, 3, 8, "Exit");
        box(menu_win, 0, 0);

        mvwin(controls_win, LINES/2 + 5, COLS/2 - 29);
        mvwprintw(controls_win, 1, 10, "Arrow keys - move, R - rotate");
        mvwprintw(controls_win, 3, 14, "P - pause, Q - exit");
        box(controls_win, 0, 0);
        wrefresh(controls_win);

        if(old_cols != COLS || old_lines != LINES){
            touchwin(stdscr);
            old_cols = COLS;
            old_lines = LINES;
        }
        
        
        switch(getch()){
            case KEY_UP:
            choice--;
            if(choice < 0){
                choice = 1;
            }
            break;
            case KEY_DOWN:
            choice++;
            if (choice > 1){
                choice = 0;
            }
            break;
            case '\n':
            handle_menu_option(choice);
            break;
        }
        if(choice == 0){
            mvwaddch(menu_win, 1, 3, '>');
            mvwaddch(menu_win, 1, 16, '<');
        }

        if(choice == 1){
            mvwaddch(menu_win, 3, 6, '>');
            mvwaddch(menu_win, 3, 13, '<');
        }
        wrefresh(menu_win);
        werase(menu_win);
        werase(title_win);
    }

    endwin();
    
}



