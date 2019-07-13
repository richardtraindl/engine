
#ifndef MATCH_HPP
    #define MATCH_HPP

    #include <list> 
    #include <map>
    #include <string>
    #include <ctime>
    #include "./board.hpp"
    #include "./move.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"
    #include "./pieces/pieceshelper.hpp"

    using namespace std;


    class cMatch{
        public:
            time_t created_at;
            int status;
            int score;
            int level;
            cBoard board;
            list<cMove> minutes;

            static map<string, unsigned> STATUS;
            static map<string, unsigned> LEVELS;
            static map<int, unsigned> SECS_PER_MOVE;
            static map<string, unsigned> RETURN_CODES;

            cMatch();
            cMatch(const cMatch &obj);
            void update_attributes();
            int movecnt();
            int next_color();
            bool is_opening();
            bool is_endgame();
            bool is_fifty_moves_rule();
            int is_move_repetition();
    };
#endif
