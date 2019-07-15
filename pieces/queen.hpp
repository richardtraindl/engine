
#ifndef QUEEN_HPP
    #define QUEEN_HPP

    #include "./piece.hpp"
    #include "./rook.hpp"
    #include "./bishop.hpp"

    using namespace std;

    class cQueen : public cRook, public cBishop{
        public:
            static int DIRS_ARY[8];
            static int STEPS[8];
            static int MV_STEPS[8][2];
            
            cQueen(cBoard *board, int pos);
            static int dir_for_move(int src, int dst);
            static int step_for_dir(int dir);
    };

#endif
