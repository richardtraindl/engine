
#ifndef PIECE_HPP
    #define PIECE_HPP

    #include <list> 
    #include "../board.hpp"
    #include "../move.hpp"
    #include "./searchforpiece.hpp"
    #include "../values.hpp"
    #include "../helper.hpp"

    using namespace std;

    struct mvstep{
        int step;
        int piece;
    };

    class cPiece{
        public:
            static int DIRS_ARY[];
            static int STEPS[];
            static int MV_STEPS[][];
            static int MAXCNT;

            cBoard *board;
            int pos;
            int piece;
            int color;
            
            cPiece(cBoard *board, int pos);
            static int dir_for_move(int src, int dst);
            static int step_for_dir(int direction);
            bool is_trapped();
            bool is_move_stuck(int dst);
            bool is_move_valid(int dst, int prompiece);
            cMove *do_move(int dst, int prompiece, int movecnt, int *score);
            bool undo_move(cMove *move, int movecnt, int *score);
            void find_attacks_and_supports(list<cTouch> *attacked, list<cTouch> *supported);
            int score_touches();
            list<cMove> *generate_moves(list<cMove> *minutes);
            list<cPrioMove> *generate_priomoves(list<cMove> *minutes, cMove *candidate, cMove *dbggmove, bool search_for_mate);
    };

#endif
