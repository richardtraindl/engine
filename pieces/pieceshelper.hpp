
#ifndef PIECESHELPER_HPP
    #define PIECESHELPER_HPP

    #include "./piece.hpp"
    #include "./rook.hpp"
    #include "./bishop.hpp"
    #include "./queen.hpp"
    #include "./king.hpp"
    #include "./knight.hpp"
    #include "./whitepawn.hpp"
    #include "./blackpawn.hpp"


    using namespace std;
    
    cPiece *obj_for_piece(cBoard *board, unsigned pos);
        
#endif
