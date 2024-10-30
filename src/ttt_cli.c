#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include "ttt.h"

/* macro for moving cursor up lines */
#define CURSOR_UP(x) printf("\033[%dF", x);
#define CURSOR_LEFT(x) printf("\033[%dD", x);

/* global variables */
static struct termios old_pref;

byte ttt_getch(void);
void ttt_display(table_t tb);

int main(void) {

	// tic tac toe fields
	table_t table = {0};

	bool
		turn_flag = false, // false - player x true - player o
		end_game = false;

	while (!end_game) {

		// display the table
		ttt_display(table);

		// which player turn it is
		byte pv = (turn_flag) ? 'O' : 'X';

		// get player input
		int pli = 0;
		while (!ttt_set(table, pli, pv)) {
			CURSOR_LEFT(10);
			printf("[%c] >> ", pv);
			pli = ttt_getch();
			putchar(' ');

			// user pressed 'q'
			if (pli == 65) {
				end_game = true;
				break;
			}
		}

		// set the cur pos before next table
		CURSOR_UP(6);

		int cw = ttt_check_win(table);
		if (cw != NONE) {
			// final view of the table
			ttt_display(table);

			if (cw == DRAW) {
				printf("DRAW\n");
				return 0;
			}

			printf("PLAYER %c WON\n", pv);
			return 0;
		}

		// toggle players turn
		turn_flag ^= 1;

	}

	return 0;
}

__attribute__((constructor))
void ttt_init(void) {
	struct termios newattr;
	tcgetattr( STDIN_FILENO, &old_pref );
	newattr = old_pref;
	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
}

__attribute__((destructor))
void ttt_destroy(void) {
	// set the old terminal pref
	tcsetattr(STDIN_FILENO, TCSANOW, &old_pref);
}

byte ttt_getch(void) {
	return getchar() - '0'; // get user input
}

void ttt_display(table_t tb) {

	for (int i = 0; i < 9; i++) {
		printf(" %c ", (tb[i] == 0) ? ' ' : tb[i]);
		if ((i + 1) % 3 == 0 && i != 8) {
			putchar('\n');
			for (int j = 1; j <= 11; j++) {
				if (j == 4 || j== 8) printf("\u253C");
				else printf("\u2500");
			}
			putchar('\n');
		}
		else if (i != 8) {
			printf("\u2502");
		}
	}
	printf("\n\n");
}
