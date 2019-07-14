
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
        unsigned piece;
    };

    class cPiece{
        public:
            static unsigned DIRS_ARY[1];
            static int STEPS[1];
            static vector<pair<int, unsigned>> MV_STEPS;
            static int MAXCNT;

            cBoard *board;
            unsigned pos;
            unsigned piece;
            unsigned color;
            
            cPiece(cBoard *board, unsigned pos);
            static unsigned dir_for_move(unsigned src, unsigned dst);
            static int step_for_dir(unsigned direction);
            bool is_trapped();
            bool is_move_stuck(unsigned dst);
            bool is_move_valid(unsigned dst, unsigned prompiece);
            cMove *do_move(unsigned dst, unsigned prompiece, int movecnt, int *score);
            bool undo_move(cMove *move, int movecnt, int *score);
            void find_attacks_and_supports(list<cTouch> *attacked, list<cTouch> *supported);
            int score_touches();
            list<cMove> *generate_moves(list<cMove> *minutes);
            list<cPrioMove> *generate_priomoves(list<cMove> *minutes, cMove *candidate, cMove *dbggmove, bool search_for_mate);
    };

#endif