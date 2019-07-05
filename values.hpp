
#ifndef VALUES_HPP
    #define VALUES_HPP

    #include <map>
    #include <string>

    using namespace std;

    typedef map<string, int> MapStrInt;
    typedef map<int, int> MapIntInt;
    typedef map<int, string> MapIntStr;

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
