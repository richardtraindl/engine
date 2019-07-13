
#ifndef BLACKPAWN_HPP
    #define BLACKPAWN_HPP

    #include "./pawn.hpp"

    using namespace std;

    class cBlackPawn : public cPawn{
        public:
            static int STEPS[2];
            static int MAXCNT;
            static int MV_STEPS_RANK7[4][2];
            static int MV_STEPS_RANK2[12][2];
            static int MV_STEPS[3][2];
            
            cBlackPawn(cBoard *board, int pos);
            int dir_for_move(int src, int dst);
            int step_for_dir(int dir);
            bool is_move_valid(int dst, int prompiece, list<cMove> *minutes);
            bool is_ep_move_ok(int dst, list<cMove> *minutes);
    };

#endif
