
#ifndef BOARD_HPP
    #define BOARD_HPP

    #include <map>
    #include <string>
    #include <cstdint>
    #include "./move.hpp"
    #include "./values.hpp"

    using namespace std;

    class cBoard{
        public:
            static const int SNOK;
            static map<string, int> RANKS;
            static map<string, int> COLS;

            const static uint64_t BASE1        = 0x0402030506030204;
            const static uint64_t BASE2        = 0x0101010101010101;
            const static uint64_t BLANKS       = 0x0000000000000000;
            const static uint64_t BASE7        = 0x0909090909090909;
            const static uint64_t BASE8        = 0x0C0A0B0D0E0B0A0C;

            const static uint64_t FULL         = 0xFFFFFFFFFFFFFFFF;

            const static uint64_t SINGLE       = 0xF000000000000000;

            const static uint64_t POSMASK[];

            const static uint64_t BITPOSMASK[];

            const static uint64_t NEGMASK[];
          
            const static uint64_t BITS1000 = 0x8888888888888888;
            const static uint64_t BITS1110 = 0xEEEEEEEEEEEEEEEE;
            const static uint64_t BITS1100 = 0xCCCCCCCCCCCCCCCC;
            const static uint64_t BITS0111 = 0x7777777777777777;
            const static uint64_t BITS0011 = 0x3333333333333333;

            const string WHITE_TEXT  = "\033[97m";
            const string WHITE_BACK  = "\033[107m";
            const string YELLOW_TEXT  = "\033[93m";
            const string YELLOW_BACK  = "\033[103m";
            const string BLACK_TEXT  = "\033[30m";
            
            const string BLUE_BACK   = "\033[104m";
            const string CYAN_BACK   = "\033[106m";            
            const string MAGENTA_BACK = "\033[44m";
            const string GREEN_BACK  = "\033[42m";
            const string BOLD_ON     = "\033[1m";
            const string BOLD_OFF    = "\033[2m";
            const string MAGIC       = "\033[3m";
            const string RESET_ALL   = "\033[0m";

            uint8_t fields[64] = { mWRK, mWKN, mWBP, mWQU, mWKG, mWBP, mWKN, mWRK,
                                   mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, 
                                   mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, 
                                   mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, 
                                   mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, 
                                   mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, 
                                   mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, 
                                   mBRK, mBKN, mBBP, mBQU, mBKG, mBBP, mBKN, mBRK };
            int wKg;
            int bKg;
            int wKg_first_move_on;
            int bKg_first_move_on;
            int wRkA_first_move_on;
            int wRkH_first_move_on;
            int bRkA_first_move_on;
            int bRkH_first_move_on;

            cBoard();

            cBoard(const cBoard &board);
            // copy constructor

            int getfield(int idx);

            void setfield(int idx, int value);

            bool verify();
            
            void eval_count_of_officers(int &wofficers, int &bofficers);

            static bool is_inbounds_core(int src, int dst);

            static bool is_inbounds(int src, int dst, int step);

            int search(int src, int step, int maxcnt);

            bool search_for_two_pieces_within_dir(int &first, int &second, int src, int step, int maxcnt);

            bool search_bi_dirs(int &first, int &second, int src, int step, int maxcnt);

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

            bool is_king_after_move_attacked(int src, int dst, list<cMove> &minutes);

            bool is_move_valid(int src, int dst, int prompiece, list<cMove> &minutes);

            bool is_move_available(list<cMove> &minutes);

            void prnt();

            bool compare(cBoard &board2);

            void cpyfields_to_bigint(int startidx, int count, uint64_t &bigint);
    };

#endif
