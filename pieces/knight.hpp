
#ifndef KNIGHT_HPP
    #define KNIGHT_HPP

    #include <array> 
    #include "./piece.hpp"

    using namespace std;

    class cKnight : public cPiece{
        public:
            static array<int, 8> STEPS;
            static array<int, 10> MV_STEPS;
            static int MAXCNT;

            cKnight(cBoard *board, int pos);

            array<int, 8> get_steps() override;
            
            array<int, 10> get_mv_steps() override;

            int get_maxcnt() override;

            bool is_trapped();

            bool is_move_valid(int dst, int prompiece);
    };

#endif

