#include <stdio.h>
#include <math.h>

#define BLEN       42
#define NUM_ROW     6
#define NUM_COL     7
#define FOOTER      "1 2 3 4 5 6 7" 

/* six row, seven columns: 
 * [0, 1, 2, 3, 4, 5, 6,
 *  ...
 * 35, 36, 37, 38, 39, 40, 41]
 */

void printboard(long bluemask, long redmask);
int makemove(long * playermask, long fullmask, int col);
void takeinput(long * playermask, long fullmask);

int main() {
    long bluemask = 0;
    long redmask = 0;
    long * rp = &redmask;

    int i;
    long * playermaskp;
    for (i = 0; 1; i++) {
        printboard(bluemask, redmask);

        playermaskp = i % 2 ? &bluemask : &redmask;
        takeinput(playermaskp, bluemask & redmask);
    }
}

void takeinput(long * playermask, long fullmask) {
    printf("Enter the column of your move: ");
    int col;
    col = scanf("%d") - 1;
    if (makemove(playermask, fullmask, col) == 0) {
        printf("Try again");
        takeinput(playermask, fullmask);
    }
}

/* makemove: makes move on playermask if possible; reutrns 1 if successful 0 if not */
int makemove(long * playermask, long fullmask, int col) {
    int i;
    long move;
    for (i = 0; i < NUM_ROW; i++) {
        move = pow(2, ((NUM_COL - 1 - col) + (NUM_COL * i)));
        if ((fullmask & move) == 0) {
            *playermask = *playermask | move;
            return 1;
        }
    }
    return 0;
}

void printboard(long bluemask, long redmask) {
    int i;
    long space;
    for (i = BLEN - 1; i >= 0; i--) {
        space = pow(2, i);
        if ((bluemask & space) != 0) {
            printf("X ");
        } else if ((redmask & space) != 0) {
            printf("O ");
        } else {
            printf("- ");
        }

        if (i % NUM_COL == 0) {
            putchar('\n');
        }
    }
    printf("%s\n", FOOTER);
}

