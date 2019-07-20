
#ifndef KING_HPP
    #define KING_HPP

    #include <boost/multiprecision/cpp_int.hpp>
    #include "./piece.hpp"

    using namespace std;

    class cKing : public cPiece{
        public:
            static array<int, 8> STEPS;
            static int MAXCNT;

            cKing(cBoard *board, int pos);

            array<int, 8> get_steps() override;

            int get_maxcnt() override;

            bool is_trapped();

            bool is_move_stuck(int dst);

            bool is_move_valid(int dst, int prompiece);

            bool is_short_castling_ok(int dst);

            bool is_long_castling_ok(int dst);

            cMove *do_move(int dst, int prompiece, int movecnt, int *score);

            bool undo_move(cMove *move, int movecnt, int *score);

            int score_touches();

            // bool is_safe();
    };

#endif
