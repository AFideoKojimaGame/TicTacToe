#pragma once
#include <iostream>

#define ROWS 3
#define COLS 3

using namespace std;

class TaTeTi {

public:
	TaTeTi();
	~TaTeTi();
	bool SetPoint(int r, int c, int id);
	int GetPoint(int x, int y);
	bool CheckVictory();
	bool CheckTie();

private:

	//VARIABLES
	int grid[ROWS][COLS];
};