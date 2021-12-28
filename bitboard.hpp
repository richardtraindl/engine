#ifndef BITBOARD_HPP
    #define BITBOARD_HPP

    #include <iostream>
    #include <cstdint>
    #include "./values.hpp"
    #include "./piece.hpp"


    using namespace std;


     class cBitBoard{
        public:
            uint64_t m_bitfields[4] = { 0, 0, 0, 0 };

            static const uint64_t m_tstmask[];

            static const uint64_t m_clearmask[];

            cBitBoard();

            uint8_t get(uint8_t x, uint8_t y) const;

            void set(uint8_t x, uint8_t y, uint8_t piece);

            void import_fields(const uint8_t fields[8][8]);

            bool compare(const uint64_t bitfields[4]) const;

            void prnt() const;

    };

#endif