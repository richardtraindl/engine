
#ifndef PAWN_HPP
    #define PAWN_HPP

    #include "./piece.hpp"

    using namespace std;

    class cPawn : virtual public cPiece{
        public:
            cPawn(cBoard *board, int pos);
            bool is_trapped();
            cMove do_move(int dst, int prompiece, int movecnt, int *score);
            bool undo_move(cMove *move, int movecnt, int *score);
            bool is_running();
    };

#endif

