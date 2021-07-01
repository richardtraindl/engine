#ifndef MOVE_HPP
    #define MOVE_HPP

    #include <cstring>
    #include <cstdint>
    #include <string>
    #include <sstream>
    #include "./values.hpp"


    using namespace std;


    class cMove{
        public:
            uint8_t src_x;
            uint8_t src_y;
            uint8_t dst_x;
            uint8_t dst_y;
            uint8_t srcpiece;
            uint8_t dstpiece;
            uint8_t prompiece;

            cMove(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t srcpiece, uint8_t dstpiece, uint8_t prompiece);

            cMove();

            cMove(const cMove &move);
            // copy constructor

            bool is_en_passant();

            bool is_promotion();

            bool is_short_castling();

            bool is_long_castling();

            string format();

            static void coord_to_indices(uint8_t &x, uint8_t &y, string coord);

            static string indices_to_coord(uint8_t x, uint8_t y);

    };

#endif
