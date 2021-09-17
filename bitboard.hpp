#ifndef BITBOARD_HPP
    #define BITBOARD_HPP

    #include <iostream>
    #include <cstdint>
    #include "./values.hpp"
    #include "./piece.hpp"


    using namespace std;


     class cBitBoard{
        private:
            static uint64_t cut_row(uint64_t src, uint8_t rank);

            static uint64_t flip_row_vertical(uint64_t src);
         
        public:
            uint64_t m_bitfields[4] = { 0, 0, 0, 0 };

            static const uint64_t m_tstmask[];

            static const uint64_t m_clearmask[];

            cBitBoard();

            uint8_t get(uint8_t x, uint8_t y);

            void set(uint8_t x, uint8_t y, uint8_t piece);

            void import_fields(uint8_t fields[8][8]);

            bool compare(const uint64_t bitfields[4]);

            void prnt();

            static void flip_horizontal(uint64_t dst[4], const uint64_t src[4]);

            static void flip_vertical(uint64_t dst[4], const uint64_t src[4]);

            static void switch_colors(uint64_t dst[4], const uint64_t src[4]);

    };

#endif