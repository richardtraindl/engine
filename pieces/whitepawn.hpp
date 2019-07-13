
#ifndef WHITEPAWN_HPP
    #define WHITEPAWN_HPP

    #include "./pawn.hpp"

    using namespace std;

    class cWhitePawn : public cPawn{
        public:
            static int STEPS[2];
            static int MAXCNT;
            static vector<pair<int, unsigned>> MV_STEPS_RANK2;
            static vector<pair<int, unsigned>> MV_STEPS_RANK7;
            static vector<pair<int, unsigned>> MV_STEPS;
            
            cWhitePawn(cBoard *board, int pos);
            int dir_for_move(int src, int dst);
            int step_for_dir(int dir);
            bool is_move_valid(int dst, int prompiece, list<cMove> *minutes);
            bool is_ep_move_ok(int dst, list<cMove> *minutes);
    };

#endif


