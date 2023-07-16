#include "cfour.h"

int minimax(struct Board board, int depth, int aiturn, long lastmove, int alpha, int beta);

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
        if (move < 0) {
            printf("|X| ");
            continue;
        }
        score = minimax(tempboard, depth - 1, 0, move, -10000, 10000);
        printf("|%d| ", score);
        if (score > maxscore) {
            maxscore = score;
            maxi = i;
        }
    }
    putchar('\n');
    return maxi;
}

int minimax(struct Board board, int depth, int aiturn, long lastmove, int alpha, int beta) {
    int i, bestval, val;
    long move;
    struct Board tempboard;

    if (lastmove) {
        if (aiturn) {
            if (checkwin(board.playermask, lastmove))
                return -100 - (depth*10);
        } else {
            if (checkwin(board.aimask, lastmove))
                return 100 + (depth * 10);
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
            val = minimax(tempboard, depth - 1, !aiturn, move, alpha, beta);
            if (val > beta)
                break;
            if (val > bestval) {
                bestval = val;
                alpha = val;
            }
        }
    } else {
        bestval = 100000;
        for (i = 0; i < NUMCOL; i++) {
            tempboard = board;
            move = makemove(&(tempboard.playermask), tempboard.aimask | tempboard.playermask, i);
            if (move < 0)
                continue;
            val = minimax(tempboard, depth - 1, !aiturn, move, alpha, beta);
            if (val < alpha)
                break;
            if (val < bestval) {
                bestval = val;
                beta = val;
            }
        }
    }

    return bestval;
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
            val += 7;
        if (checkwin(board.playermask, move))
            val -= 7;
    }
    return val;
}

