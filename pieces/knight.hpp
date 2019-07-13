
#ifndef KNIGHT_HPP
    #define KNIGHT_HPP

    #include "./piece.hpp"

    using namespace std;

    class cKnight : public cPiece{
        public:
            static unsigned DIRS_ARY[1];
            static int STEPS[8];
            static vector<pair<int, unsigned>> MV_STEPS;
            static int MAXCNT;

            cKnight(cBoard *board, int pos);
            bool is_trapped();
            bool is_move_valid(int dst, int prompiece);
    };

#endif

