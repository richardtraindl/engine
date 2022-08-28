
  #ifndef MOVE_HPP

    #define MOVE_HPP


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

            static const uint8_t P_TOP                 =   0;
            static const uint8_t P_BOTTOM              = 250;
            static const uint8_t P_PROMOTION           =   5;
            static const uint8_t P_CAPTURE             =  10;
            static const uint8_t P_SAC_FOR_SUPPLY      =  30;
            static const uint8_t P_TOUCH_WEAK_PIECE    =  31;
            static const uint8_t P_ESCAPE              =  32;
            static const uint8_t P_CHECK               =  40;
            static const uint8_t P_CASTLING            =  41;
            static const uint8_t P_PASSED_PAWN         =  50;
            static const uint8_t P_PASSED_PAWN_SUPPORT =  51;
            static const uint8_t A_QUITE_GOOD          =  10;
            static const uint8_t A_HAZY                =  35;
            static const uint8_t A_BAD                 =  70;

            cMove(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t srcpiece, uint8_t dstpiece, uint8_t prompiece, uint8_t prio);

            cMove();

            cMove(const cMove &move);
            // copy constructor

            ~cMove();

            bool is_capture() const;

            bool is_en_passant() const;

            bool is_promotion() const;

            bool is_short_castling() const;

            bool is_long_castling() const;

            void create_fake_rook_move(cMove &rookmove) const;

            bool compare(const cMove &move) const;

            string format(bool ext) const;

    };

  #endif
