#ifndef ENDGAME_HPP
    #define ENDGAME_HPP

    #include "./values.hpp"
    #include "./piece.hpp"
    #include "./match.hpp"
    #include "./board.hpp"
    #include "./bitboard.hpp"


    using namespace std;


    class cEndGame100_base{
        public:
            static int32_t eval_push_kg_to_margin(uint8_t stage, uint8_t depth, const uint8_t fields[8][8], uint8_t wkg_x, uint8_t wkg_y, uint8_t bkg_x, uint8_t bkg_y);

            uint8_t eval_path_to_mate(uint8_t stage, const uint8_t fields[8][8]);

            cEndGame100_base();

    };


    class cEndGame100{
        public:
            static const int32_t m_single_KG[8][8];

            static const uint64_t m_path_to_mate[4][5][4];

            cEndGame100();

    };


    class cEndGame110{
        public:
            static const int32_t m_single_KG[8][8];

            static const uint64_t m_path_to_mate[4][5][4];

            cEndGame110();

    };


    class cEndGame120{
        public:
            static const uint64_t m_path_to_mate[4][5][4];

            cEndGame120();

    };


    class cEndGame130{
        public:
            static const uint64_t m_path_to_mate[4][5][4];

            cEndGame130();

    };


#endif