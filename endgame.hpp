#ifndef ENDGAME_HPP
    #define ENDGAME_HPP

    #include "./values.hpp"
    #include "./piece.hpp"
    #include "./bitboard.hpp"


    using namespace std;


    class cEndGame100{
        public:
            static const int32_t m_single_bKG[8][8];

            static const uint64_t m_prolong_filter[8][4];

            cEndGame100();

    };


    class cEndGame110{
        public:
            static const int32_t m_single_bKG[8][8];

            static const uint64_t m_prolong_filter[8][4];

            cEndGame110();

    };


    class cEndGame120{
        public:
            static const uint64_t m_prolong_filter[8][4];

            cEndGame120();

    };


    class cEndGame130{
        public:
            static const uint64_t m_prolong_filter[8][4];

            cEndGame130();

    };


#endif