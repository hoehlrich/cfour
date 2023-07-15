#include "cfour.h"

int evaluateposition(long ourmask, long opmask, int ourturn, long lastmove, int eval);

int choosemove(long ourmask, long opmask, int ourturn) {
    int i, eval, maxeval, bestcol;
    long move, temp;
    move = 4;
    maxeval = 0;
    for (i = 0; i < NUMROW; i++) {
        temp = ourmask;
        move = makemove(&temp, ourmask | opmask, i);
        eval = evaluateposition(ourmask, opmask, ourturn, move, 0);
        if (eval > maxeval) {
            maxeval = eval;
            bestcol = i;
        }
    }
    return bestcol;
}

int evaluateposition(long ourmask, long opmask, int ourturn, long lastmove, int eval) {
    if (checkwin(ourmask, lastmove) && !ourturn) {
        eval += 10000000000;
    }
    if (checkwin(opmask, lastmove) && ourturn) {
        eval -= 10000000000;
    }
    return eval;
}
