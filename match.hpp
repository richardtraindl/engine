#ifndef MATCH_HPP
    #define MATCH_HPP

    #include <cstdint>
    #include <unistd.h>
    #include <ctime>
    #include <iomanip>  
    #include <vector>
    #include <algorithm> 
    #include "./board.hpp"
    #include "./move.hpp"
    #include "./field.hpp"
    #include "./endgame.hpp"
    #include "./bitboard.hpp"


    using namespace std;
    //#define DEBUG

    class cMatch{
        public:
            int32_t m_score = 0;

            cBoard m_board;

            vector<cMove> m_minutes;

            vector<cMove> m_prev_search;

            static const uint8_t STATUS_OPEN = 0;
            static const uint8_t STATUS_WINNER_WHITE = 1;
            static const uint8_t STATUS_WINNER_BLACK = 2;
            static const uint8_t STATUS_DRAW = 3;

            static const uint8_t STAGE_OPENING     =  10;
            static const uint8_t STAGE_MIDDLE      =  50;
            static const uint8_t STAGE_ENDGAME     =  90;
            static const uint8_t STAGE_ENDGAME_100 = 100;
            static const uint8_t STAGE_ENDGAME_110 = 110;
            static const uint8_t STAGE_ENDGAME_120 = 120;
            static const uint8_t STAGE_ENDGAME_130 = 130;

            cMatch();

            cMatch(const cMatch &match);
            // copy constructor

            ~cMatch();
            
            void reset();

            uint8_t next_color() const;

            bool is_move_valid(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece);

            uint8_t eval_status();

            void prnt_status(uint8_t status) const;

            void do_usr_move(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece);

            void do_move(const cMove &move);

            bool undo_move();

            bool is_three_times_repetition(const cMove &move, uint8_t depth) const;

            bool is_fifty_moves_rule() const;

            static string fmt_moves(vector<cMove> &moves);

            static void prnt_moves(vector<cMove> &moves);

            static void prnt_fmttime(string msg, int seconds);

            void prnt_minutes() const;

            // calculation
            void calc_move(int32_t &rscore, vector<cMove> &rmoves);

            #ifdef DEBUG
              void calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage, const uint8_t mvidx);
            #else
              void calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage);
            #endif

            void start_alphabeta_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, const uint8_t depth, const uint8_t maxdepth, int32_t alpha, int32_t beta, const uint8_t stage);

            static bool sortByPrio(const cMove &a, const cMove &b);

            static bool sortByEngame_100(const cMove &a, const cMove &b);

            bool does_move_touch_weak_piece(const cMove &move) const;

            bool does_move_sac_for_supply(const cMove &move) const;

            bool eval_move(const cMove &move, const uint8_t depth, const uint8_t maxdepth, const uint8_t stage);

            void append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, const cMove &move);

            void gen_moves(vector<cMove> &moves, uint8_t color);

            void add_wpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece);

            void gen_wpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y);

            void add_bpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece);

            void gen_bpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y);

            void gen_kn_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t knight);

            void gen_qrb_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t piece, char _switch);

            void gen_kg_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king);

            void gen_wkg_castling_moves(vector<cMove> &moves, uint8_t x, uint8_t y);

            void gen_bkg_castling_moves(vector<cMove> &moves, uint8_t x, uint8_t y);

            void gen_kg_supporting_moves(vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y, uint8_t attacking_piece, uint8_t attacking_x, uint8_t attacking_y, uint8_t color);

            void set_prio_for_move(cMove &move);

            void set_prio_for_castling(cMove &move);

            uint8_t eval_stage() const;

            int32_t score_terminate(uint8_t depth) const;

            bool is_stormy() const;

            int32_t score_board(const cMove &move, const uint8_t depth, const uint8_t stage) const;

            int32_t score_opening_middlegame_board(const cMove &move, const uint8_t stage) const;

            int32_t score_endgame_board(const cMove &move, const uint8_t depth, const uint8_t status) const;

    };

#endif
