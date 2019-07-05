
#ifndef BOARD_HPP
    #define BOARD_HPP

    #include <map>
    #include <string>

    using namespace std;

    typedef map<string, int> MapStrInt;

    class cBoard{
        public:
            int fields[8][8];
            int wKg_x;
            int wKg_y;
            int bKg_x;
            int bKg_y;
            int wKg_first_move_on;
            int bKg_first_move_on;
            int wRkA_first_move_on;
            int wRkH_first_move_on;
            int bRkA_first_move_on;
            int bRkH_first_move_on;

            const int BASE     = 0xF; // 0x42356324111111110000000000000000000000000000000099999999CABDEBAC;
            const int FULL     = 0xF; // 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;
            const int SINGLE   = 0xF; // 0xF000000000000000000000000000000000000000000000000000000000000000;
            const int BITS1000 = 0xF; // 0x8888888888888888888888888888888888888888888888888888888888888888;
            const int BITS1110 = 0xF; // 0xEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE;
            const int BITS1100 = 0xF; // 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC;
            const int BITS0111 = 0xF; // 0x7777777777777777777777777777777777777777777777777777777777777777;
            const int BITS0011 = 0xF; // 0x3333333333333333333333333333333333333333333333333333333333333333; */

            map<string, int>RANKS = {
                {"1", 0},
                {"2", 1},
                {"3", 2},
                {"4", 3},
                {"5", 4},
                {"6", 5},
                {"7", 6},
                {"8", 7}
            };

            map<string, int>COLS = {
                {"A", 0},
                {"B", 1},
                {"C", 2},
                {"D", 3},
                {"E", 4},
                {"F", 5},
                {"G", 6},
                {"H", 7}
            };

            cBoard();

    };

#endif