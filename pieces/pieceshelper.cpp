
#include "./pieceshelper.hpp"

    cPiece *obj_for_piece(cBoard *board, unsigned pos){
        unsigned piece = board->getfield(pos);
        if(piece == PIECES["wPw"]){
            return new cWhitePawn(board, pos);
        }
        if(piece == PIECES["bPw"]){
            return new cBlackPawn(board, pos);
        }
        if(piece == PIECES["wKn"] || piece == PIECES["bKn"]){
            return new cKnight(board, pos);
        }
        if(piece == PIECES["wBp"] || piece == PIECES["bBp"]){
            return new cBishop(board, pos);
        }
        if(piece == PIECES["wRk"] || piece == PIECES["bRk"]){
            return new cRook(board, pos);
        }
        if(piece == PIECES["wQu"] || piece == PIECES["bQu"]){
            return new cQueen(board, pos);
        }
        if(piece == PIECES["wKg"] || piece == PIECES["bKg"]){
            return new cKing(board, pos);
        }
        return NULL;
    }
    