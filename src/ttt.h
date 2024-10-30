#ifndef TTT_HEADER
#define TTT_HEADER

#include <stdbool.h>

typedef char table_t[9];
typedef char byte;

enum game_flags {
	NONE,
	X_WON,
	O_WON,
	DRAW,
};

byte ttt_check_win(table_t tb);
bool ttt_fields_filled(table_t tb);
bool ttt_set(table_t tb, byte index, byte value);

#endif