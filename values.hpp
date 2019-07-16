
#ifndef VALUES_HPP
    #define VALUES_HPP

    #include <map>
    #include <string>

    using namespace std;

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

    extern map<string, int> PIECES;

    extern map<int, int> PIECES_BARE;

    extern map<int, int> PIECES_RANK;

    extern map<int, int> PIECES_MASK;

    extern map<int, int> PIECES_NMASK;

    extern map<string, int> COLORS;

    extern map<int, int> REVERSED_COLORS;

    extern map<int, int> PIECES_COLORS;

    extern map<string, int> DIRS;

    extern map<int, int> REVERSE_DIRS;

    extern map<int, int> DIRS_FOR_STEP;

    extern map<int, int> SCORES;

    extern map<int, int> SUPPORTED_SCORES;

    extern map<int, int> ATTACKED_SCORES;
    
    extern map<string, int> EVAL_MODES;
#endif
