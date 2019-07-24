
#ifndef PIECEEXT_HPP
    #define PIECEEXT_HPP

    #include <list> 
    #include "./piece.hpp"
    #include "./move.hpp"
     
    bool is_move_valid_from_ext(cPiece *cpiece, int dst, int prompiece, list<cMove> *minutes);

    bool is_move_valid_for_whitepawn(cPiece *cpiece, int dst, int prompiece, list<cMove> *minutes);

    bool is_ep_move_ok_for_whitepawn(cPiece *piece, int dst, list<cMove> *minutes);

    bool is_move_valid_for_blackpawn(cPiece *cpiece, int dst, int prompiece, list<cMove> *minutes);

    bool is_ep_move_ok_for_blackpawn(cPiece *piece, int dst, list<cMove> *minutes);

    bool is_move_valid_for_knight(cPiece *cpiece, int dst, int prompiece);

    bool is_move_valid_for_king(cPiece *cpiece, int dst, int prompiece);

    bool is_short_castling_ok(cPiece *cpiece, int dst);

    bool is_long_castling_ok(cPiece *cpiece, int dst);

    bool is_move_valid_for_long_distance_piece(cPiece *cpiece, int dst, int prompiece);

#endif