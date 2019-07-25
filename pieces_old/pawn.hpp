
#ifndef PAWN_HPP
    #define PAWN_HPP

    #include "./piece.hpp"
    #include "../move.hpp"

    using namespace std;

    class cPawn : virtual public cPiece{
        public:
            cPawn(cBoard *board, int pos);

            bool is_trapped() override;

            bool is_move_valid(int dst, int prompiece, list<cMove> *minutes) override;

            cMove *do_move(int dst, int prompiece, int movecnt, int *score) override;

            bool undo_move(cMove *move, int movecnt, int *score) override;

            bool is_running();
            
    };

#endif

