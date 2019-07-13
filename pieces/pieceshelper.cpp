
#include "./pieceshelper.hpp"
    /*
    #if !defined(mOBJ_FOR_PIECE)
        #define mOBJ_FOR_PIECE(MATCH, PIECE, POS) \
                if(PIECE == mWPW){ \
                    return cWhitePawn(MATCH, POS); \
                } \
                if(PIECE == mBPW){ \
                    return cBlackPawn(MATCH, POS); \
                } \
                if(PIECE == mWKN || PIECE == mBKN){ \
                    return cKnight(MATCH, POS); \
                } \
                if(PIECE == mWRK || PIECE == mBRK){ \
                    return cRook(MATCH, POS); \
                } \
                if(PIECE == mWBP || PIECE == mBBP){ \
                    return cBishop(MATCH, POS); \
                } \
                if(PIECE == mWPW || PIECE == mBPW){ \
                    return cQueen(MATCH, POS); \
                } \
                return NULL;
    #endif
    */
    

    cPiece *obj_for_piece(cBoard *board, unsigned pos){
        unsigned piece = board->getfield(pos);
        if(piece == PIECES["wPw"]){
            return new cWhitePawn(board, pos);
        }
        if(piece == PIECES["bPw"]){
            return new cBlackPawn(board, pos);
        }
        if(piece == PIECES["wKn"] or piece == PIECES["bKn"]){
            return new cKnight(board, pos);
        }
        if(piece == PIECES["wBp"] or piece == PIECES["bBp"]){
            return new cBishop(board, pos);
        }
        if(piece == PIECES["wRk"] or piece == PIECES["bRk"]){
            return new cRook(board, pos);
        }
        if(piece == PIECES["wQu"] or piece == PIECES["bQu"]){
            return new cQueen(board, pos);
        }
        /*if(piece == PIECES["wKg"] or piece == PIECES["bKg"]){
            return new cKing(board, pos);
        }*/
        return NULL;
    }
    