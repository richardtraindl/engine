#ifndef ENDGAME_HPP
    #define ENDGAME_HPP

    #include "./values.hpp"
    #include "./piece.hpp"
    #include "./bitboard.hpp"


    using namespace std;


    class cEndGame100{
        public:
            static const int32_t m_single_top_right_KG[8][8];

            static const int32_t m_single_bottom_left_KG[8][8];

            cEndGame100();

    };


    class cEndGame110{
        public:
            static const int32_t m_single_top_left_KG[8][8];

            static const int32_t m_single_bottom_right_KG[8][8];

            cEndGame110();

    };


#endif