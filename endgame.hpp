#ifndef ENDGAME_HPP
    #define ENDGAME_HPP

    #include "./values.hpp"
    #include "./piece.hpp"
    #include "./bitboard.hpp"


    using namespace std;


    class cEndGame100_base{
        public:
            static int32_t m_score[5];

            cEndGame100_base();

    };


    class cEndGame100{
        public:
            static const int32_t m_single_KG[8][8];

            static array<uint64_t(*)[4], 4> m_100_series;

            static uint64_t m_bottom_left_estbnd[5][4];

            static uint64_t m_bottom_left_nthbnd[5][4];

            static uint64_t m_top_right_wstbnd[5][4];

            static uint64_t m_top_right_sthbnd[5][4];

            cEndGame100();

    };


    class cEndGame110{
        public:
            static const int32_t m_single_KG[8][8];

            static array<uint64_t(*)[4], 4> m_110_series;

            static uint64_t m_bottom_right_wstbnd[5][4];

            static uint64_t m_bottom_right_nthbnd[5][4];

            static uint64_t m_top_left_estbnd[5][4];

            static uint64_t m_top_left_sthbnd[5][4];

            cEndGame110();

    };


    class cEndGame120{
        public:
            static const int32_t m_single_KG[8][8];

            static array<uint64_t(*)[4], 4> m_120_series;

            static uint64_t m_bottom_left_estbnd[5][4];

            static uint64_t m_bottom_left_nthbnd[5][4];

            static uint64_t m_top_right_wstbnd[5][4];

            static uint64_t m_top_right_sthbnd[5][4];

            cEndGame120();

    };

    class cEndGame130{
        public:
            static const int32_t m_single_KG[8][8];

            static array<uint64_t(*)[4], 4> m_130_series;

            static uint64_t m_bottom_right_wstbnd[5][4];

            static uint64_t m_bottom_right_nthbnd[5][4];

            static uint64_t m_top_left_estbnd[5][4];

            static uint64_t m_top_left_sthbnd[5][4];

            cEndGame130();

    };

#endif