#include "cfour.h"

int minimax(struct Board board, int depth, int aiturn, long lastmove);

int choosemove(long ourmask, long opmask, int depth) {
    printf("inside choosemove\n");
    int i, maxi, score, maxscore;
    long move;
    struct Board board, tempboard;
    board.aimask = ourmask;
    board.playermask = opmask;

    maxscore = -10000;
    for (i = 0; i < NUMCOL; i++) {
        tempboard = board;
        move = makemove(&tempboard.aimask, tempboard.aimask | tempboard.playermask, i);
        score = minimax(tempboard, depth, 0, move);
        if (move < 0)
            score = -100000;
        printf("score %d\n", score);
        if (score > maxscore) {
            maxscore = score;
            maxi = i;
        }
    }
    return maxi;
}

int heuristic(struct Board board) {
    int n, val;
    long move;

    val = 0;
    for (n = 0; n < NUMCOL*NUMROW; n++) {
        move = pow(2, n);
        if (move & fullmask(board))
            continue;
        if (checkwin(board.aimask, move))
            val += 10;
        if (checkwin(board.playermask, move))
            val -= 10;
    }
    return val;
}

int minimax(struct Board board, int depth, int aiturn, long lastmove) {
    int i, bestval, val;
    long move;
    struct Board tempboard;

    if (lastmove) {
        if (aiturn) {
            if (checkwin(board.playermask, lastmove))
                return -10 * depth;
        } else {
            if (checkwin(board.aimask, lastmove))
                return 10 * depth;
        }
    }

    if (depth == 0)
        return heuristic(board);

    if (aiturn) {
        bestval = -100000;
        for (i = 0; i < NUMCOL; i++) {
            tempboard = board;
            move = makemove(&(tempboard.aimask), tempboard.aimask | tempboard.playermask, i);
            if (move < 0)
                continue;
            val = minimax(tempboard, depth - 1, !aiturn, move);
            if (val > bestval)
                bestval = val;
        }
    } else {
        bestval = 100000;
        for (i = 0; i < NUMCOL; i++) {
            tempboard = board;
            move = makemove(&(tempboard.playermask), tempboard.aimask | tempboard.playermask, i);
            if (move < 0)
                continue;
            val = minimax(tempboard, depth - 1, !aiturn, move);
            if (val < bestval)
                bestval = val;
        }
    }

    return bestval;
}

