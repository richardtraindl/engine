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
            uint8_t m_src_x;

            uint8_t m_src_y;

            uint8_t m_dst_x;

            uint8_t m_dst_y;

            uint8_t m_srcpiece;

            uint8_t m_dstpiece;

            uint8_t m_prompiece;

            uint8_t m_prio;

            static const uint8_t PRIO_PROMOTION = 10;
            static const uint8_t PRIO_EN_PASSANT = 29;
            static const uint8_t PRIO_CAPTURE = 30;
            static const uint8_t PRIO_RUNNING_PAWN = 35;
            static const uint8_t PRIO_PRE_CALC = 35;
            static const uint8_t PRIO_URGENT_TOUCH = 40;
            static const uint8_t PRIO_CASTLING = 70;
            static const uint8_t PRIO_STANDARD = 100;
            static const uint8_t PRIO_BAD_DSTFIELD = 110;

            cMove(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t srcpiece, uint8_t dstpiece, uint8_t prompiece, uint8_t prio);

            cMove();

            cMove(const cMove &move);
            // copy constructor

            bool is_en_passant() const;

            bool is_promotion() const;

            bool is_short_castling() const;

            bool is_long_castling() const;

            bool compare(const cMove &move) const;

            string format(bool ext) const;

            static void coord_to_indices(uint8_t &x, uint8_t &y, string coord);

            static string indices_to_coord(uint8_t x, uint8_t y);

    };

#endif
