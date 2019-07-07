
#ifndef PIECE_HPP
    #define PIECE_HPP

    #include "../match.hpp"

    using namespace std;

    class cPiece{
        public:
            static int DIRS_ARY[1];
            static int STEPS[1];
            static int MV_STEPS[1];
            static int MAXCNT;

            cMatch *match;
            int pos;
            int piece;
            int color;
            
            cPiece(cMatch *match, int pos);

            int dir_for_move(int src, int dst);

            int step_for_dir(int direction);

            bool is_trapped();

            bool is_move_stuck(int dst);

            bool is_move_valid(int dst, int prompiece);

            cMove do_move(int dst, int prompiece, int movecnt);

            bool undo_move(cMove *move, int movecnt);

    };

#endif