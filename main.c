#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define BOMB_CHANCE .15

typedef uint8_t BYTE;

BYTE D1, D2;
BYTE **table;
BYTE **userTable;
int nBomb;
int nFounedBomb;

struct input {
    BYTE x;
    BYTE y;
    BYTE action;
};

void generate_table();
void print_table();
struct input translate_action(char action, char cell);
bool is_bomb(BYTE x, BYTE y);
void update_table(struct input input);
void free_table();

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    D1 = atoi(argv[1]);
    D2 = atoi(argv[2]);
    if (D1 < 10 || D2 < 10) {
        return 1;
    }

    generate_table();
    free_table();

    return 0;
}

void generate_table() {
    
    table = calloc(D1, sizeof(BYTE*));
    for (BYTE i = 0; i < D1; i++) {
        table[i] = calloc(D2, sizeof(BYTE));
    }
    userTable = calloc(D1, sizeof(BYTE*));
    for (BYTE i = 0; i < D1; i++) {
        userTable[i] = calloc(D2, sizeof(BYTE));
    }

    for (BYTE i = 0; i < D1; i++)
        for (BYTE j = 0; j < D2; j++)
            table[i][j] = ((double)rand() / (double)RAND_MAX) > (1. - BOMB_CHANCE) ? 0b10000000:0b00000000;

    for (BYTE i = 0; i < D1; i++) {
        for (BYTE j = 0; j < D2; j++) {
            if (table[i][j] == 0x80) {
                nBomb++;

                if (i > 0 && j > 0 && i < D1-1 && j < D2-1) {
                    table[i-1][j-1] += table[i-1][j-1] != 0x80;
                    table[i-1][j]   += table[i-1][j]   != 0x80;
                    table[i-1][j+1] += table[i-1][j+1] != 0x80;
                    table[i][j-1]   += table[i][j-1]   != 0x80;
                    table[i][j+1]   += table[i][j+1]   != 0x80;
                    table[i+1][j-1] += table[i+1][j-1] != 0x80;
                    table[i+1][j]   += table[i+1][j]   != 0x80;
                    table[i+1][j+1] += table[i+1][j+1] != 0x80;
                }
                else if (i == 0 && j > 0 && j < D2-1) {
                    table[i][j-1]   += table[i][j-1]   != 0x80;
                    table[i][j+1]   += table[i][j+1]   != 0x80;
                    table[i+1][j-1] += table[i+1][j-1] != 0x80;
                    table[i+1][j]   += table[i+1][j]   != 0x80;
                    table[i+1][j+1] += table[i+1][j+1] != 0x80;
                }
                else if (i == D1-1 && j > 0 && j < D2-1) {
                    table[i-1][j-1] += table[i-1][j-1] != 0x80;
                    table[i-1][j]   += table[i-1][j]   != 0x80;
                    table[i-1][j+1] += table[i-1][j+1] != 0x80;
                    table[i][j-1]   += table[i][j-1]   != 0x80;
                    table[i][j+1]   += table[i][j+1]   != 0x80;
                }
                else if (j == 0 && i > 0 && i < D1-1) {
                    table[i-1][j]   += table[i-1][j]   != 0x80;
                    table[i-1][j+1] += table[i-1][j+1] != 0x80;
                    table[i][j+1]   += table[i][j+1]   != 0x80;
                    table[i+1][j]   += table[i+1][j]   != 0x80;
                    table[i+1][j+1] += table[i+1][j+1] != 0x80;
                }
                else if (j == D2-1 && i > 0 && i < D1-1) {
                    table[i-1][j-1] += table[i-1][j-1] != 0x80;
                    table[i-1][j]   += table[i-1][j]   != 0x80;
                    table[i][j-1]   += table[i][j-1]   != 0x80;
                    table[i+1][j-1] += table[i+1][j-1] != 0x80;
                    table[i+1][j]   += table[i+1][j]   != 0x80;
                }
                else if (table[0][0] == 0x80) {
                    table[0][1] += table[0][1] != 0x80;
                    table[1][1] += table[1][1] != 0x80;
                    table[1][0] += table[1][0] != 0x80;
                }
                else if (table[0][D2-1] == 0x80) {
                    table[0][D2-2] += table[0][D2-2] != 0x80;
                    table[1][D2-2] += table[1][D2-2] != 0x80;
                    table[1][D2-1] += table[1][D2-1] != 0x80;
                }
                else if (table[D1-1][0] == 0x80) {
                    table[D1-2][0] += table[D1-2][0] != 0x80;
                    table[D1-2][1] += table[D1-2][1] != 0x80;
                    table[D1-1][1] += table[D1-1][1] != 0x80;
                }
                else if (table[D1-1][D2-1] == 0x80) {
                    table[D1-2][D2-2] += table[D1-2][D2-2] != 0x80;
                    table[D1-2][D2-2] += table[D1-2][D2-2] != 0x80;
                    table[D1-1][D2-1] += table[D1-1][D2-1] != 0x80;
                }
                
            }
            
        }
    }

}

void print_table() {
    // TODO: print table and number of bombs remaind
}

struct input translate_action(char action, char cell) {
    // TODO: convert text values into usable structure
}

bool is_bomb(BYTE x, BYTE y) {
    // TODO: check if user clicked on a bomb
}

void update_table(struct input input) {
    // TODO: update table values based on user input
}

void free_table() {
    // TODO: clear tables from memory

    for (BYTE i = 0; i < D1; i++) {
        free(table[i]);
        free(userTable[i]);
    }
    free(table);
    free(userTable);

}