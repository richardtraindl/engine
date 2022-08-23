

  #ifndef GENERATOR_HPP

    #define GENERATOR_HPP


    #include "./match.hpp"
    #include "./board.hpp"
    #include "./move.hpp"


    using namespace std;


    class cGenerator{

        public:
          cMatch *m_matchptr;

          cGenerator(cMatch &match);

          ~cGenerator();

          void gen_moves(vector<cMove> &moves, uint8_t color);

        private:
          void add_wpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece);

          void gen_wpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y);

          void add_bpw_moves(vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece);

          void gen_bpw_moves(vector<cMove> &moves, uint8_t x, uint8_t y);

          void gen_kn_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t knight);

          void gen_qrb_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t piece, char _switch);

          void gen_kg_moves(vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king);

          void gen_wkg_castling_moves(vector<cMove> &moves,  uint8_t kg_x, uint8_t kg_y);

          void gen_bkg_castling_moves(vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y);

          void gen_capture_checking_piece_moves(vector<cMove> &moves, const cPiece &attacker); 

          void gen_blocking_checking_piece_moves(vector<cMove> &moves, const cPiece &attacker);

    };

  #endif
