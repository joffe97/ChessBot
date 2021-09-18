//
// Created by Joach on 19.03.2021.
//

#include "botBrain.h"
#include "pieces.h"
#include "board.h"

/*int boardvalue_pawn[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
      5,  10,  10, -20, -20,  10,  10,   5,
      5,  -5, -10,   0,   0, -10,  -5,   5,
      0,   0,   0,  20,  20,   0,   0,   0,
      5,   5,  10,  25,  25,  10,   5,   5,
     10,  10,  20,  30,  30,  20,  10,  10,
     50,  50,  50,  50,  50,  50,  50,  50,
      0,   0,   0,   0,   0,   0,   0,   0
};

int boardvalue_knight[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20,   0,   5,   5,   0, -20, -40,
    -30,   5,  10,  15,  15,  10,   5, -30,
    -30,   0,  15,  20,  20,  15,   0, -30,
    -30,   5,  15,  20,  20,  15,   5, -30,
    -30,   0,  10,  15,  15,  10,   0, -30,
    -40, -20,   0,   0,   0,   0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50
};

int boardvalue_bishop[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10,   5,   0,   0,   0,   0,   5, -10,
    -10,  10,  10,  10,  10,  10,  10, -10,
    -10,   0,  10,  10,  10,  10,   0, -10,
    -10,   5,   5,  10,  10,   5,   5, -10,
    -10,   0,   5,  10,  10,   5,   0, -10,
    -10,   0,   0,   0,   0,   0,   0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};

int boardvalue_rook[64] = {
      0,   0,   0,   5,   5,   0,   0,   0,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
      5,  10,  10,  10,  10,  10,  10,   5,
      0,   0,   0,   0,   0,   0,   0,   0
};

int boardvalue_queen[64] = {
    -20, -10, -10,  -5,  -5, -10, -10, -20,
    -10,   0,   5,   0,   0,   0,   0, -10,
    -10,   5,   5,   5,   5,   5,   0, -10,
      0,   0,   5,   5,   5,   5,   0,  -5,
     -5,   0,   5,   5,   5,   5,   0,  -5,
    -10,   0,   5,   5,   5,   5,   0, -10,
    -10,   0,   0,   0,   0,   0,   0, -10,
    -20, -10, -10,  -5,  -5, -10, -10, -20
};

int boardvalue_king[64] = {
     20,  30,  10,   0,   0,  10,  30,  20,
     20,  20,   0,   0,   0,   0,  20,  20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30
};*/

#define INFINITY 999999

// Values gotten from https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function

#define MG_PAWN_VALUE 82 
#define MG_KNIGHT_VALUE 337
#define MG_BISHOP_VALUE 365
#define MG_ROOK_VALUE 477
#define MG_QUEEN_VALUE 1025
#define MG_KING_VALUE 20000

#define EG_PAWN_VALUE 94
#define EG_KNIGHT_VALUE 281
#define EG_BISHOP_VALUE 297
#define EG_ROOK_VALUE 512
#define EG_QUEEN_VALUE 936
#define EG_KING_VALUE 20000

#define MG_MAX_VALUE (16 * MG_PAWN_VALUE + 4 * (MG_KNIGHT_VALUE + MG_BISHOP_VALUE + MG_ROOK_VALUE) + 2 * MG_QUEEN_VALUE)


int mg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0,
};

int eg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0,
};

int mg_knight_table[64] = {
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23,
};

int eg_knight_table[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64,
};

int mg_bishop_table[64] = {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21,
};

int eg_bishop_table[64] = {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int mg_rook_table[64] = {
     32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
     -24, -11,   7,  26, 24, 35,  -8, -20,
     -36, -26, -12,  -1,  9, -7,   6, -23,
     -45, -25, -16, -17,  3,  0,  -5, -33,
     -44, -16, -20,  -9, -1, 11,  -6, -71,
     -19, -13,   1,  17, 16,  7, -37, -26,
};

int eg_rook_table[64] = {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20,
};

int mg_queen_table[64] = {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50,
};

int eg_queen_table[64] = {
     -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41,
};

int mg_king_table[64] = {
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14,
};

int eg_king_table[64] = {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};

int empty_table[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


int mg_piece_values[7] = {0, MG_PAWN_VALUE, MG_KNIGHT_VALUE, MG_BISHOP_VALUE, MG_ROOK_VALUE, MG_QUEEN_VALUE, MG_KING_VALUE};
int eg_piece_values[7] = {0, EG_PAWN_VALUE, EG_KNIGHT_VALUE, EG_BISHOP_VALUE, EG_ROOK_VALUE, EG_QUEEN_VALUE, EG_KING_VALUE};

int *mg_piece_tables[7] = {empty_table, mg_pawn_table, mg_knight_table, mg_bishop_table, mg_rook_table, mg_queen_table, mg_king_table};
int *eg_piece_tables[7] = {empty_table, eg_pawn_table, eg_knight_table, eg_bishop_table, eg_rook_table, eg_queen_table, eg_king_table};


// TODO: Add weights for endgame aswell. This could affect more if there is less pieces on the board. E.g. 80% endgame weights and 20% earlygame weights.

void initMoveData(struct moveData *md) {
    md->isSet = 0;
    md->from = 0;
    md->to = 0;
    md->score = 0;
}

void setMoveData(struct moveData *md, int from, int to, int score) {
    md->isSet = 1;
    md->from = from;
    md->to = to;
    md->score = score;
}

int maxMoveData(struct moveData **mds, int n) {
    int highestIndex = 0;
    for (int i = 1; i < n; i++) {
        if (mds[i]->score > mds[highestIndex]->score) {
            highestIndex = i;
        }
    }
    return highestIndex;
}

int minMoveData(struct moveData **mds, int n) {
    int lowestIndex = 0;
    for (int i = 1; i < n; i++) {
        if (mds[i]->score < mds[lowestIndex]->score) {
            lowestIndex = i;
        }
    }
    return lowestIndex;
}

// Return endgame rate. 0 for early earlygame and 100 for late lategame.
int calculateEndgameRate(int *board) {
    int pieceSum, pieceType;
    pieceSum = 0;

    for (int i = 0; i < 64; i++) {
        pieceType = getPieceType(board[i]);
        if (!pieceType || pieceType == KING) continue;
        pieceSum += mg_piece_values[pieceType];
    }

    pieceSum *= 100;

    return pieceSum > MG_MAX_VALUE ? 0 : 100 - (pieceSum / MG_MAX_VALUE);
}

int getPieceValue(int pieceType, int endgameRate) {
    int mg, eg;
    mg = mg_piece_values[pieceType] * (100 - endgameRate);
    eg = eg_piece_values[pieceType] * endgameRate;
    return (mg + eg) / 200;
}

int getRoutePieceValue(int *board, int route, int endgameRate) {
    int piece, pieceType, pieceValue, valueIndex, routeValue, mg, eg;

    piece = getPiece(board, route);
    pieceType = getPieceType(piece);
    pieceValue = getPieceValue(pieceType, endgameRate);

    if (getPieceColor(piece) == WHITE) {
        valueIndex = getRouteNo(getColumn(route), 7 - getRow(route));
    } else {
        valueIndex = route;
    }

    mg = mg_piece_tables[pieceType][valueIndex] * (100 - endgameRate);
    eg = eg_piece_tables[pieceType][valueIndex] * endgameRate;
    routeValue = (mg + eg) / 200;

    return routeValue + pieceValue;
}

int getScoreForColor(int *board, int color) {
    int score, piece, endgameRate;
    score = 0;
    endgameRate = calculateEndgameRate(board);

    for (int route = 0; route < 64; route++) {
        piece = getPiece(board, route);
        if (!piece || getPieceColor(piece) != color) continue;
        score += getRoutePieceValue(board, route, endgameRate);
    }

    return score;
}

int getScore(int *board) {
    int score, piece, endgameRate;
    score = 0;
    endgameRate = calculateEndgameRate(board);

    for (int route = 0; route < 64; route++) {
        piece = getPiece(board, route);
        if (!piece) continue;
        score += (((getPieceColor(piece) == WHITE) * 2) - 1) * getRoutePieceValue(board, route, endgameRate);
    }

    return score;
} 

int getScoreOfBoardAfterMove(int *board, int from, int to) {
    int board_copy[64];
    copyBoard(board, board_copy);    
    movePiece(board_copy, from, to);
    return getScore(board_copy);
}

void getAllRoutesForColor(int *dest, int *board, int color) {
    int piece, destsize;
    destsize = 0;

    for (int i = 0; i < 64; i++) {
        piece = getPiece(board, i);
        if (piece && getPieceColor(piece) == color) {
            dest[destsize] = i;
            destsize++;
        }
    }

    dest[destsize] = -1;
}

/*
int indexOfBestBoard(int **boards, int colorsTurn) {
    int bestIndex;
    bestIndex = 0;

    for (int i = 0; boards[i] != NULL; i++) {
        if (getScore(boards[i]) > getScore(boards[bestIndex])) {
            bestIndex = i;
        }
    }

    return bestIndex;
}
*/

void getBestMoveAtBoard(struct moveData *md, int *board, int colorsTurn) {
    int availableMoves[32], pieceRoutes[64], score, from, to;

    getAllRoutesForColor(pieceRoutes, board, colorsTurn);

    for (int i = 0; i < 64; i++) {
        from = pieceRoutes[i];
        if (from == -1) break;
        getAvailableMoves(availableMoves, board, from);

        for (int j = 0; j < 32; j++) {
            to = availableMoves[j];
            if (to == -1) break;
            score = getScoreOfBoardAfterMove(board, from, to);

            if (md->isSet == 0 || (score - md->score) * (((colorsTurn == WHITE) * 2) - 1) > 0) {
                setMoveData(md, from, to, score);
            }
        }
    }
}

struct moveData minmax(int *board, int depth, int colorsTurn, int alpha, int beta) {
    struct moveData md, mdTmp;
    int from, to, pieceRoutes[64], availableMoves[32], board_copy[64];

    initMoveData(&md);

    if (depth <= 1) {
        getBestMoveAtBoard(&md, board, colorsTurn);
        return md;
    }

    if (colorsTurn == WHITE) {
        setMoveData(&md, -1, -1, -INFINITY);
        getAllRoutesForColor(pieceRoutes, board, WHITE);

        for (int i = 0; i < 64; i++) {
            from = pieceRoutes[i];
            if (from == -1) break;
            getAvailableMoves(availableMoves, board, from);

            for (int j = 0; j < 32; j++) {
                to = availableMoves[j];
                if (to == -1) break;
                copyBoard(board, board_copy);
                movePiece(board_copy, from, to);
                mdTmp = minmax(board_copy, depth - 1, BLACK, alpha, beta);
                if (mdTmp.score > md.score) {
                    setMoveData(&md, from, to, mdTmp.score);
                }
                if (mdTmp.score > alpha) alpha = mdTmp.score;
                if (beta <= alpha) {
                    i = 64;
                    break;
                }
            }
        }
        return md;
    }

    else {
        setMoveData(&md, -1, -1, INFINITY);
        getAllRoutesForColor(pieceRoutes, board, BLACK);

        for (int i = 0; i < 64; i++) {
            from = pieceRoutes[i];
            if (from == -1) break;
            getAvailableMoves(availableMoves, board, from);

            for (int j = 0; j < 32; j++) {
                to = availableMoves[j];
                if (to == -1) break;
                copyBoard(board, board_copy);
                movePiece(board_copy, from, to);
                mdTmp = minmax(board_copy, depth - 1, WHITE, alpha, beta);
                if (mdTmp.score < md.score) {
                    setMoveData(&md, from, to, mdTmp.score);
                }
                if (mdTmp.score < beta) beta = mdTmp.score;
                if (beta <= alpha) {
                    i = 64;
                    break;
                }
            }
        }
        return md;
    }
}

void getBestMove(int *from_dest, int *to_dest, int *board, int depth, int colorsTurn) {
    struct moveData md;
    md = minmax(board, depth, colorsTurn, -INFINITY, INFINITY);
    *from_dest = md.from;
    *to_dest = md.to;
}

