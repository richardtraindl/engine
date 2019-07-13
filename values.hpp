
#ifndef VALUES_HPP
    #define VALUES_HPP

    #include <map>
    #include <string>

    using namespace std;

    typedef map<string, int> MapStrInt;
    typedef map<int, int> MapIntInt;
    typedef map<int, string> MapIntStr;

    #if !defined(mBLK)
        #define mBLK    0x0 // extern constexpr int BPW;
    #endif
    #if !defined(mWPW)
        #define mWPW    0x1 // extern constexpr int BPW;
    #endif
    #if !defined(mWKN)
        #define mWKN    0x2 // extern constexpr int BPW;
    #endif
    #if !defined(mWBP)
        #define mWBP    0x3 // extern constexpr int BPW;
    #endif
    #if !defined(mWRK)
        #define mWRK    0x4 // extern constexpr int BPW;
    #endif
    #if !defined(mWQU)
        #define mWQU    0x5 // extern constexpr int BPW;
    #endif
    #if !defined(mWKG)
        #define mWKG    0x6 // extern constexpr int BPW;
    #endif

    #if !defined(mBPW)
        #define mBPW    0x9 // extern constexpr int BPW;
    #endif
    #if !defined(mBKN)
        #define mBKN    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mBBP)
        #define mBBP    0xB // extern constexpr int BPW;
    #endif
    #if !defined(mBRK)
        #define mBRK    0xC // extern constexpr int BPW;
    #endif
    #if !defined(mBQU)
        #define mBQU    0xD // extern constexpr int BPW;
    #endif
    #if !defined(mBKG)
        #define mBKG    0xE // extern constexpr int BPW;
    #endif

    extern map<string, unsigned> PIECES;

    extern map<unsigned, unsigned> PIECES_BARE;

    extern map<unsigned, unsigned> PIECES_RANK;

    extern MapIntInt PIECES_MASK;

    extern MapIntInt PIECES_NMASK;

    extern map<string, unsigned> COLORS;

    extern map<unsigned, unsigned> REVERSED_COLORS;

    extern map<unsigned, unsigned> PIECES_COLOR;

    extern map<string, unsigned> DIRS;

    extern map<unsigned, unsigned> REVERSE_DIRS;

    extern map<int, unsigned> DIR_FOR_STEP;

    extern map<unsigned, int> SCORES;

    extern map<unsigned, int> SUPPORTED_SCORES;

    extern map<unsigned, int> ATTACKED_SCORES;
    
    extern map<string, unsigned> EVAL_MODES;
#endif
