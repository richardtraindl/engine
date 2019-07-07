
#ifndef BISHOP_HPP
    #define BISHOP_HPP

    #include "./piece.hpp"
    #include "../match.hpp"
    #include "../values.hpp"

    using namespace std;

    class cBishop : public cPiece{
        public:
            static int DIRS_ARY[4];
            static int STEPS[4];
            static int MV_STEPS[4][2];
            
            cBishop();

        int dir_for_move(int src, int dst);

        int step_for_dir(int dir);
    };

#endif

