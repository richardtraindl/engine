
#ifndef PIECESHELPER_HPP
    #define PIECESHELPER_HPP

    #include "./piece.hpp"
    #include "./rook.hpp"
    #include "./bishop.hpp"
    #include "./queen.hpp"
    // #include "./king.hpp"
    #include "./knight.hpp"
    #include "./whitepawn.hpp"
    #include "./blackpawn.hpp"


    using namespace std;
    using boost::multiprecision::cpp_int;
    
    cPiece *obj_for_piece(cBoard *board, int pos);
        
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
#endif
