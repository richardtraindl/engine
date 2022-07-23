
#ifndef VALUES_HPP
    #define VALUES_HPP

    #include <cstdint>
    #include <map>
    #include <string>


    using namespace std;


    // pieces
    #if !defined(mBLK)
        #define mBLK    0x0
    #endif

    #if !defined(mWPW)
        #define mWPW    0x1
    #endif

    #if !defined(mWKN)
        #define mWKN    0x2
    #endif

    #if !defined(mWBP)
        #define mWBP    0x3
    #endif

    #if !defined(mWRK)
        #define mWRK    0x4
    #endif

    #if !defined(mWQU)
        #define mWQU    0x5
    #endif

    #if !defined(mWKG)
        #define mWKG    0x6
    #endif

    #if !defined(mBPW)
        #define mBPW    0x9
    #endif

    #if !defined(mBKN)
        #define mBKN    0xA
    #endif

    #if !defined(mBBP)
        #define mBBP    0xB
    #endif

    #if !defined(mBRK)
        #define mBRK    0xC
    #endif

    #if !defined(mBQU)
        #define mBQU    0xD
    #endif

    #if !defined(mBKG)
        #define mBKG    0xE
    #endif

    // fake pieces for board evaluation
    #if !defined(mWPLUS1)
        #define mWPLUS1    0x10
    #endif

    #if !defined(mBPLUS1)
        #define mBPLUS1    0x11
    #endif

    #if !defined(mWPLUS2)
        #define mWPLUS2    0x12
    #endif

    #if !defined(mBPLUS2)
        #define mBPLUS2    0x13
    #endif

    #if !defined(mWPLUS3)
        #define mWPLUS3    0x14
    #endif

    #if !defined(mBPLUS3)
        #define mBPLUS3    0x15
    #endif

    #if !defined(mWPLUS4)
        #define mWPLUS4    0x16
    #endif

    #if !defined(mBPLUS4)
        #define mBPLUS4    0x17
    #endif

    // colors
    #if !defined(mBLANK)
        #define mBLANK  0x0
    #endif

    #if !defined(mWHITE)
        #define mWHITE  0x1
    #endif

    #if !defined(mBLACK)
        #define mBLACK  0x9
    #endif


    // dirs
    #if !defined(mNO_DIR)
        #define mNO_DIR         0x0
    #endif

    #if !defined(mEST_WST)
        #define mEST_WST        0x1
    #endif

    #if !defined(mSTH_NTH)
        #define mSTH_NTH        0x2
    #endif

    #if !defined(mSTHWST_NTHEST)
        #define mSTHWST_NTHEST  0x3
    #endif

    #if !defined(mSTHEST_NTHWST)
        #define mSTHEST_NTHWST  0x4
    #endif

    #if !defined(mKNIGHT_DIR)
        #define mKNIGHT_DIR     0x5
    #endif

    
    extern map<string, uint8_t> PIECES;

    extern map<string, uint8_t> COLORS;

    extern map<uint8_t, uint8_t> REVERSED_COLORS;

    extern map<uint8_t, uint8_t> PIECES_COLORS;

    extern map<string, uint8_t> DIRS;

    extern map<uint8_t, int32_t> SCORES;

    extern map<uint8_t, int32_t> REVERSED_SCORES;

    extern map<uint8_t, uint8_t> PIECES_RANKS;

    string reverse_lookup(const map<string, uint8_t> &dict, int value);

#endif
