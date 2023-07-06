#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "cfour.h"

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

int makemove(long * playermask, long fullmask, int col);
int checkdir(long playermask, long move, int xshift, int yshift);

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
    if (checkdir(playermask, move, -1, 0)) 
        return 1;
    else if (checkdir(playermask, move, -1, 1))
        return 1;
    else if (checkdir(playermask, move, 0, 1))
        return 1;
    else if (checkdir(playermask, move, 1, 1))
        return 1;
    else if (checkdir(playermask, move, 1, 0))
        return 1;
    return 0;
}

int checkdir(long playermask, long move, int xshift, int yshift) {
    int shift, connected, omove;
    connected = 1;
    omove = move;
    while (1) {
        shift = xshift + (BYTE * yshift);
        if (shift > 0) {
            move <<= shift;
        } else if (shift < 0) {
            move >>= -shift;
        }
        if (move & playermask)
            connected++;
        else
            break;
    }
    while (1) {
        shift = -xshift + (BYTE * -yshift);
        if (shift > 0) {
            omove <<= shift;
        } else if (shift < 0) {
            omove >>= -shift;
        }
        if (omove & playermask)
            connected++;
        else
            break;
    }

    if (connected >= CONNECT_FOUR)
        return 1;
    else 
        return 0;
}

