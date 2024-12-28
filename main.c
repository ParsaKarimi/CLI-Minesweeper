#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

BYTE D1, D2;
BYTE **table;
BYTE **user_table;

struct input {
    BYTE x;
    BYTE y;
    BYTE action;
};


int main(int argc, char const *argv[]) {

    return 0;
}

void generate_table() {
    // TODO: generates bombs and cells of table
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