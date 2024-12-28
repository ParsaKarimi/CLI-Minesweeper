#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

#define BOMB_CHANCE .10
#define UNVISITED 0x25A1

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
    setlocale(LC_CTYPE, "");

    D1 = atoi(argv[1]);
    D2 = atoi(argv[2]);
    if (D1 < 10 || D2 < 10) {
        return 1;
    }

    generate_table();
    print_table();
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

char choose_table_letter(BYTE value) {
    switch (value) {
        case 0b01000000:
            return 'E';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 0x80:
            return 'F';
        default:
            return ' ';
    }
}

void print_table() {
    // TODO: print table and number of bombs remaind

    // system("cls");

    printf("┌");
    for (BYTE i = 0; i < D2; i++) printf("───┬");
    printf("\b┐");
    for (BYTE i = 0; i < D1; i++) {
        printf("\n│");
        for (BYTE j = 0; j < D2; j++) {
            printf(" %c │", choose_table_letter(userTable[i][j]));
        }
        printf("\n├");
        for (BYTE j = 0; j < D2; j++) printf("───┼");
        printf("\b┤");
    }
    for (BYTE i = 0; i < D2; i++) printf("\b\b\b\b\b");
    printf("└");
    for (BYTE i = 0; i < D2; i++) printf("───┴");
    printf("\b┘\n");

    printf("Number of Bombs: %d\n", nBomb);
    printf("Number of Flags: %d\n", nFounedBomb);

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