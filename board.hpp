
#ifndef BOARD_HPP
    #define BOARD_HPP

    #include <map>
    #include <string>
    #include <boost/multiprecision/cpp_int.hpp>
    #include "./move.hpp"

    using namespace boost::multiprecision;
    using namespace boost::multiprecision::literals;
    using namespace std;

    class cBoard{
        public:
            static map<string, int> RANKS;
            static map<string, int> COLS;
            const uint256_t BASE     = 0x42356324111111110000000000000000000000000000000099999999CABDEBAC_cppui;
            const uint256_t FULL     = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_cppui;
            const uint256_t SINGLE   = 0xF000000000000000000000000000000000000000000000000000000000000000_cppui;
            const uint256_t BITS1000 = 0x8888888888888888888888888888888888888888888888888888888888888888_cppui;
            const uint256_t BITS1110 = 0xEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE_cppui;
            const uint256_t BITS1100 = 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC_cppui;
            const uint256_t BITS0111 = 0x7777777777777777777777777777777777777777777777777777777777777777_cppui;
            const uint256_t BITS0011 = 0x3333333333333333333333333333333333333333333333333333333333333333_cppui;

            uint256_t fields;
            int wKg;
            int bKg;
            int wKg_first_move_on;
            int bKg_first_move_on;
            int wRkA_first_move_on;
            int wRkH_first_move_on;
            int bRkA_first_move_on;
            int bRkH_first_move_on;

            cBoard();

            int getfield(int idx);

            void setfield(int idx, int value);

            void copyfields(uint256_t *fields);

            bool comparefields(uint256_t fields);

            bool verify();

            static bool is_inbounds_core(int src, int dst);

            static bool is_inbounds(int src, int dst, int step);

            int search(int src, int step, int maxcnt);

            bool search_bi_dirs(int *first, int *second, int src, int step, int maxcnt);

            static bool is_nth(int src, int dst);
            static bool is_sth(int src, int dst);
            static bool is_est(int src, int dst);
            static bool is_wst(int src, int dst);
            static bool is_nth_est(int src, int dst);
            static bool is_sth_wst(int src, int dst);
            static bool is_nth_wst(int src, int dst);
            static bool is_sth_est(int src, int dst);

            int eval_pin_dir(int src);

            int eval_soft_pin_dir(int src);

            bool is_king_after_move_attacked(int src, int dst, list<cMove> *minutes);

            bool is_move_valid(int src, int dst, int prompiece, list<cMove> *minutes);

            bool is_move_available(list<cMove> *minutes);

            void prnt();
    };

#endif
