
#ifndef BLACKPAWN_HPP
    #define BLACKPAWN_HPP

    #include <array> 
    #include "./piece.hpp"
    #include "../board.hpp"
    #include "../move.hpp"

    using namespace std;

    class cBlackPawn : public cPiece{
        public:
            static array<int, 8> STEPS;
            static array<int, 10> MV_STEPS;
            static array<int, 4> PROM_PIECES;
            static int MAXCNT;

            cBlackPawn(cBoard *board, int pos);

            array<int, 8> get_steps() override;

            array<int, 10> get_mv_steps() override;

            array<int, 4> get_prom_pieces() override;

            int get_maxcnt() override;

            int dir_for_move(int src, int dst) override;

            int step_for_dir(int dir) override;

            bool is_trapped();

            bool is_move_valid(int dst, int prompiece, list<cMove> *minutes) override;

            bool is_ep_move_ok(int dst, list<cMove> *minutes);
            
            cMove *do_move(int dst, int prompiece, int movecnt, int *score) override;

            bool undo_move(cMove *move, int movecnt, int *score) override;

            bool is_running();
    };

#endif
