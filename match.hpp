

  #ifndef MATCH_HPP

    #define MATCH_HPP


    #include <vector>
    #include <iomanip>
    #include <unistd.h>
    #include <algorithm> 
    #include "./board.hpp"
    #include "./move.hpp"
    #include "./piece.hpp"
    #include "./values.hpp"


    using namespace std;


    class cMatch{

      public:
        int32_t m_score = 0;

        cBoard m_board;

        vector<cMove> m_minutes;
        
        static const uint8_t STATUS_OPEN = 0;
        static const uint8_t STATUS_WINNER_WHITE = 1;
        static const uint8_t STATUS_WINNER_BLACK = 2;
        static const uint8_t STATUS_DRAW = 3;

        static const uint8_t STAGE_OPENING     =  10;
        static const uint8_t STAGE_MIDDLE      =  50;
        static const uint8_t STAGE_ENDGAME     =  90;

        cMatch();

        // copy constructor
        cMatch(const cMatch &match);

        ~cMatch();

        cMove *get_last_move();

        void do_move(const cMove &move);

        bool undo_move();

        uint8_t next_color() const;

        bool is_king_attacked() const;

        uint8_t eval_status();

        static string format_moves(const vector<cMove> &moves, const bool ext);

        static void prnt_fmttime(string msg, int seconds);

        void calc_move(int32_t &rscore, vector<cMove> &rmoves);

        void calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage);
        
        bool is_move_valid(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece);

      private:
        uint8_t eval_stage() const;

        void append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, const cMove &move);

        bool is_three_times_repetition(const cMove &move, uint8_t depth) const;

        bool is_search_continue(const cMove &move, const uint8_t depth, const uint8_t maxdepth, const uint8_t stage);

        void start_alphabeta_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage);

        bool is_fifty_moves_rule() const;

        int32_t score_terminate(uint8_t depth) const;

        static bool sortByPrio(const cMove &a, const cMove &b);

        static bool sortByRank(const cPiece &a, const cPiece &b);

    };

  #endif
