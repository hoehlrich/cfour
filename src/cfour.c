#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "cfour.h"

#define FOOTER      "1 2 3 4 5 6 7" 
#define CONNECT_FOUR    4


/* 6 rows of bytes
 * last bit unused
 * [0, 1, 2, 3, 4, 5, 6, -
 * ...
 * 40, 41, 42, 43, 44, 45, 46, -]
*/

int checkdir(long playermask, long move, int xshift, int yshift);
long inputfailed(long * playermaskp, long fullmask);

/* takeinput: takes user input and executes move; returns move */
int takeinput(long * playermaskp, long fullmask) {
    printf("Enter the column of your move: ");

    int col;
    long move;

    scanf("%d", &col);
    while (getchar() != '\n');

    if (col == -1)
        quit();
    else if (col >= 0 && col <= NUMCOL)
        col--;
    else
        return inputfailed(playermaskp, fullmask);

    move = makemove(playermaskp, fullmask, col);
    if (move == -1) {
        return inputfailed(playermaskp, fullmask);
    }
    return move;
}

/* inputfailed: called when input fails; asks to try again and returns first input */
long inputfailed(long * playermaskp, long fullmask) {
    printf("Input failed. Try again\n");
    return takeinput(playermaskp, fullmask);
}

/* makemove: makes move on playermask if possible; reutrns move if successful -1 if not */
long makemove(long * playermaskp, long fullmask, int col) {
    int i, n;
    long move;

    for (i = 0; i < NUMROW; i++) {
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
    for (i = NUMROW - 1; i >= 0; i--) {
        for (j = 0; j < NUMCOL; j++) {
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
    int shift, connected;
    long omove;
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

