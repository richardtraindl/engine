  #ifndef MOVE_HPP

    #define MOVE_HPP


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

            uint32_t m_impact;

            static const uint8_t P_HIGH_UP      = 20;
            static const uint8_t P_HIGH         = 40;
            static const uint8_t P_HIGH_DOWN    = 60;
            static const uint8_t P_MEDIUM_UP    = 80;
            static const uint8_t P_MEDIUM       = 100;
            static const uint8_t P_MEDIUM_DOWN  = 120;
            static const uint8_t P_LOW          = 140;

            cMove(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t srcpiece, uint8_t dstpiece, uint8_t prompiece, uint8_t prio);

            cMove();

            cMove(const cMove &move);
            // copy constructor

            ~cMove();

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
