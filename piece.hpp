
#ifndef PIECE_HPP
    #define PIECE_HPP
    
    #include <cstdint>


    using namespace std;


    class cPiece{
        public:
            uint8_t m_piece;
            uint8_t m_xpos;
            uint8_t m_ypos;

            cPiece(uint8_t piece, uint8_t xpos, uint8_t ypos);

    };


    class cTouchedPiece : public cPiece{
        public:
            uint8_t m_dir;
        
            cTouchedPiece(uint8_t piece, uint8_t xpos, uint8_t ypos, uint8_t dir);

    };

#endif