#include "TaTeTi.h"

TaTeTi::TaTeTi() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			grid[i][j] = 0;
		}
	}
}

TaTeTi::~TaTeTi() {}

bool TaTeTi::SetPoint(int c, int r, int id) {
	if (grid[c][r] != 0)
		return false;

	grid[c][r] = id;
	return true;
}

int TaTeTi::GetPoint(int x, int y) {
	return grid[x][y];
}

bool TaTeTi::CheckVictory() {
	if (grid[0][0] == grid[0][1] && grid[0][0] == grid[0][2]){
		if (grid[0][0] != 0)
			return true;
	}
	else if (grid[1][0] == grid[1][1] && grid[1][0] == grid[1][2]) {
		if (grid[1][0] != 0)
			return true;
	}
	else if (grid[2][0] == grid[2][1] && grid[2][0] == grid[2][2]) {
		if (grid[2][0] != 0)
			return true;
	}
	else if (grid[0][0] == grid[1][0] && grid[0][0] == grid[2][0]) {
		if (grid[0][0] != 0)
			return true;
	}
	else if (grid[0][1] == grid[1][1] && grid[0][1] == grid[2][1]) {
		if (grid[0][1] != 0)
			return true;
	}
	else if (grid[0][2] == grid[1][2] && grid[0][2] == grid[2][2]) {
		if (grid[0][2] != 0)
			return true;
	}
	else if (grid[0][0] == grid[1][1] && grid[0][0] == grid[2][2]) {
		if (grid[0][0] != 0)
			return true;
	}
	else if (grid[0][2] == grid[1][1] && grid[0][2] == grid[2][0]) {
		if (grid[0][2] != 0)
			return true;
	}

	return false;
}

bool TaTeTi::CheckTie() {

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (grid[i][j] == 0)
				return false;
		}
	}

	if (!CheckVictory())
		return true;

	return false;
}