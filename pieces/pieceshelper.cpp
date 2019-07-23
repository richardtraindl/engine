
    #include "./pieceshelper.hpp"
    #include "./rook.hpp"
    #include "./bishop.hpp"
    #include "./queen.hpp"
    #include "./king.hpp"
    #include "./knight.hpp"
    #include "./whitepawn.hpp"
    #include "./blackpawn.hpp"
    #include "../values.hpp"

    cPiece *obj_for_piece(cBoard *board, int pos){
        int piece = board->getfield(pos);
        if(piece == mWPW){
            return new cWhitePawn(board, pos);
        }
        if(piece == mBPW){
            return new cBlackPawn(board, pos);
        }
        if(piece == mWKN || piece == mBKN){
            return new cKnight(board, pos);
        }
        if(piece == mWBP || piece == mBBP){
            return new cBishop(board, pos);
        }
        if(piece == mWRK || piece == mBRK){
            return new cRook(board, pos);
        }
        if(piece == mWQU || piece == mBQU){
            return new cQueen(board, pos);
        }
        if(piece == mWKG || piece == mBKG){
            return new cKing(board, pos);
        }
        return NULL;
    }
    
