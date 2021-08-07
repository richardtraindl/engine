

    #include "./endgame.hpp"


    cEndGame001::cEndGame001(){
    }


    const cPiece cEndGame001::m_mate_with_wkn_wbp[][4] = { 
        { cPiece(mWKG, 5, 5), cPiece(mWKN, 3, 7), cPiece(mBKG, 7, 5), cPiece(mWBP, 4, 1) }, 
        { cPiece(mWKG, 5, 5), cPiece(mWKN, 5, 6), cPiece(mBKG, 5, 7), cPiece(mWBP, 7, 6) }, 
        { cPiece(mWKG, 4, 5), cPiece(mWKN, 3, 6), cPiece(mBKG, 2, 6), cPiece(mWBP, 7, 6) }, 
        { cPiece(mWKG, 4, 5), cPiece(mWKN, 3, 6), cPiece(mBKG, 2, 5), cPiece(mWBP, 3, 2) },
        { cPiece(mWKG, 4, 5), cPiece(mWKN, 3, 6), cPiece(mBKG, 2, 6), cPiece(mWBP, 4, 3) },
        { cPiece(mWKG, 3, 5), cPiece(mWKN, 3, 6), cPiece(mBKG, 4, 7), cPiece(mWBP, 3, 4) },
        { cPiece(mWKG, 3, 5), cPiece(mWKN, 1, 6), cPiece(mBKG, 3, 7), cPiece(mWBP, 5, 6) },
        { cPiece(mWKG, 1, 5), cPiece(mWKN, 1, 6), cPiece(mBKG, 2, 7), cPiece(mWBP, 4, 5) },
        { cPiece(mWKG, 1, 5), cPiece(mWKN, 0, 5), cPiece(mBKG, 1, 7), cPiece(mWBP, 7, 2) } 
    };
    

    const int32_t cEndGame001::m_white_WKG[8][8] = {
        { 0,  0,  0,  0,  0,  0,  0, 0 },
        { 0, 10, 10, 10, 10, 10, 10, 0 },
        { 0, 30, 30, 20, 20, 20, 20, 0 },
        { 0, 30, 20,  0,  0, 20, 20, 0 },
        { 0, 20, 20,  0,  0, 20, 30, 0 },
        { 0, 20, 20, 20, 20, 30, 30, 0 },
        { 0, 10, 10, 10, 10, 10, 10, 0 },
        { 0,  0,  0,  0,  0,  0,  0, 0 }
    };


    const int32_t cEndGame001::m_white_BKG[8][8] = {
        { 99, 98, 97, 96, 95, 94, 93, 92 },
        { 98, 98, 97,  0,  0, 94, 93, 93 },
        { 97, 97,  0,  0,  0,  0, 94, 94 },
        { 96,  0,  0,  0,  0,  0,  0, 95 },
        { 95,  0,  0,  0,  0,  0,  0, 96 },
        { 94, 94,  0,  0,  0,  0, 97, 97 },
        { 93, 93, 94,  0,  0, 97, 98, 99 },
        { 92, 93, 94, 95, 96, 97, 98, 99 }
    };
