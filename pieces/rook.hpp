
#ifndef ROOK_HPP
    #define ROOK_HPP

    #include "./piece.hpp"

    using namespace std;

    class cRook : virtual public cPiece{
        public:
            static int DIRS_ARY[4];
            static int STEPS[4];
            static int MV_STEPS[4][2];

            cRook(cBoard *board, unsigned pos);
            static int dir_for_move(int src, int dst);
            static int step_for_dir(int dir);
            cMove *do_move(int dst, int prompiece, int movecnt, int *score);
            bool undo_move(cMove *move, int movecnt, int *score);
    };

#endif

