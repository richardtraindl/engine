#ifndef FIELD_HPP
    #define FIELD_HPP
    
    #include <cstdint>
    #include "./board.hpp"


    using namespace std;


    class cField{
        private:
            const cBoard *m_board;
            uint8_t m_xpos;
            uint8_t m_ypos;
            uint8_t m_piece;
            uint8_t m_wtouchcnt;
            uint8_t m_btouchcnt;
            uint8_t m_wsoftpincnt;
            uint8_t m_bsoftpincnt;
            uint8_t m_wlowesttouch;
            uint8_t m_blowesttouch;

        public:
            cField(const cBoard *board, uint8_t xpos, uint8_t ypos);

            void fill();

            bool is_clear();

            bool is_only_wtouched();

            bool is_only_btouched();

            int8_t eval_score();

    };

#endif