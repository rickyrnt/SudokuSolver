#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <vector>
#include <tuple>

using namespace std;

class SudokuSolver {

private:
	vector<vector<int8_t>> grid;
	vector<vector<bitset<9>>> poss;
	vector<vector<vector<int8_t>>> pastGrid;
	vector<vector<vector<bitset<9>>>> pastPoss;
	vector<tuple<int, int, int>> guesses;

	void inputGrid();
	void printGrid();
	bool isSolved();
	void writeNum(int num, int row, int col);
	bool checkSolos(bool col, int loc);
	bool checkSolosSect(int x, int y);
	bool checkNum(bool col, int loc, int num);
	bool checkNumSect(int x, int y, int num);
	bool checkUniqueInLine(bool col, int loc, int num);
	bool checkLinedUp(int x, int y, int num);
	bool findTuples(bool col, int loc, int num);
	bool tuplesRec(bool col, int loc, bitset<9> area, bitset<9> tuple, int tupleSize, int depth, int begin);
	bool findTuplesSect(int x, int y, int num);
	bool tuplesSect(int x, int y, bitset<9> area, bitset<9> tuple, int tupleSize, int depth, int begin);
	bitset<9> getPlaces(bool col, int loc, int num);
	bitset<9> getPlacesSect(int x, int y, int num);

public:
	void solve();
};