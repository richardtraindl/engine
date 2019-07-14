
#ifndef KING_HPP
    #define KING_HPP

    #include <boost/multiprecision/cpp_int.hpp>
    #include "./piece.hpp"

    using namespace std;

    class cKing : public cPiece{
        public:
            static unsigned DIRS_ARY[1];
            static int STEPS[8];
            static vector<pair<int, unsigned>> MV_STEPS;
            static int MAXCNT;

            cKing(cBoard *board, unsigned pos);
            bool is_trapped();
            bool is_move_stuck(unsigned dst);
            bool is_move_valid(unsigned dst, unsigned prompiece);
            bool is_short_castling_ok(unsigned dst);
            bool is_long_castling_ok(unsigned dst);
            cMove *do_move(unsigned dst, unsigned prompiece, int movecnt, int *score);
            bool undo_move(cMove *move, int movecnt, int *score);
            int score_touches();
            // bool is_safe();
    };

#endif
