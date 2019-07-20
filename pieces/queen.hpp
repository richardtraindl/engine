
#ifndef QUEEN_HPP
    #define QUEEN_HPP

    #include "./piece.hpp"
    #include "./rook.hpp"
    #include "./bishop.hpp"

    using namespace std;

    class cQueen : virtual public cPiece{
        public:
            static array<int, 8> DIRS_ARY;
            static array<int, 8> STEPS;
            static int MAXCNT;

            cQueen(cBoard *board, int pos);

            array<int, 8> get_dirs_ary() override;

            array<int, 8> get_steps() override;

            int get_maxcnt() override;

            static int dir_for_move(int src, int dst);

            static int step_for_dir(int dir);
    };

#endif
