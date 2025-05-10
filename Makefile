CC = gcc
CURSES_FLAG = -lncursesw
CHECK_FLAGS = -lcheck -lpthread -lrt -lm -lsubunit

game: tetris.c cli.c
	$(CC) -o game tetris.c cli.c highscore_logic.c $(CURSES_FLAG)
	./game

test: tetris.c test.c
	$(CC) -o test tetris.c test.c $(CURSES_FLAG) $(CHECK_FLAGS)
	./test

lcov_report:
	$(CC) -o lcov_report test.c tetris.c -fprofile-arcs -ftest-coverage $(CURSES_FLAG) $(CHECK_FLAGS)
	./lcov_report
	mkdir coverage
	mv *.gcda coverage
	mv *.gcno coverage
	lcov --capture --directory coverage --output-file coverage/coverage.info
	genhtml coverage/coverage.info --output-directory final_report

sanitize:
	$(CC) -o sanitize tetris.c cli.c $(CURSES_FLAG) -fsanitize=address

clean:
	rm -rf coverage final_report game lcov_report *.gcda *.gcno test
