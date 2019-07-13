
#ifndef ROOK_HPP
    #define ROOK_HPP

    #include "./piece.hpp"

    using namespace std;

    class cRook : virtual public cPiece{
        public:
            static unsigned DIRS_ARY[4];
            static int STEPS[4];
            static vector<pair<int, unsigned>> MV_STEPS;

            cRook(cBoard *board, int pos);
            static int dir_for_move(int src, int dst);
            static int step_for_dir(int dir);
            cMove *do_move(int dst, int prompiece, int movecnt, int *score);
            bool undo_move(cMove *move, int movecnt, int *score);
    };

#endif

