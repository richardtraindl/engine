

  #ifndef EVALUATOR_HPP

    #define EVALUATOR_HPP


    #include "./match.hpp"
    #include "./board.hpp"
    #include "./move.hpp"


    using namespace std;


    class cEvaluator{

        public:
          cEvaluator();

          ~cEvaluator();

          static int32_t score_move(cMatch &match, const cMove &move);

          static int32_t score_board(cMatch &match, const uint8_t depth, const uint8_t stage);

          static int32_t score_touches_on_all_pieces(cMatch &match);
          
          static int8_t eval_piece_state(cMatch &match, const uint8_t piece, const uint8_t src_x, const uint8_t src_y);

          static uint8_t eval_field_state(cMatch &match, const uint8_t src_x, const uint8_t src_y);

          static bool is_hotspot(cMatch &match, const uint8_t src_x, const uint8_t src_y);

          static int32_t score_piece_state(cMatch &match, const uint8_t piece, const uint8_t src_x, const uint8_t src_y, const int8_t state);

          static bool has_passed_pawn(const cBoard &board, const uint8_t color);

          static bool is_wking_weak(cMatch &match);

          static bool is_bking_weak(cMatch &match);

          static bool is_wqu_wrk_on_7th_rank(uint8_t &x, uint8_t &y, const cBoard &board);

          static bool is_bqu_brk_on_2th_rank(uint8_t &x, uint8_t &y, const cBoard &board);

          static void priorize_move(cMatch &match, cMove &move);

        private:
          static bool is_field_save_for_color(const uint8_t field_state, const uint8_t color);

          static const cPiece *get_lowest(vector <cPiece> &pieces);

          static bool priorize_capture_move(cMatch &match, cMove &move, const uint8_t dstfield_state);

          static bool priorize_promotion_move(cMatch &match, cMove &move, const uint8_t dstfield_state);

          static bool priorize_castling_move(cMatch &match, cMove &move);

          static bool priorize_passed_pawn_move(cMatch &match, cMove &move, const uint8_t dstfield_state);

          static bool priorize_passed_pawn_supporting_move(cMatch &match, cMove &move, const uint8_t dstfield_state);

          static bool priorize_king_attacking_move(cMatch &match, cMove &move, const uint8_t dstfield_state);

  };

  #endif
