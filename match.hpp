#ifndef MATCH_HPP
    #define MATCH_HPP

    #include <cstdint>
    #include <unistd.h>
    #include <ctime>
    #include <iomanip>  
    #include <vector>
    #include <algorithm> 
    #include "./values.hpp"
    #include "./board.hpp"
    #include "./move.hpp"
    #include "./endgame.hpp"
    #include "./bitboard.hpp"


    using namespace std;


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

            cMatch();

            cMatch(const cMatch &match);
            // copy constructor

            ~cMatch();
            
            void reset();

            uint8_t next_color();

            bool is_king_attacked(uint8_t attacker_color);

            bool is_move_valid(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece);

            uint8_t eval_status();

            void do_usr_move(uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t prompiece);

            void do_move(cMove &move);

            bool undo_move();

            bool is_three_times_repetition(cMove &move, uint8_t depth);

            bool is_fifty_moves_rule();

            static string fmt_moves(vector<cMove> &moves);

            void prnt_moves(vector<cMove> &moves);

            static void prnt_fmttime(string msg, int seconds);

            void prnt_minutes();

            // calculation
            void calc_move(int32_t &rscore, vector<cMove> &rmoves, uint8_t maxdepth);

            void calc_alphabeta(int32_t &rscore, vector<cMove> &rmoves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta);

            void calc_alphabeta_endgame(int32_t &rscore, vector<cMove> &rmoves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta, uint8_t status);

            void start_alphabeta_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta);

            void start_alphabeta_endgame_threads(int32_t &rscore, vector<cMove> &rmoves, vector<cMove> &moves, uint8_t depth, uint8_t maxdepth, int32_t alpha, int32_t beta, uint8_t status);

            static bool sortByPrio(const cMove &a, const cMove &b);

            static bool sortByEngame_100(const cMove &a, const cMove &b);

            bool does_move_touch_weak_piece(cMove &move);

            bool is_good_field_for_piece(cPiece piece, uint8_t dst_x, uint8_t dst_y);

            bool filter(cMove &move, uint8_t depth, uint8_t maxdepth);

            bool filter_endgame(cMove &move, uint8_t depth, uint8_t maxdepth, uint8_t status);

            void append_newmove(vector<cMove> &rcandidates, const vector<cMove> &newcandidates, cMove &move);

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

            void eval_prio(cMove &move);

            bool is_opening();

            bool is_endgame(uint8_t &status);

            int32_t eval_terminate(uint8_t depth);

            int32_t eval_board(cMove &move);

            int32_t eval_endgame_board(bool &islib, cMove &move, uint8_t depth, uint8_t status);

    };

#endif
