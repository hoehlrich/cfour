#include <stdio.h>
#include <math.h>

#define NUM_ROW     6
#define NUM_COL     7
#define BYTE        8
#define FOOTER      "1 2 3 4 5 6 7" 
#define CONNECT_FOUR    4


/* 6 rows of bytes
 * last bit unused
 * [0, 1, 2, 3, 4, 5, 6, 7
 * ...
 * 40, 41, 42, 43, 44, 45, 46, 47]
 */

void printboard(long bluemask, long redmask);
int makemove(long * playermask, long fullmask, int col);
int takeinput(long * playermask, long fullmask);
int checkwin(long playermask, long move);
int checkdir(long playermask, long move, int xshift, int yshift);

int main() {
    long bluemask, redmask;
    bluemask = redmask = 0;

    int i;
    long * playermaskp;
    long move;
    for (i = 0; 1; i++) {
        printboard(bluemask, redmask);
        playermaskp = i % 2 ? &bluemask : &redmask;
        move = takeinput(playermaskp, bluemask | redmask);
        if (checkwin(*playermaskp, move)) {
            char winner;
            winner = i % 2 ? 'X' : 'O';
            printf("%c won!\n", winner);
        }
    }
}

/* takeinput: takes user input and executes move; returns move */
int takeinput(long * playermaskp, long fullmask) {
    printf("Enter the column of your move: ");
    int col;
    scanf("%d", &col);
    col--;
    if (col >= NUM_COL) {
        goto failed;
    }

    long move;
    move = makemove(playermaskp, fullmask, col);
    if (move == -1) {
        failed:
            printf("Try again\n");
            move = takeinput(playermaskp, fullmask);
    }
    return move;
}

/* makemove: makes move on playermask if possible; reutrns move if successful -1 if not */
int makemove(long * playermaskp, long fullmask, int col) {
    int i, n;
    long move;

    for (i = 0; i < NUM_ROW; i++) {
        n = (BYTE * i) + col;
        move = pow(2, n);
        if ((fullmask & move) == 0) {
            *playermaskp = *playermaskp | move;
            return move;
        }
    }
    return -1;
}

void printboard(long bluemask, long redmask) {
    int i, j;
    long space;
    for (i = NUM_ROW - 1; i >= 0; i--) {
        for (j = 0; j < NUM_COL; j++) {
            space = pow(2, (BYTE*i) + j);
            if ((bluemask & space) != 0) {
                printf("X ");
            } else if ((redmask & space) != 0) {
                printf("O ");
            } else {
                printf("- ");
            }
        }
        putchar('\n');
    }
    printf("%s\n", FOOTER);
}

int checkwin(long playermask, long move) {
    return (checkdir(playermask, move, -1, -1)) 
        | (checkdir(playermask, move, -1, 0))
        | (checkdir(playermask, move, -1, 1))
        | (checkdir(playermask, move, 0, -1))
        | (checkdir(playermask, move, 0, 1))
        | (checkdir(playermask, move, 1, -1))
        | (checkdir(playermask, move, 1, 0))
        | (checkdir(playermask, move, 1, 1));
}

int checkdir(long playermask, long move, int xshift, int yshift) {
    int i;
    for (i = 0; i < CONNECT_FOUR; i++) {
        move <<= (xshift + (NUM_COL * yshift));
        if (!(move & playermask))
            return 0;
    }
    return 1;
}
