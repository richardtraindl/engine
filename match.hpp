
#ifndef MATCH_HPP
    #define MATCH_HPP

    #include <list> 
    #include <map>
    #include <string>
    #include <ctime>
    #include "./board.hpp"
    #include "./move.hpp"

    using namespace std;


    class cMatch{
    // private:
        public:
            time_t created_at;
            int status;
            int score;
            int level;
            cBoard board;
            list<cMove> minutes;

            map<string, int> STATUS = { 
                {"active", 10}, 
                {"draw", 11}, 
                {"winner-white", 12}, 
                {"winner-black", 13}
            };

            map<string, int> LEVELS = {
                {"blitz", 15}, 
                {"low", 30}, 
                {"medium", 60}, 
                {"high", 90}
            };

            map<int, int> SECS_PER_MOVE = {
                {LEVELS["blitz"], 15}, 
                {LEVELS["low"], 30}, 
                {LEVELS["medium"], 60}, 
                {LEVELS["high"], 90}
            };

            map<string, int> EVAL_MODES = {
                {"ignore-pins", 0}, 
                {"only-pins-to-king", 1}, 
                {"all-pins", 2}
            };

            map<string, int> RETURN_CODES = {
                {"ok", 10}, 
                {"draw", 11}, 
                {"winner-white", 12}, 
                {"winner-black", 13}, 
                {"match-cancelled", 14}, 
                {"wrong-color", 15}, 
                {"piece-error", 16},
                {"king-attacked-error", 17}, 
                {"format-error", 18}, 
                {"out-of-bounds", 19},
                {"general-error", 20}
            };

            cMatch();

            void update_attributes();

            int color_of_piece(int piece);

            int oppcolor_of_piece(int piece);

            int movecnt();

            int next_color();

            bool is_opening();

            bool is_endgame();  
    };
#endif
