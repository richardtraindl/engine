
#ifndef PIECE_HPP
    #define PIECE_HPP

    #include <list> 
    #include <array> 
    #include "../board.hpp"
    #include "../move.hpp"
    #include "./touch.hpp"

    using namespace std;

    class cPiece{
        public:
            static array<int, 8> DIRS_ARY;
            static array<int, 8> STEPS;
            static array<int, 10> MV_STEPS;
            static array<int, 4> PROM_PIECES;
            static int MAXCNT;
            cBoard *board;
            int pos;
            int piece;
            int color;
            
            cPiece(cBoard *board, int pos);

            virtual array<int, 8> get_dirs_ary(); 

            virtual array<int, 8> get_steps(); 

            virtual array<int, 10> get_mv_steps();

            virtual array<int, 4> get_prom_pieces(); 

            virtual int get_maxcnt();

            static int dir_for_move(int src, int dst);

            static int step_for_dir(int direction);

            bool is_trapped();

            bool is_move_stuck(int dst);

            bool is_move_valid(int dst, int prompiece);

            cMove *do_move(int dst, int prompiece, int movecnt, int *score);

            bool undo_move(cMove *move, int movecnt, int *score);

            void find_attacks_and_supports(list<cTouch> *attacked, list<cTouch> *supported);

            int score_touches();

            void generate_moves(list<cMove> *minutes, list<cMove> *moves);

            void generate_priomoves(list<cMove> *minutes, cMove *candidate, cMove *dbggmove, bool search_for_mate, list<cPrioMove> *priomoves);
    };

#endif
