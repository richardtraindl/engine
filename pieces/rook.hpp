
#ifndef ROOK_HPP
    #define ROOK_HPP

    #include "./piece.hpp"
    #include "../match.hpp"
    #include "../values.hpp"

    using namespace std;

    class cRook : public cPiece{
        public:
            static int DIRS_ARY[4];
            static int STEPS[4];
            static int MV_STEPS[4][2];

            cRook();

            int dir_for_move(int src, int dst);

            int step_for_dir(int dir);

            cMove do_move(int dst, int prompiece, int movecnt);

            bool undo_move(cMove *move, int movecnt);
    };

#endif

