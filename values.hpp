
#ifndef VALUES_HPP
    #define VALUES_HPP

    #include <map>
    #include <string>

    using namespace std;

    typedef map<string, int> MapStrInt;
    typedef map<int, int> MapIntInt;
    typedef map<int, string> MapIntStr;

    #if !defined(mBLK)
        #define mBLK    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mWPW)
        #define mWPW    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mWKN)
        #define mWKN    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mWBP)
        #define mWBP    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mWRK)
        #define mWRK    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mWQU)
        #define mWQU    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mWKG)
        #define mWKG    0xA // extern constexpr int BPW;
    #endif

    #if !defined(mBPW)
        #define mBPW    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mBKN)
        #define mBKN    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mBBP)
        #define mBBP    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mBRK)
        #define mBRK    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mBQU)
        #define mBQU    0xA // extern constexpr int BPW;
    #endif
    #if !defined(mBKG)
        #define mBKG    0xA // extern constexpr int BPW;
    #endif

    extern MapStrInt PIECES;

    extern MapIntStr STR_PIECES;

    extern MapIntInt PIECES_BARE;

    extern MapIntInt PIECES_RANK;

    extern MapIntInt PIECES_MASK;

    extern MapIntInt PIECES_NMASK;

    extern MapStrInt COLORS;

    extern MapIntInt REVERSED_COLORS;

    extern MapIntInt PIECES_COLOR;

    extern MapStrInt DIRS;

    extern MapIntInt REVERSE_DIRS;

    extern MapIntInt DIR_FOR_STEP;

    extern MapIntInt SCORES;

    extern MapIntInt SUPPORTED_SCORES;

    extern MapIntInt ATTACKED_SCORES;
#endif
