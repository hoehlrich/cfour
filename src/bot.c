#include "cfour.h"

int minimax(struct Board board, int depth, int aiturn, long lastmove, int alpha, int beta);

int choosemove(struct Board board, int depth, int *scores) {
    int i, maxi, score, maxscore;
    long move;
    struct Board tempboard;

    maxscore = -10000;
    for (i = 0; i < NUMCOL; i++) {
        tempboard = board;
        move = makemove(&tempboard.aimask, tempboard.aimask | tempboard.playermask, i);
        if (move < 0) {
            scores[i] = 0;
            continue;
        }
        score = minimax(tempboard, depth - 1, 0, move, -10000, 10000);
        scores[i] = score;
        if (score > maxscore) {
            maxscore = score;
            maxi = i;
        }
    }
    return maxi;
}

int minimax(struct Board board, int depth, int aiturn, long lastmove, int alpha, int beta) {
    int i, bestval, val;
    long move;
    struct Board tempboard;

    if (lastmove) {
        if (aiturn) {
            if (checkwin(board.playermask, lastmove))
                return -(100 + (depth * 10));
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
            if (val > bestval) {
                bestval = val;
                alpha = val;
            if (val > beta)
                break;
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
            if (val < bestval) {
                bestval = val;
                beta = val;
            if (val < alpha)
                break;
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

