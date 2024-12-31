#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>

#define BOMB_CHANCE .12

typedef uint8_t BYTE;

BYTE D1, D2;
BYTE **table;
BYTE **userTable;
int nBomb;
int nFounedBomb;
bool playing = true;

struct {
    BYTE x;
    BYTE y;
    BYTE action;
} input;

struct stack {
    struct stack *prev;
    BYTE x, y, checked;
};

void generate_table();
void print_table();
void translate_action(char action, char *cell);
bool is_bomb();
void handle_click();
void handle_flag();
bool is_game_finished();
void free_table();

int main(int argc, char const *argv[]) {
    srand(time(NULL));
    setlocale(LC_CTYPE, ".UTF8");

    D1 = atoi(argv[2]);
    D2 = atoi(argv[1]);
    if (D1 < 10 || D2 < 10) {
        return 1;
    }

    generate_table();
    while (playing) {

        for (BYTE i = 0; i < 255; i++) printf("\n");

        print_table();
        char userAction[2];
        char cell[6];
        
        scanf(" %1s", &userAction, 1);
        if (toupper(userAction[0]) == 'E') break;
        // if (toupper(userAction[0]) == 'T') {
        //     playing = false;
        //     print_table();
        //     playing = true;
        //     continue;
        // }

        scanf(" %2s", &cell[0], 2);
        scanf(" %2s", &cell[3], 2);
        
        translate_action(userAction[0], cell);

        if (input.action == 0b00) continue;
        if (input.action == 0b10 && is_bomb()) {
            printf("Sorry, you lost the game\n");
            playing = false;
            print_table();
            break;
        }
        if (input.action == 0b11) handle_flag();
        else handle_click();

        if (is_game_finished()) {
            printf("Congrats, you are alive, but not for long!\n");
            break;
        }

    }
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

    // Count number of bombs on center
    for (BYTE i = 1; i < D1-1; i++) {
        for (BYTE j = 1; j < D2-1; j++) {

            if (table[i][j] != 0x80) continue;
            
            nBomb++;
            table[i-1][j-1] += table[i-1][j-1] != 0x80;
            table[i-1][j]   += table[i-1][j]   != 0x80;
            table[i-1][j+1] += table[i-1][j+1] != 0x80;
            table[i][j-1]   += table[i][j-1]   != 0x80;
            table[i][j+1]   += table[i][j+1]   != 0x80;
            table[i+1][j-1] += table[i+1][j-1] != 0x80;
            table[i+1][j]   += table[i+1][j]   != 0x80;
            table[i+1][j+1] += table[i+1][j+1] != 0x80;
            
        }
    }

    // Count number of bombs on the top and bottom borders
    for (int i = 1; i < D2 - 1; i++) {
        if (table[0][i] == 0x80) {
            nBomb++;
            table[0][i-1] += table[0][i-1] != 0x80;
            table[0][i+1] += table[0][i+1] != 0x80;
            table[1][i-1] += table[1][i-1] != 0x80;
            table[1][i]   += table[1][i]   != 0x80;
            table[1][i+1] += table[1][i+1] != 0x80;
        }
        if (table[D1-1][i] == 0x80) {
            nBomb++;
            table[D1-2][i-1] += table[D1-2][i-1] != 0x80;
            table[D1-2][i]   += table[D1-2][i]   != 0x80;
            table[D1-2][i+1] += table[D1-2][i+1] != 0x80;
            table[D1-1][i-1] += table[D1-1][i-1] != 0x80;
            table[D1-1][i+1] += table[D1-1][i+1] != 0x80;
        }
    }
    // Count number of bombs on the left and right borders
    for (int i = 1; i < D1 - 1; i++) {
        if (table[i][0] == 0x80) {
            nBomb++;
            table[i-1][0] += table[i-1][0] != 0x80;
            table[i-1][1] += table[i-1][1] != 0x80;
            table[i][1]   += table[i][1]   != 0x80;
            table[i+1][0] += table[i+1][0] != 0x80;
            table[i+1][1] += table[i+1][1] != 0x80;
        }
        if (table[i][D2-1] == 0x80) {
            nBomb++;
            table[i-1][D2-1] += table[i-1][D2-1] != 0x80;
            table[i-1][D2-2] += table[i-1][D2-2] != 0x80;
            table[i][D2-2]   += table[i][D2-2]   != 0x80;
            table[i+1][D2-1] += table[i+1][D2-1] != 0x80;
            table[i+1][D2-2] += table[i+1][D2-2] != 0x80;
        }
    }

    // Count number of bombs on the corners
    if (table[0][0] == 0x80) {
        nBomb++;
        table[0][1] += table[0][1] != 0x80;
        table[1][1] += table[1][1] != 0x80;
        table[1][0] += table[1][0] != 0x80;
    }
    else if (table[0][D2-1] == 0x80) {
        nBomb++;
        table[0][D2-2] += table[0][D2-2] != 0x80;
        table[1][D2-2] += table[1][D2-2] != 0x80;
        table[1][D2-1] += table[1][D2-1] != 0x80;
    }
    else if (table[D1-1][0] == 0x80) {
        nBomb++;
        table[D1-2][0] += table[D1-2][0] != 0x80;
        table[D1-2][1] += table[D1-2][1] != 0x80;
        table[D1-1][1] += table[D1-1][1] != 0x80;
    }
    else if (table[D1-1][D2-1] == 0x80) {
        nBomb++;
        table[D1-2][D2-2] += table[D1-2][D2-2] != 0x80;
        table[D1-2][D2-2] += table[D1-2][D2-2] != 0x80;
        table[D1-1][D2-1] += table[D1-1][D2-1] != 0x80;
    }

}

char choose_user_table_letter(BYTE value) {
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
char choose_table_letter(BYTE value) {
    value = choose_user_table_letter(value);
    if (value == 'F') return 'B';
    return value;
}

void print_table() {

    printf("┌");
    for (BYTE i = 0; i < D2; i++) printf("───┬");
    printf("\b┐");
    for (BYTE i = 0; i < D1; i++) {
        printf("\n│");
        for (BYTE j = 0; j < D2; j++) {
            if (playing) {
                printf(" %c │", choose_user_table_letter(userTable[i][j]));
            } else {
                printf(" %c │", choose_table_letter(table[i][j]));
            }
        }
        printf("\n├");
        for (BYTE j = 0; j < D2; j++) printf("───┼");
        printf("\b┤");
    }
    for (BYTE i = 0; i < D2; i++) printf("\b\b\b\b\b");
    printf("└");
    for (BYTE i = 0; i < D2; i++) printf("───┴");
    printf("\b┘\n");

    if (playing) {
        printf("Number of Bombs: %d\n", nBomb);
        printf("Number of Flags: %d\n", nFounedBomb);
    }

}

void translate_action(char action, char *cell) {

    input.action = 0;

    for (BYTE i = 0; i < 6; i++) 
        if ((cell[i] < '0' || cell[i] > '9') && (tolower(cell[i]) < 'a' || tolower(cell[i]) > 'f') && cell[i] != 0) return;

    input.x = strtol(&cell[0], NULL, 16);
    input.y = strtol(&cell[3], NULL, 16);

    if (input.x >= D2 || input.y >= D1) {
        input.action = 0;
        return;
    }
    

    switch (toupper(action)) {
        case 'C':
            input.action = 0b10;
            break;
        case 'F':
            input.action = 0b11;
            break;
        default:
            input.action = 0b00;
            break;
    }

}

bool is_bomb() {
    
    if (userTable[input.y][input.x] == 0x80) return false;
    if (table[input.y][input.x] == 0x80) return true;
    return false;

}

bool visit_cell(struct stack **visited, BYTE x, BYTE y) {
    if (userTable[y][x] == 0b01000000) return false;

    if (userTable[y][x] == 0x80) {
        nFounedBomb--;
    }
    userTable[y][x] = table[y][x];

    if (table[y][x] == 0) {
        userTable[y][x] = 0b01000000;
        struct stack *new = malloc(sizeof(struct stack));
        new->x = x;
        new->y = y;
        new->prev = *visited;
        new->checked = 0;
        *visited = new;
        return true;
    }
    
    return false;
}

void handle_click() {
    
    if (userTable[input.y][input.x] != 0) return;
    if (table[input.y][input.x] != 0) {
        userTable[input.y][input.x] = table[input.y][input.x];
        return;
    }

    userTable[input.y][input.x] = 0b01000000;
    
    struct stack *visited = malloc(sizeof(struct stack));
    visited->x = input.x;
    visited->y = input.y;
    visited->checked = 0;
    visited->prev = NULL;

    while (visited != NULL) {
        if (visited->y == D1-1) visited->checked = visited->checked | 0b1000;
        if (visited->x == 0) visited->checked = visited->checked | 0b0100;
        if (visited->y == 0) visited->checked = visited->checked | 0b0010;
        if (visited->x == D2-1) visited->checked = visited->checked | 0b0001;

        if (!(visited->checked & 0b1000)) {
            
            visited->checked = visited->checked | 0b1000;
            if (visit_cell(&visited, visited->x, visited->y+1)) {
                visited->checked = 0b0010;
                continue;
            }
            
        }
        if (!(visited->checked & 0b0100)) {

            visited->checked = visited->checked | 0b0100;
            if (visit_cell(&visited, visited->x-1, visited->y)) {
                visited->checked = 0b0001;
                continue;
            }
            
        }
        if (!(visited->checked & 0b0010)) {

            visited->checked = visited->checked | 0b0010;
            if (visit_cell(&visited, visited->x, visited->y-1)) {
                visited->checked = 0b1000;
                continue;
            }
            
        }
        if (!(visited->checked & 0b0001)) {

            visited->checked = visited->checked | 0b0001;
            if (visit_cell(&visited, visited->x+1, visited->y)) {
                visited->checked = 0b0100;
                continue;
            }
            
        }

        struct stack *temp = visited;
        visited = visited->prev;
        free(temp);
        
    }
    
}

void handle_flag() {
    
    if (userTable[input.y][input.x] != 0x80 && userTable[input.y][input.x] != 0) return;
    userTable[input.y][input.x] = userTable[input.y][input.x] == 0x80 ? 0:0x80;
    nFounedBomb += userTable[input.y][input.x] == 0x80 ? 1:-1;

}

bool is_game_finished() {

    if (nBomb != nFounedBomb) return false;
    
    for (BYTE i = 0; i < D1; i++) {
        for (BYTE j = 0; j < D2; j++) {
            if (userTable[i][j] == 0) return false;
            if (userTable[i][j] == 0x80 && userTable[i][j] != table[i][j]) return false;
        }
    }

    return true;

}

void free_table() {

    for (BYTE i = 0; i < D1; i++) {
        free(table[i]);
        free(userTable[i]);
    }
    free(table);
    free(userTable);

}