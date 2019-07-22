
#ifndef WHITEPAWN_HPP
    #define WHITEPAWN_HPP

    #include <array> 
    #include "./piece.hpp"
    #include "../board.hpp"
    #include "../move.hpp"

    using namespace std;

    class cWhitePawn : public cPiece{
        public:
            static array<int, 8> STEPS;
            static array<int, 10> MV_STEPS;
            //static array<int, 4> MV_STEPS_START;
            static array<int, 4> PROM_PIECES;
            static int MAXCNT;

            cWhitePawn(cBoard *board, int pos);

            array<int, 8> get_steps() override;

            array<int, 10> get_mv_steps();

            //array<int, 4> get_mv_steps_start();

            array<int, 4> get_prom_pieces() override;

            int get_maxcnt() override;

            static int dir_for_move(int src, int dst);

            static int step_for_dir(int dir);
            
            bool is_trapped();

            bool is_move_valid(int dst, int prompiece, list<cMove> *minutes) override;

            bool is_ep_move_ok(int dst, list<cMove> *minutes);

            cMove *do_move(int dst, int prompiece, int movecnt, int *score) override;

            bool undo_move(cMove *move, int movecnt, int *score) override;

            bool is_running();
    };

#endif

