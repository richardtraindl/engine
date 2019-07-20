
#ifndef BLACKPAWN_HPP
    #define BLACKPAWN_HPP

    #include "./pawn.hpp"

    using namespace std;

    class cBlackPawn : public cPawn{
        public:
            static array<int, 8> STEPS;
            static array<int, 3> MV_STEPS;
            static array<int, 4> MV_STEPS_START;
            static array<int, 4> PROM_PIECES;
            static int MAXCNT;

            cBlackPawn(cBoard *board, int pos);

            array<int, 8> get_steps() override;

            array<int, 3> get_mv_steps();

            array<int, 4> get_mv_steps_start();

            int get_maxcnt() override;

            static int dir_for_move(int src, int dst);

            static int step_for_dir(int dir);

            bool is_move_valid(int dst, int prompiece, list<cMove> *minutes);

            bool is_ep_move_ok(int dst, list<cMove> *minutes);
    };

#endif
