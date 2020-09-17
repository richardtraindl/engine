
#ifndef MOVE_HPP
    #define MOVE_HPP

    //#include "./board.hpp"
    #include <cstdint>
    #include <string>

    using namespace std;


    class cGMove{
        public:
            uint64_t src;
            uint64_t dst;
            uint8_t prompiece;

            cGMove(uint64_t src, uint64_t dst, uint8_t prompiece);
            cGMove();

            string format();
    };


    class cMove{
        public:
            uint8_t type;
            uint64_t src;
            uint64_t dst;
            uint8_t prompiece;
            uint64_t prev_field[8];

            cMove(uint8_t type, uint64_t src, uint64_t dst, uint8_t prompiece, uint64_t *prev_field);
            cMove();

            string format();
    };

#endif
