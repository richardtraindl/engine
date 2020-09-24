
#ifndef MATCH_HPP
    #define MATCH_HPP

    #include <ctime>
    #include <iostream>
    #include <fstream>
    #include <list> 
    #include <map>
    #include <string>
    #include "./helper.hpp"
    #include "./board.hpp"
    #include "./move.hpp"
    #include "./values.hpp"

    using namespace std;


    class cMatch{
        public:
            static map<string, int> STATUS;
            static map<string, int> LEVELS;
            static map<int, int> SECS_PER_MOVE;
            static map<string, int> RETURN_CODES;

            time_t created_at;
            int16_t score;
            uint8_t level;
            cBoard board;
            list<cMove> minutes;

            cMatch();

            cMatch(const cMatch &match);
            // copy constructor
            
            ~cMatch();

            int next_color();

            bool is_opening();

            bool is_endgame();

            bool is_fifty_moves_rule();

            bool is_three_times_rep();

            void do_move(uint64_t src, uint64_t dst, uint8_t prompiece);

            bool undo_move();
            
            // int eval_status();

            void prnt_minutes();

            void import(string filename);

            /*
            bool tst_wpw_move(uint64_t pos, uint64_t newpos);

            bool tst_bpw_move(uint64_t pos, uint64_t newpos);

            bool tst_wkg_move(uint64_t pos, uint64_t newpos);

            bool tst_bkg_move(uint64_t pos, uint64_t newpos);

            void gen_moves(uint8_t color); */
    };

#endif
