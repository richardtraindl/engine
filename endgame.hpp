#ifndef ENDGAME_HPP
    #define ENDGAME_HPP

    //#include <vector>
    #include "./values.hpp"
    #include "./piece.hpp"


    using namespace std;

    class cEndGame001{
        public:
            cEndGame001();

            static const int32_t m_100_white_army_WKG[8][8];

            static const int32_t m_100_white_army_BKG[8][8];

            static const int32_t m_100_black_army_BKG[8][8];

            static const int32_t m_100_black_army_WKG[8][8];

            static const int32_t m_101_BKG[8][8];

            static const int32_t m_102_BKG[8][8];

            static const int32_t m_103_BKG[8][8];

            static const int32_t m_104_BKG[8][8];

            static const int32_t m_111_BKG[8][8];
            
            static const int32_t m_112_BKG[8][8];
            
            static const int32_t m_113_BKG[8][8];
            
            static const int32_t m_114_BKG[8][8];

            static const int32_t m_120_final_black_army_WKG[8][8];

            static const int32_t m_130_final_black_army_WKG[8][8];

    };

#endif