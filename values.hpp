
#ifndef VALUES_HPP
    #define VALUES_HPP

    #include <cstdint>
    #include <map>
    #include <string>

    using namespace std;


    // colors
    #if !defined(mBLK)
        #define mBLK    0b0
    #endif

    #if !defined(mWHITE)
        #define mWHITE  0b10000000
    #endif

    #if !defined(mBLACK)
        #define mBLACK  0b01000000
    #endif


    // pieces
    #if !defined(mWPW)
        #define mWPW    (mWHITE | 0b00000001)
    #endif

    #if !defined(mBPW)
        #define mBPW    (mBLACK | 0b00000001)
    #endif

    #if !defined(mWKN)
        #define mWKN    (mWHITE | 0b00000010)
    #endif

    #if !defined(mBKN)
        #define mBKN    (mBLACK | 0b00000010)
    #endif

    #if !defined(mWBP)
        #define mWBP    (mWHITE | 0b00000100)
    #endif

    #if !defined(mBBP)
        #define mBBP    (mBLACK | 0b00000100)
    #endif

    #if !defined(mWRK)
        #define mWRK    (mWHITE | 0b00001000)
    #endif

    #if !defined(mBRK)
        #define mBRK    (mBLACK | 0b00001000)
    #endif

    #if !defined(mWQU)
        #define mWQU    (mWHITE | 0b00010000)
    #endif

    #if !defined(mBQU)
        #define mBQU    (mBLACK | 0b00010000)
    #endif

    #if !defined(mWKG)
        #define mWKG    (mWHITE | 0b00100000)
    #endif

    #if !defined(mBKG)
        #define mBKG    (mBLACK | 0b00100000)
    #endif


    // field indices
    #if !defined(mIDX_WHITE)
        #define mIDX_WHITE    0
    #endif

    #if !defined(mIDX_BLACK)
        #define mIDX_BLACK   1
    #endif

    #if !defined(mIDX_KING)
        #define mIDX_KING    2
    #endif

    #if !defined(mIDX_QUEEN)
        #define mIDX_QUEEN    3
    #endif

    #if !defined(mIDX_ROOK)
        #define mIDX_ROOK    4
    #endif

    #if !defined(mIDX_BISHOP)
        #define mIDX_BISHOP    5
    #endif

    #if !defined(mIDX_KNIGHT)
        #define mIDX_KNIGHT    6
    #endif

    #if !defined(mIDX_PAWN)
        #define mIDX_PAWN       7    
    #endif


    // pin indices
    #if !defined(mIDX_PIN)
        #define mIDX_PIN                0
    #endif

    #if !defined(mIDX_WST_EST)
        #define mIDX_WST_EST            1
    #endif

    #if !defined(mIDX_STH_NTH)
        #define mIDX_STH_NTH            2
    #endif

    #if !defined(mIDX_STH_WST_NTH_EST)
        #define mIDX_STH_WST_NTH_EST    3
    #endif

    #if !defined(mIDX_STH_EST_NTH_WST)
        #define mIDX_STH_EST_NTH_WST    4
    #endif


    // Borders
    #if !defined(mEST_BORDER)
        #define mEST_BORDER             0x0101010101010101
    #endif

    #if !defined(mWST_BORDER)
        #define mWST_BORDER             0x8080808080808080
    #endif

    #if !defined(mNTH_BORDER)
        #define mNTH_BORDER             0x00000000000000FF
    #endif

    #if !defined(mSTH_BORDER)
        #define mSTH_BORDER             0xFF00000000000000
    #endif

    #if !defined(mNTH_EST_BORDER)
        #define mNTH_EST_BORDER         0x01010101010101FF
    #endif

    #if !defined(mNTH_WST_BORDER)
        #define mNTH_WST_BORDER         0x80808080808080FF
    #endif

    #if !defined(mSTH_EST_BORDER)
        #define mSTH_EST_BORDER         0xFF01010101010101
    #endif

    #if !defined(mSTH_WST_BORDER)
        #define mSTH_WST_BORDER         0xFF80808080808080
    #endif

    #if !defined(mNTH1_WST2_BORDER)
        #define mNTH1_WST2_BORDER       0xC0C0C0C0C0C0C0FF
    #endif

    #if !defined(mNTH2_WST1_BORDER)
        #define mNTH2_WST1_BORDER       0x808080808080FFFF
    #endif

    #if !defined(mNTH1_EST2_BORDER)
        #define mNTH1_EST2_BORDER       0x03030303030303FF
    #endif
 
    #if !defined(mNTH2_EST1_BORDER)
        #define mNTH2_EST1_BORDER       0x010101010101FFFF
    #endif

    #if !defined(mSTH1_EST2_BORDER)
        #define mSTH1_EST2_BORDER       0xFF00030303030303
    #endif

    #if !defined(mSTH2_EST1_BORDER)
        #define mSTH2_EST1_BORDER       0xFFFF010101010101
    #endif

    #if !defined(mSTH1_WST2_BORDER)
        #define mSTH1_WST2_BORDER       0xFFC0C0C0C0C0C0C0
    #endif

    #if !defined(mSTH2_WST1_BORDER)
        #define mSTH2_WST1_BORDER       0xFFFF808080808080
    #endif
    
    #if !defined(mWHITE_CASTL_BORDER)
        #define mWHITE_CASTL_BORDER     0xF700000000000000
    #endif

    #if !defined(mBLACK_CASTL_BORDER)
        #define mBLACK_CASTL_BORDER     0x00000000000000F7
    #endif


    // useful Bitmasks
    #if !defined(mPOS_A1)
        #define mPOS_A1                 0x8000000000000000
    #endif


    // terminal colors
    #if !defined(mWHITE_TEXT)
        #define mWHITE_TEXT     string("\033[97m")
    #endif

    #if !defined(mWHITE_BACK)
        #define mWHITE_BACK     string("\033[107m")
    #endif

    #if !defined(mYELLOW_TEXT)
        #define mYELLOW_TEXT    string("\033[93m")
    #endif

    #if !defined(mYELLOW_BACK)
        #define mYELLOW_BACK    string("\033[103m")
    #endif

    #if !defined(mBLACK_TEXT)
        #define mBLACK_TEXT     string("\033[30m")
    #endif
            
    #if !defined(mBLUE_BACK)
        #define mBLUE_BACK      string("\033[104m")
    #endif

    #if !defined(mCYAN_BACK)
        #define mCYAN_BACK      string("\033[106m")
    #endif

    #if !defined(mMAGENTA_BACK)
        #define mMAGENTA_BACK   string("\033[44m")
    #endif

    #if !defined(mGREEN_BACK)
        #define mGREEN_BACK     string("\033[42m")
    #endif

    #if !defined(mBOLD_ON)
        #define mBOLD_ON        string("\033[1m")
    #endif

    #if !defined(mBOLD_OFF)
        #define mBOLD_OFF       string("\033[2m")
    #endif

    #if !defined(mMAGIC)
        #define mMAGIC          string("\033[3m")
    #endif

    #if !defined(mRESET_ALL)
        #define mRESET_ALL      string("\033[0m")
    #endif


    extern map<string, uint8_t> PIECES;

    extern map<uint8_t, string> PIECES_STR;

    extern map<uint8_t, uint8_t> PIECES_RANKS;

    extern map<uint8_t, int16_t> SCORES;

    extern map<uint8_t, int16_t> REVERSED_SCORES;

    extern map<string, uint8_t> STEP_OWNER;

    extern map<string, uint8_t> MOVE_TYPE;
#endif
