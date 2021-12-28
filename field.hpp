#ifndef FIELD_HPP
    #define FIELD_HPP
    
    #include <cstdint>
    #include "./board.hpp"


    using namespace std;


    class cField{
        private:
            const cBoard *m_board;
            uint8_t m_pos_x;
            uint8_t m_pos_y;
            uint8_t m_piece;
            uint8_t m_wtouchcnt;
            uint8_t m_btouchcnt;
            uint8_t m_wsoftpincnt;
            uint8_t m_bsoftpincnt;
            uint8_t m_wlowesttouch;
            uint8_t m_blowesttouch;

        public:
            cField(const cBoard *board, uint8_t pos_x, uint8_t pos_y);

            void fill();

            bool is_clear();

            bool is_only_wtouched();

            bool is_only_btouched();

            int8_t eval_advant_score();

    };

#endif