
#ifndef QUEEN_HPP
    #define QUEEN_HPP

    #include "./rook.hpp"
    #include "./bishop.hpp"
    #include "../match.hpp"
    #include "../values.hpp"

    using namespace std;

    class cQueen : public cBishop, public cRook{
        public:
            static int DIRS_ARY[8];
            static int STEPS[8];
            static int MV_STEPS[8][2];

            int dir_for_move(int src, int dst);

            int step_for_dir(int dir);
    };

#endif
