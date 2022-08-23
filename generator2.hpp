
  #ifndef GENERATOR2_HPP

    #define GENERATOR2_HPP

      #include "./match.hpp"
      #include "./board.hpp"
      #include "./move.hpp"

      using namespace std;

      class cGenerator2{

        public:
          cGenerator2(cMatch &match);

          ~cGenerator2();

          cMove *gen_move();

        private:
          cMatch *m_matchptr;

          uint8_t m_check_cnt;

          cPiece *m_king_attackerptr;
          
          uint8_t m_gen_x;

          uint8_t m_gen_y;

          uint8_t m_dir_idx;

          uint8_t m_step_idx;

          uint8_t m_prom_idx;

          bool m_king_move_remaining;

          bool m_king_move_has_started;

          bool m_capture_of_checking_remaining;

          bool m_capture_of_checking_has_startet;

          bool m_blocking_of_checking_remaining;

          bool m_blocking_of_checking_has_started;

          bool m_gen_end;

          cMove *m_postponedptr;

          cMove *_gen_move();

          void incr_gen(const uint8_t max_prom_cnt, const uint8_t max_step_cnt, const uint8_t max_dir_cnt);

          cMove *gen_wpw_move(const uint8_t pawn);

          cMove *_create_wpw_move(const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t dstpiece);

          cMove *gen_bpw_move(const uint8_t pawn);

          cMove *_create_bpw_move(const uint8_t src_x, const uint8_t src_y, const uint8_t dst_x, const uint8_t dst_y, const uint8_t dstpiece);

          void incr_gen(const uint8_t max_step_idx, const uint8_t max_dir_idx);

          cMove *gen_kn_move(const uint8_t knight);

          cMove *gen_long_range_move(const uint8_t piece);

          cMove *gen_kg_move(const uint8_t king, const bool isattacked);

          cMove *_gen_wkg_sh_castl_move();

          cMove *_gen_wkg_lg_castl_move();

          cMove *_gen_bkg_sh_castl_move();

          cMove *_gen_bkg_lg_castl_move();

          cMove *gen_capture_move_checking_piece();

          cMove *gen_blocking_move_checking_piece();

    };

  #endif