#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t BYTE;

BYTE D1, D2;
BYTE **table;
BYTE **user_table;

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

int main(int argc, char const *argv[]) {
    D1 = atoi(argv[1]);
    D2 = atoi(argv[2]);
    
    generate_table();
    return 0;
}

void generate_table() {
    // TODO: generates bombs and cells of table
    
    table = calloc(D1, sizeof(BYTE*));
    for (BYTE i = 0; i < D1; i++) {
        table[i] = calloc(D2, sizeof(BYTE));
    }
    user_table = calloc(D1, sizeof(BYTE*));
    for (BYTE i = 0; i < D1; i++) {
        user_table[i] = calloc(D2, sizeof(BYTE));
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