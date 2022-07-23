

    #include "./piece.hpp"


    cPiece::cPiece(uint8_t piece, uint8_t xpos, uint8_t ypos){ 
        m_piece = piece;
        m_xpos = xpos;
        m_ypos = ypos;
    }


    cTouchedPiece::cTouchedPiece(uint8_t piece, uint8_t xpos, uint8_t ypos, uint8_t dir) : cPiece(piece, xpos, ypos){
        m_dir = dir;
    }
