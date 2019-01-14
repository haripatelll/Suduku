#include "sudoku.h"
#include "array.h"

const int DIM = 9;

static const int EMPTY = 0;
static const char BLANK = '-';
static const int MIN = 1;
static const int MAX = 9;

static const int SUCCESS = 0;
static const int ERROR = -1;
static const int ERASE_EMPTY_CELL = -2;
static const int ERASE_FILLED_CELL = -3;
static const int ERROR_NEXT_CELL = -4;

struct sudoku {
  int puzzle[DIM * DIM];
  int sol[DIM * DIM];
};


static bool cell_empty(const int board[], int row, int col);


int fill_cell(struct sudoku *s, int row, int col, int num) {
  int topl_row = row;
  int topl_col = col;
  int rr = row;
  int cc = col;
  int tempcol = topl_col;
  int posn = 0;
  if (cell_empty(s->sol, row, col) == false) {
    return ERROR;
  } else {
    for (int left = col - MIN; left >= 0; left--) {
      if (num == s->sol[(MAX * row) + left]) {
        return ERROR;
      }
    } for (int right = col + MIN; right < MAX; right++) {
      if (num == s->sol[(MAX * row) + right]) {
        return ERROR;
      }
    } for (int up = row - MIN; up >= 0; up--) {
      if (num == s->sol[(MAX * up) + col]) {
        return ERROR;
      }
    } for (int down = row + MIN; down < MAX; down++) {
      if (num == s->sol[(MAX * down) + col]) {
        return ERROR;
      }
    } while (1) {
      if (topl_row % 3 == 0) {
        break;
      } else {
        topl_row--;
      }
    } while (1) {
      if (topl_col % 3 == 0) {
        break;
      } else {
        topl_col--;
      }
    } while (1) {
      tempcol = topl_col;
      rr = topl_row;
      cc = topl_col;
      break;
    } while (topl_row < (rr + 3)) {
      while (topl_col < (cc + 3)) {
        if (num == s->sol[(9 * topl_row) + topl_col]) {
          return ERROR;
        } else {
          topl_col++;
        }
      }
      topl_col = tempcol;
      topl_row++;
    }
  }
  posn = (row * MAX) + col;
  s->sol[posn] = num;
  return SUCCESS;
}

// chekfill(s,row,col,num) tries to fill num in the cell (row,col) if
// possible. It returns 0 if doing so does not violate any of the row, 
// column,and box constraints. Otherwise, it returns a negative integer.
// requires: row and col are valid indices. 
//           num is an integer between 1 and 9.
// time: O(n^2)
int chekfill(const struct sudoku *s, int row, int col, int num) {
  int topl_row = row;
  int topl_col = col;
  int rr = row;
  int cc = col;
  int tempcol = topl_col;
  if (cell_empty(s->sol, row, col) == false) {
    return ERROR;
  } else {
    for (int left = col - MIN; left >= 0; left--) {
      if (num == s->sol[(MAX * row) + left]) {
        return ERROR;
      }
    } for (int right = col + MIN; right < MAX; right++) {
      if (num == s->sol[(MAX * row) + right]) {
        return ERROR;
      }
    } for (int up = row - MIN; up >= 0; up--) {
      if (num == s->sol[(MAX * up) + col]) {
        return ERROR;
      }
    } for (int down = row + MIN; down < MAX; down++) {
      if (num == s->sol[(MAX * down) + col]) {
        return ERROR;
      }
    } while (1) {
      if (topl_row % 3 != 0) {
        topl_row--;
      } else {
        break;
      }
    } while (1) {
      if (topl_col % 3 != 0) {
        topl_col--;
      } else {
        break;
      }
    } while (1) {
      tempcol = topl_col;
      rr = topl_row;
      cc = topl_col;
      break;
    } while (topl_row < (rr + 3)) {
      while (topl_col < (cc + 3)) {
        if (num == s->sol[(MAX * topl_row) + topl_col]) {
          return ERROR;
        } else {
          topl_col++;
        }
      }
      topl_col = tempcol;
      topl_row++;
    }
  }
  return SUCCESS;
}


void choices_cell(const struct sudoku *s, int row, int col, 
                  int choices[], int *num_choices) {
  int counter = 0;
  int values[9] = {0};
  int arcount = 0;
  for (int index = MIN; index <= MAX; index++) {
    if (chekfill(s, row, col, index) == SUCCESS) {
      counter++;
      values[index - 1] = 1;
    }
  } for (int i = 0; i < MAX; i++) {
    if (values[i] >= MIN) {
      choices[arcount] = i + 1;
      arcount++;
    }
  } 
  *num_choices = counter;
}


bool solved_puzzle(const struct sudoku *s) {
  int dupcounter = 0;
  int rowc = 3;
  int colc = 3;
  for (int rowf = 0; rowf < MAX; rowf++) {
    for (int colf = 0; colf < MAX; colf++) { 
      if (rowf == 9 || colf == MAX) {
        break;
      } else if (cell_empty(s->sol, rowf, colf) == true) {
        return false;
      }
    }  
  } for (int row = 0; row < MAX; row++) {
    for (int counter = MIN; counter < 10; counter++) {
      for (int col = 0; col < MAX; col++) {
        if (dupcounter >= 2) {
          return false;
        } else if (counter == next_cell(s, &row, &col)) {
          dupcounter += 1;
        }
      }
      dupcounter = 0;
    }
    dupcounter = 0;
  } for (int col = 0; col < MAX; col++) {
    for (int counter = MIN; counter < 10; counter++) {
      for (int row = 0; row < MAX; row++) {
        if (dupcounter >= 2) {
          return false;
        } else if (counter == next_cell(s, &row, &col)) {
          dupcounter += 1;
        }
      }
      dupcounter = 0;
    }
    dupcounter = 0;
  } for (int strow = 0; (strow < 7); (strow = strow + 3)) {
    for (int stcol = 0; (stcol < 7); (stcol = stcol + 3)) {
      for (int counter = MIN; counter < 10; counter++) {
        for (int row = strow ; row < rowc; row++) {
          for (int col = stcol; col < colc; col++) {
            if (dupcounter >= 2) {
              return false;
            } else if (counter == next_cell(s, &row, &col)) {
              dupcounter += 1;
            }
          }
          dupcounter = 0;
        }        
      }
      colc = colc + 3;
    }
    rowc = rowc + 3;
  }
  return true;
}


// find_empty_cell(s, row, col) returns true
// if cell (row,col) is empty on board.
// requires: s is a valid sudoku puzzle.
// effects: modifies row and col for solve
// time: O(n^2)
int find_empty_cell(struct sudoku *s, int *row, int *col) {
  for (*row = 0; *row < MAX; (*row)++) {
    for (*col = 0; *col < MAX; (*col)++) {
      if (s->sol[(MAX * *row) + *col] == 0) {
        return 1;
      }
    }
  }
  return 0;
}


bool solve(struct sudoku *s) {
  int row = 0;
  int col = 0;
  if (find_empty_cell(s, &row, &col) == false) {
    return 1;
  } for (int num = MIN; num <= MAX; num++) {
    if (chekfill(s, row, col, num) == SUCCESS) {
      s->sol[row * MAX + col] = num;
      if (solve(s)) {
        return 1;
      } else {
        s->sol[(row * MAX) + col] = 0;
      }
    }
  }
  return 0;
}


// There is no need to modify the rest of the functions.

struct sudoku *read_sudoku(void) {
  struct sudoku *s = malloc(sizeof(struct sudoku));
  char c = 0;
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      scanf(" %c", &c);
      if (c == BLANK) {
        s->puzzle[row * DIM + col] = 0;
      } else {
        s->puzzle[row * DIM + col] = c - '0';
      }
    }
  }

  // copy puzzle to solution
  reset_sol(s);

  return s;
}

void sudoku_destroy(struct sudoku *s) {
  assert(s);
  free(s);
}

void print_sol(const struct sudoku *s) {
  assert(s);

  printf("\n");
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      int num = s->sol[row * DIM + col];
      if (num == EMPTY) {
        printf("%c", BLANK);
      } else {
        printf("%d", num);
      }  
    }
    printf("\n");
  }
  printf("\n");
}

void reset_sol(struct sudoku *s) {
  assert(s);

  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      s->sol[row * DIM + col] = s->puzzle[row * DIM + col];
    }
  }  
}

// cell_empty(board, row, col) returns true
// if cell (row,col) is empty on board.
// requires: board is a valid sudoku puzzle.
static bool cell_empty(const int board[], int row, int col) {
  assert(board);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  return board[row * DIM + col] == EMPTY;
}

int erase_cell(struct sudoku *s, int row, int col) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (cell_empty(s->sol, row, col)) {
    return ERASE_EMPTY_CELL;
  }
  if (!cell_empty(s->puzzle, row, col)) {
    return ERASE_FILLED_CELL;
  }
  s->sol[row * DIM + col] = EMPTY;
  return SUCCESS;
}


int next_cell(const struct sudoku *s, int *row, int *col) {
  assert(s);
  assert(row);
  assert(col);

  int choices[DIM];
  int num_choices = 0;
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j) {
      if (!cell_empty(s->sol, i, j)) continue;
      choices_cell(s, i, j, choices, &num_choices);
      if (num_choices == 1) {
        *row = i;
        *col = j;
        return SUCCESS;
      }
    }
  }
  return ERROR_NEXT_CELL;
}
