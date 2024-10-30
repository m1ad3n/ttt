#include "ttt.h"

bool ttt_fields_filled(table_t tb) {
	for (int i = 0; i < 9; i++)
		if (tb[i] == 0)
			return false; // at least one field is not filled

	// all fields all used
	return true;
}

byte ttt_check_win(table_t tb) {
	// 1 0 0
	// 1 0 0
	// 1 0 0
	for (int i = 0; i < 3; i++) {
		if (tb[i] == tb[3 + i]
			&& tb[i] == tb[6 + i]
			&& tb[i] != 0)
			return (tb[i] == 'X') ? X_WON : O_WON;
	}

	// 1 1 1
	// 0 0 0
	// 0 0 0
	for (int i = 0; i <= 6; i += 3) {
		if (tb[i] == tb[1 + i]
			&& tb[i] == tb[2 + i]
			&& tb[i] != 0)
			return (tb[i] == 'X') ? X_WON : O_WON;
	}

	// 1 0 0
	// 0 1 0
	// 0 0 1
	if (tb[0] == tb[4]
		&& tb[0] == tb[8]
		&& tb[0] != 0)
		return (tb[0] == 'X') ? X_WON : O_WON;

	// 0 0 1
	// 0 1 0
	// 1 0 0
	if (tb[2] == tb[4]
		&& tb[2] == tb[6]
		&& tb[2] != 0)
		return (tb[2] == 'X') ? X_WON : O_WON;

	if (ttt_fields_filled(tb))
		return DRAW;
	return NONE;
}

bool ttt_set(table_t tb, byte index, byte value) {
	if (!(index >= 1 && index <= 9)) return false;
	if (tb[(int)--index] == 0) {
		tb[(int)index] = value;
		return true;
	}
	return false;
}