
#ifndef BOARD_HPP
    #define BOARD_HPP

    #include <map>
    #include <string>

    using namespace std;

    typedef map<string, int> MapStrInt;

    class cBoard{
        private:
            unsigned long long int first;
            unsigned long long int second;
            unsigned long long int third;
            unsigned long long int fourth
        public:
            int wKg;
            int bKg;
            int wKg_first_move_on;
            int bKg_first_move_on;
            int wRkA_first_move_on;
            int wRkH_first_move_on;
            int bRkA_first_move_on;
            int bRkH_first_move_on;

            const unsigned long long int BASE0  = 0b0000000000000000000000000000000000000000000000001111111111111111;
            const unsigned long long int BASE1  = 0b1001100100000000000000000000000000000000000000000000000010011001;
            const unsigned long long int BASE2  = 0x0110111000000000000000000000000000000000000000000000000001101110;
            const unsigned long long int BASE3  = 0x0011010011111111000000000000000000000000000000001111111100110100;
            const unsigned long long int FULL   = 0xFFFFFFFFFFFFFFFF;
            const unsigned long long int SINGLE = 0b1000000000000000000000000000000000000000000000000000000000000000;
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
        
            int getField(int idx);

            void setfield(int idx, int value);

    };

#endif
