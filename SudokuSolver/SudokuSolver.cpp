#include "SudokuSolver.h"

void SudokuSolver::solve() {
    int steps = 0;
    //initialize poss
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            poss[i][j] = 0b111111111;
        }
    }

    inputGrid();
    bitset<9> lastPoss[9][9];
    check:
        //printGrid();
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                lastPoss[i][j] = poss[i][j];
            }
        }

        //check isolated possibilities
        for (int i = 0; i < 9; i++)
            checkSolos(true, i);
        for (int i = 0; i < 9; i++)
            checkSolos(false, i);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                checkSolosSect(i, j);
            }
        }

        //check for lines & single quadrant line nums

        //check unique number possibilities
        for (int k = 1; k <= 9; k++) {
            for (int i = 0; i < 9; i++)
                //checkNum(true, i, k);
                checkUniqueInLine(true, i, k);
            for (int i = 0; i < 9; i++)
                //checkNum(false, i, k);
                checkUniqueInLine(false, i, k);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    //checkNumSect(i, j, k);
                    checkLinedUp(i, j, k);
                }
            }
        }

        //check if anything has changed
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (lastPoss[i][j] != poss[i][j]) {
                    steps++;
                    goto check;
                }
            }
        }

        if (!isSolved()) {
            //we're stuck, check for tuples
            for (int k = 1; k <= 9; k++) {
                for (int i = 0; i < 9; i++)
                    findTuples(true, i, k);
                for (int i = 0; i < 9; i++)
                    findTuples(false, i, k);
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        findTuplesSect(i, j, k);
                    }
                }
            }

            //check if anything has changed now
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (lastPoss[i][j] != poss[i][j]) {
                        steps++;
                        goto check;
                    }
                }
            }

            //things haven't changed, start guessing

        }

    printGrid();
    cout << "Solved in " << steps << " steps.\n";
}

bool SudokuSolver::isSolved() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == -1)
                return false;
        }
    }
    return true;
}

void SudokuSolver::inputGrid() {
    string rowIns;
    ifstream input;
    input.open("Input.txt");
    getline(input, rowIns);

    /*cout << "Input sudoku grid:\n";
    cin >> rowIns;*/
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int charAt = (i * 9) + j;
            if (rowIns[charAt] == 'x') {
                grid[i][j] = -1;
            } else {
                int toIns = rowIns[charAt] - '0';
                //cout << rowIns[charAt];
                writeNum(toIns, i, j);
            }
        }
    }
}

void SudokuSolver::printGrid() {
    for (int i = 0; i < 9; i++) {
        if (i > 0 && i % 3 == 0)
            cout << "---------------------\n";
        for (int j = 0; j < 9; j++) {
            if (j > 0 && j % 3 == 0)
                cout << "| ";
            if (grid[i][j] == -1)
                cout << "  ";
            else cout << (int)grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void SudokuSolver::writeNum(int num, int row, int col) {
    grid[row][col] = num;
    poss[row][col] = 0b0;

    for (int i = 0; i < 9; i++)
        poss[row][i][num - 1] = 0;
    for (int i = 0; i < 9; i++)
        poss[i][col][num - 1] = 0;
    
    int8_t secY = row / 3;
    int8_t secX = col / 3;

    for (int i = secX * 3; i < secX * 3 + 3; i++) {
        for (int j = secY * 3; j < secY * 3 + 3; j++) {
            poss[j][i][num - 1] = 0;
        }
    }
}

//see if number is only possible number for the square
bool SudokuSolver::checkSolos(bool col, int loc) {
    if (col) {
        for (int i = 0; i < 9; i++) {
            bool alone = true;
            int candidate = -1;
            for (int j = 0; j < 9; j++) {
                if (poss[i][loc][j]) {
                    if (alone) {
                        alone = false;
                        candidate = j + 1;
                    } else {
                        candidate = -1;
                        break;
                    }
                }
            }
            if (candidate > 0) {
                writeNum(candidate, i, loc);
                return true;
            }
        }
    } else {
        for (int i = 0; i < 9; i++) {
            bool alone = true;
            int candidate = -1;
            for (int j = 0; j < 9; j++) {
                if (poss[loc][i][j]) {
                    if (alone) {
                        alone = false;
                        candidate = j + 1;
                    } else {
                        candidate = -1;
                        break;
                    }
                }
            }
            if (candidate > 0) {
                writeNum(candidate, loc, i);
                return true;
            }
        }
    }
    return false;
}

//checksolos for a sector
bool SudokuSolver::checkSolosSect(int x, int y) {
    int8_t secY = y * 3;
    int8_t secX = x * 3;

    for (int i = secX; i < secX + 3; i++) {
        for (int j = secY; j < secY + 3; j++) {
            bool alone = true;
            int candidate = -1;
            for (int k = 0; k < 9; k++) {
                if (poss[i][j][k]) {
                    if (alone) {
                        alone = false;
                        candidate = k + 1;
                    } else {
                        candidate = -1;
                        break;
                    }
                }
            }
            if (candidate > 0) {
                writeNum(candidate, i, j);
                return true;
            }
        }
    }
    return false;
}

//check if number is only possible at one place in a line
bool SudokuSolver::checkNum(bool col, int loc, int num) {
    bool alone = true;
    uint8_t tarX, tarY;
    if (col) {
        for (int i = 0; i < 9; i++) {
            if (poss[i][loc][num - 1]) {
                if (alone) {
                    alone = false;
                    tarY = i;
                    tarX = loc;
                } else {
                    alone = true;
                    break;
                }
            }
        }
    } else {
        for (int i = 0; i < 9; i++) {
            if (poss[loc][i][num - 1]) {
                if (alone) {
                    alone = false;
                    tarY = loc;
                    tarX = i;
                } else {
                    alone = true;
                    break;
                }
            }
        }
    }
    if (!alone) {
        writeNum(num, tarY, tarX);
        return true;
    } else return false;
}

//checknum for a sector
bool SudokuSolver::checkNumSect(int x, int y, int num) {
    uint8_t secY = y * 3;
    uint8_t secX = x * 3;
    uint8_t tarX, tarY;

    bool alone = true;
    for (int i = secX; i < secX + 3; i++) {
        for (int j = secY; j < secY + 3; j++) {
            if (poss[i][j][num - 1]) {
                if (alone) {
                    alone = false;
                    tarX = j;
                    tarY = i;
                } else {
                    alone = true;
                    goto duplicates;
                }
            }
        }
    }
duplicates:
    if (!alone) {
        writeNum(num, tarY, tarX);
        return true;
    } else return false;
}

//check if number is only possible in one sector along a line
//if so, the number MUST be on that line and is impossible for the rest of sector
bool SudokuSolver::checkUniqueInLine(bool col, int loc, int num) {
    //use of occurrences replaces checkNum()
    int occurences = 0;
    if (col) {
        int secY = -1;
        for (int i = 0; i < 9; i++) {
            if (poss[i][loc][num - 1]) {
                occurences++;
                if (secY == -1) {
                    secY = i;
                } else if (i / 3 != secY / 3) {
                    return false;
                }
            }
        }
        if (secY > -1) {
            if (occurences == 1) {
                writeNum(num, secY, loc);
            } else {
                secY /= 3;
                int secX = loc / 3;
                for (int i = secX * 3; i < (secX * 3) + 3; i++) {
                    if (i == loc)
                        continue;
                    for (int j = secY * 3; j < (secY * 3) + 3; j++) {
                        poss[j][i][num - 1] = 0;
                    }
                }
            }
            return true;
        }
    } else {
        int secX = -1;
        for (int i = 0; i < 9; i++) {
            if (poss[loc][i][num - 1]) {
                occurences++;
                if (secX == -1) {
                    secX = i;
                } else if (i / 3 != secX / 3) {
                    return false;
                }
            }
        }
        if (secX > -1) {
            if (occurences == 1) {
                writeNum(num, loc, secX);
            } else {
                secX /= 3;
                int secY = loc / 3;
                for (int i = secY * 3; i < (secY * 3) + 3; i++) {
                    if (i == loc)
                        continue;
                    for (int j = secX * 3; j < (secX * 3) + 3; j++) {
                        poss[i][j][num - 1] = 0;
                    }
                }
            }
            return true;
        }
    }
    return false;
}

//if possibilities in a sector are lined up, treat it like a directional number
bool SudokuSolver::checkLinedUp(int x, int y, int num) {
    bool horiz = true;
    bool vert = true;
    int hY, vX;
    hY = -1;

    //TODO: Optimize for if we already know it's vertical/horizontal we don't have to check every vertical/horizontal
    for (int i = x * 3; i < (x * 3) + 3; i++) {
        for (int j = y * 3; j < (y * 3) + 3; j++) {
            if (poss[j][i][num - 1]) {
                if (hY == -1) {
                    hY = j;
                    vX = i;
                } else {
                    if (j != hY)
                        horiz = false;
                    if (i != vX)
                        vert = false;
                }
            }
        }
    }

    if (hY != -1) {
        if (horiz && vert) {
            //this should replace checkNumSect()
            writeNum(num, hY, vX);
        } else if (horiz) {
            for (int i = 0; i < 9; i++) {
                if (i / 3 != x)
                    poss[hY][i][num - 1] = 0;
            }
        } else if (vert) {
            for (int i = 0; i < 9; i++) {
                if (i / 3 != y)
                    poss[i][vX][num - 1] = 0;
            }
        } else return false;
        return true;
    }

    return false;
}

//find tuples- sets of x numbers that are only possible in x spaces- and remove other numbers
//this function is EXPENSIVE, use only as last resort before guess and check
bool SudokuSolver::findTuples(bool col, int loc, int num) {
    checkUniqueInLine(col, loc, num);
    bitset<9> places = getPlaces(col, loc, num);
    if (places == 0b0)
        return false;

    bitset<9> tuple = 0b0;
    tuple[num - 1] = true;

    bool successful = false;
    for (int i = places.count(); i < 8 && !successful; i++) {
        successful = tuplesRec(col, loc, places, tuple, i, i - 1, 1);
    }

    return successful;
}

//recursive tuples search (dfs)
//col, loc are coordinates of the row/col to use
//area is the current cells making up the proposed tuple
//tuple is the digits in the proposed tuple
//tupleSize is the size limit of the proposed tuple
//depth is current recursive depth
//begin indicates where to start the tuple search, so we don't repeat lower numbers
bool SudokuSolver::tuplesRec(bool col, int loc, bitset<9> area, bitset<9> tuple, int tupleSize, int depth, int begin) {
    for (int i = 0; i < 9; i++) {
        if (area[i]) {
            for (int j = begin - 1; j < 9; j++) {
                if (col) {
                    if (!tuple[j] && poss[i][loc][j]) {
                        bitset<9> places = getPlaces(col, loc, j + 1);
                        bitset<9> intersect = places | area;

                        if (intersect.count() <= tupleSize) {
                            bitset<9> tryTuple = tuple;
                            tryTuple[j] = true;

                            if (depth == 1) {
                                //exit condition- apply tuple and return successful
                                for (int k = 0; k < 9; k++) {
                                    if (intersect[k]) {
                                        poss[k][loc] &= tryTuple;
                                    }
                                }
                                return true;
                            } else {
                                bool out = tuplesRec(col, loc, intersect, tryTuple, tupleSize, depth - 1, j + 1);
                                if (out)
                                    return true;
                            }
                        }
                    }
                } else {
                    if (!tuple[j] && poss[loc][i][j]) {
                        bitset<9> places = getPlaces(col, loc, j + 1);
                        bitset<9> intersect = places | area;

                        if (intersect.count() <= tupleSize) {
                            bitset<9> tryTuple = tuple;
                            tryTuple[j] = true;

                            if (depth == 1) {
                                //exit condition- apply tuple and return successful
                                for (int k = 0; k < 9; k++) {
                                    if (intersect[k]) {
                                        poss[loc][k] &= tryTuple;
                                    }
                                }
                                return true;
                            } else {
                                bool out = tuplesRec(col, loc, intersect, tryTuple, tupleSize, depth - 1, j + 1);
                                if (out)
                                    return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

//findtuples in a sector
bool SudokuSolver::findTuplesSect(int x, int y, int num) {
    checkLinedUp(x, y, num);
    bitset<9> places = getPlacesSect(x, y, num);
    if (places == 0b0)
        return false;

    bitset<9> tuple = 0b0;
    tuple[num - 1] = true;

    bool successful = false;
    for (int i = places.count(); i < 8 && !successful; i++) {
        successful = tuplesSect(x, y, places, tuple, i, i - 1, 1);
    }

    return successful;
}

//recursive tuples search (dfs)
//x, y are coordinates of sector (0-2)
//area is the current cells making up the proposed tuple
//tuple is the digits in the proposed tuple
//tupleSize is the size limit of the proposed tuple
//depth is current recursive depth
//begin indicates where to start the tuple search, so we don't repeat lower numbers
bool SudokuSolver::tuplesSect(int x, int y, bitset<9> area, bitset<9> tuple, int tupleSize, int depth, int begin) {
    for (int i = 0; i < 9; i++) {
        if (area[i]) {
            for (int j = begin - 1; j < 9; j++) {
                if (!tuple[j] && poss[(i % 3) + (y * 3)][(i / 3) + (x * 3)][j]) {
                    bitset<9> places = getPlacesSect(x, y, j + 1);
                    bitset<9> intersect = places | area;

                    if (intersect.count() <= tupleSize) {
                        bitset<9> tryTuple = tuple;
                        tryTuple[j] = true;

                        if (depth == 1) {
                            //exit condition- apply tuple and return successful
                            for (int k = 0; k < 9; k++) {
                                if (intersect[k]) {
                                    poss[(k % 3) + (y * 3)][(k / 3) + (x * 3)] &= tryTuple;
                                }
                            }
                            return true;
                        } else {
                            bool out = tuplesSect(x, y, intersect, tryTuple, tupleSize, depth - 1, j + 1);
                            if (out)
                                return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

//returns a bitset representation of where a number is possible in a line
bitset<9> SudokuSolver::getPlaces(bool col, int loc, int num) {
    bitset<9> places = 0b0;
    for (int i = 0; i < 9; i++) {
        if (col) {
            if (poss[i][loc][num - 1])
                places[i] = true;
        } else {
            if (poss[loc][i][num - 1])
                places[i] = true;
        }
    }
    return places;
}

//getplaces for sector
bitset<9> SudokuSolver::getPlacesSect(int x, int y, int num) {
    bitset<9> places = 0b0;
    for (int i = 0; i < 9; i++) {
        if (poss[(i % 3) + (y * 3)][(i / 3) + (x * 3)][num - 1]) {
            places[i] = true;
        }
    }
    return places;
}