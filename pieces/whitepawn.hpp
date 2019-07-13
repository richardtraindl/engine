
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
            
            cWhitePawn(cBoard *board, unsigned pos);
            static int dir_for_move(unsigned src, unsigned dst);
            static int step_for_dir(unsigned dir);
            bool is_move_valid(unsigned dst, unsigned prompiece, list<cMove> *minutes);
            bool is_ep_move_ok(unsigned dst, list<cMove> *minutes);
    };

#endif


