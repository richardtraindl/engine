
#ifndef BISHOP_HPP
    #define BISHOP_HPP

    #include "./piece.hpp"

    using namespace std;

    class cBishop : virtual public cPiece{
        public:
            static unsigned DIRS_ARY[4];
            static int STEPS[4];
            static vector<pair<int, unsigned>> MV_STEPS;

            cBishop(cBoard *board, int pos);
            int dir_for_move(int src, int dst);
            int step_for_dir(int dir);
    };

#endif

