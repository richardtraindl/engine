
#ifndef MATCH_HPP
    #define MATCH_HPP

    #include <list> 
    #include <vector> 
    #include <map>
    #include <string>
    #include <ctime>
    #include <iostream>
    #include "./board.hpp"
    #include "./move.hpp"

    using namespace std;

    class cMatch{
        public:
            time_t created_at;
            int status;
            int score;
            int level;
            cBoard board;
            list<cMove> minutes;

            static map<string, int> STATUS;
            static map<string, int> LEVELS;
            static map<int, int> SECS_PER_MOVE;
            static map<string, int> RETURN_CODES;

            cMatch();
            cMatch(const cMatch &obj);
            void update_attributes();
            int movecnt();
            int next_color();
            bool is_opening();
            bool is_endgame();
            bool is_fifty_moves_rule();
            bool is_move_repetition();
            cMove *do_move(int src, int dst, int prompiece);
            bool undo_move();
            int eval_status();
    };

#endif
