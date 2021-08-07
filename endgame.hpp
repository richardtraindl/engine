#ifndef ENDGAME_HPP
    #define ENDGAME_HPP

    //#include <vector>
    #include "./values.hpp"
    #include "./piece.hpp"


    using namespace std;

    class cEndGame001{
        public:
            cEndGame001();

            static const cPiece m_mate_with_wkn_wbp[][4];

            static const int32_t m_white_WKG[8][8];

            static const int32_t m_white_BKG[8][8];
    };

#endif