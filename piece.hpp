
#ifndef PIECE_HPP
    #define PIECE_HPP
    
    #include <cstdint>


    using namespace std;


    class cPiece{
        public:
            uint8_t piece;
            uint8_t src_x;
            uint8_t src_y;

            cPiece(uint8_t piece, uint8_t src_x, uint8_t src_y);
    };

#endif