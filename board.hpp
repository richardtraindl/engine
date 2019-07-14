
#ifndef BOARD_HPP
    #define BOARD_HPP

    #include <map>
    #include <string>
    #include <boost/multiprecision/cpp_int.hpp>
    #include "./move.hpp"
    #include "./values.hpp"
    #include "./helper.hpp"
    
    using namespace boost::multiprecision;
    using namespace boost::multiprecision::literals;
    using namespace std;

    class cBoard{
        public:
            uint256_t fields;
            int wKg;
            int bKg;
            int wKg_first_move_on;
            int bKg_first_move_on;
            int wRkA_first_move_on;
            int wRkH_first_move_on;
            int bRkA_first_move_on;
            int bRkH_first_move_on;

            static map<string, unsigned> RANKS;
            static map<string, unsigned> COLS;
            const uint256_t BASE     = 0x42356324111111110000000000000000000000000000000099999999CABDEBAC_cppui;
            const uint256_t FULL     = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_cppui;
            const uint256_t SINGLE   = 0xF000000000000000000000000000000000000000000000000000000000000000_cppui;
            const uint256_t BITS1000 = 0x8888888888888888888888888888888888888888888888888888888888888888_cppui;
            const uint256_t BITS1110 = 0xEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE_cppui;
            const uint256_t BITS1100 = 0xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC_cppui;
            const uint256_t BITS0111 = 0x7777777777777777777777777777777777777777777777777777777777777777_cppui;
            const uint256_t BITS0011 = 0x3333333333333333333333333333333333333333333333333333333333333333_cppui;

            cBoard();
            unsigned getfield(unsigned idx);
            void setfield(unsigned idx, unsigned value);
            void copyfields(uint256_t *fields);
            bool comparefields(uint256_t fields);
            bool verify();
            static bool is_inbounds_core(unsigned src, unsigned dst);
            static bool is_inbounds(unsigned src, unsigned dst, int step);
            unsigned search(unsigned src, unsigned step, int maxcnt);
            bool search_bi_dirs(unsigned *first, unsigned *second, unsigned src, int step, int maxcnt);
            static bool is_nth(unsigned src, unsigned dst);
            static bool is_sth(unsigned src, unsigned dst);
            static bool is_est(unsigned src, unsigned dst);
            static bool is_wst(unsigned src, unsigned dst);
            static bool is_nth_est(unsigned src, unsigned dst);
            static bool is_sth_wst(unsigned src, unsigned dst);
            static bool is_nth_wst(unsigned src, unsigned dst);
            static bool is_sth_est(unsigned src, unsigned dst);
            unsigned eval_pin_dir(unsigned src);
            unsigned eval_soft_pin_dir(unsigned src);
            bool is_king_after_move_attacked(unsigned src, unsigned dst, list<cMove> *minutes);
            bool is_move_valid(unsigned src, unsigned dst, unsigned prompiece, list<cMove> *minutes);
            bool is_move_available(list<cMove> *minutes);
    };

#endif
