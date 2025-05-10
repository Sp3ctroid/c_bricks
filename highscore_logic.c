/*!
    @file highscore_logic.c
    @brief Логика взаимодействия с файлом, хранящим рекорд

    @warning Данный код работает только если в папке с программой присутствует файл highscore.txt
*/

#include "tetris.h"

/*!
    @brief Функция обновляет рекорд
    @param score Набранное количество очков

     tetris.c update_highscore
*/

void update_highscore(int score) {
    int highscore = read_highscore();
    if (score > highscore) {
        highscore = score;
        FILE *highscore_file = fopen("highscore.txt", "w");
        fprintf(highscore_file, "%d", highscore);
        fclose(highscore_file);
    }

}


/*!
    @brief Функция считывает рекорд из файла

    @return int - рекорд

     tetris.c read_highscore
*/

int read_highscore() {
    FILE *highscore_file = fopen("highscore.txt", "r");
    int highscore = 0;
    fscanf(highscore_file, "%d", &highscore);
    fclose(highscore_file);
    return highscore;
}

