
#ifndef BISHOP_HPP
    #define BISHOP_HPP

    #include "./piece.hpp"

    using namespace std;

    class cBishop : public cPiece{
        public:
            static array<int, 8> DIRS_ARY;
            static array<int, 8> STEPS;
            static array<int, 10> MV_STEPS;
            static int MAXCNT;

            cBishop(cBoard *board, int pos);

            array<int, 8> get_dirs_ary() override;

            array<int, 8> get_steps() override;
            
            array<int, 10> get_mv_steps() override;

            int get_maxcnt() override;

            int dir_for_move(int src, int dst) override;

            int step_for_dir(int dir) override;
            
            // bool is_move_valid(int dst, int prompiece, list<cMove> *minutes) override;
    };

#endif

