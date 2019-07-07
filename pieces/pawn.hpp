
#ifndef PAWN_HPP
    #define PAWN_HPP

    #include "./piece.hpp"
    #include "../match.hpp"
    #include "../values.hpp"

    using namespace std;

    class cPawn : public cPiece{
        public:
            bool is_trapped();

            cMove do_move(int dst, int prompiece, int movecnt);

            bool undo_move(cMove *move, int movecnt);

            bool is_running();
    };

#endif

