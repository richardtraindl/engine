
#ifndef MOVE_HPP
    #define MOVE_HPP

    #include <cstdint>
    #include <string>
    #include "./helper.hpp"
    #include "./values.hpp"

    using namespace std;


    class cGMove{
        public:
            uint64_t src;
            uint64_t dst;
            uint8_t prompiece;
            uint8_t presort;

            static const uint8_t PRESORT_STEP;
            static const uint8_t PRESORT_STORMY;
            static const uint8_t PRESORT_HIGH;
            static const uint8_t PRESORT_MEDIUM;
            static const uint8_t PRESORT_LOW;

            cGMove(uint64_t src, uint64_t dst, uint8_t prompiece);
            cGMove();

            string format(bool core);
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
