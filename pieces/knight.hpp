
#ifndef KNIGHT_HPP
    #define KNIGHT_HPP

    #include "./piece.hpp"
    #include "../match.hpp"
    #include "../values.hpp"

    using namespace std;

    class cKnight : public cPiece{
        public:
            static int DIRS_ARY[1];
            static int STEPS[8];
            static int MV_STEPS[8][2];
            static int MAXCNT;

            cKnight();

        bool is_trapped();

        bool is_move_valid(int dst, int prompiece);
    };

#endif

