
#ifndef KNIGHT_HPP
    #define KNIGHT_HPP

    #include "./piece.hpp"

    using namespace std;

    class cKnight : virtual public cPiece{
        public:
            static array<int, 8> STEPS;
            static int MAXCNT;

            cKnight(cBoard *board, int pos);

            array<int, 8> get_steps() override;

            int get_maxcnt() override;

            bool is_trapped();

            bool is_move_valid(int dst, int prompiece);
    };

#endif

