#ifndef PIECEEXT_HPP
    #define PIECEEXT_HPP

    #include <list> 
    #include "./piece.hpp"
    #include "./move.hpp"

    cMove *do_move_from_ext(cPiece *cpiece, int dst, int prompiece, int movecnt, int *score);

    bool undo_move_from_ext(cPiece *cpiece, cMove *move, int movecnt, int *score);

#endif