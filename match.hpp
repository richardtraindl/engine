
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

            static map<string, int> STATUS;
            static map<string, int> LEVELS;
            static map<int, int> SECS_PER_MOVE;
            static map<string, int> EVAL_MODES;
            static map<string, int> RETURN_CODES;

            cMatch();
            cMatch(const cMatch &obj);

            void update_attributes();

            int color_of_piece(int piece);

            int oppcolor_of_piece(int piece);

            int movecnt();

            int next_color();

            bool is_opening();

            bool is_endgame();

            bool is_fifty_moves_rule();

            int is_move_repetition();

    };
#endif
