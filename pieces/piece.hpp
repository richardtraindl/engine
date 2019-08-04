
#ifndef PIECE_HPP
    #define PIECE_HPP

    #include <list> 
    #include <array> 
    #include "../match.hpp"
    #include "../board.hpp"
    #include "../move.hpp"
    #include "./touch.hpp"

    using namespace std;

    class cPiece{
        public:
            cBoard *board;
            int pos;
            int piece;
            int color;

            cPiece(cBoard &newboard, int newpos);
            ~cPiece();

            array<int, 8> get_dirs_ary(); 

            array<int, 8> get_steps(); 

            array<int, 10> get_mv_steps();

            array<int, 4> get_prom_pieces(); 

            int get_maxcnt();

            static int dir_for_move(int piece, int src, int dst);

            static int step_for_dir(int piece, int dir);

            static bool is_queen_or_rook(int piece);

            static bool is_queen_or_bishop(int piece);

            bool is_trapped();

            bool is_move_stuck(int dst);

            bool is_move_valid(int dst, int prompiece, list<cMove> &minutes);
            
            cMove *do_move(int dst, int prompiece, int movecnt, int &score);

            void undo_move(cMove &move, int movecnt, int &score);

            void find_attacks_and_supports(list<cTouch> &supported, list<cTouch> &attacked);

            int score_touches();

            void generate_moves(cMatch &match, list<cMove> &moves);

            void generate_priomoves(cMatch &match, cMove *candidate, cMove *dbggmove, bool search_for_mate, list<cPrioMove> &priomoves);

            bool is_running_pawn();

            bool is_safe_king();

    };

#endif
