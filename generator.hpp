
    #define GENERATOR_HPP


    #include "./match.hpp"
    #include "./board.hpp"
    #include "./move.hpp"


    using namespace std;


    class cGenerator{

        public:
          cGenerator();

          ~cGenerator();

          static void gen_moves(cMatch &match, vector<cMove> &moves, uint8_t color);

        private:
          static void add_wpw_moves(cMatch &match, vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece);

          static void gen_wpw_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y);

          static void add_bpw_moves(cMatch &match, vector<cMove> &moves, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y, uint8_t dstpiece);

          static void gen_bpw_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y);

          static void gen_kn_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t knight);

          static void gen_qrb_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t piece, char _switch);

          static void gen_kg_moves(cMatch &match, vector<cMove> &moves, uint8_t x, uint8_t y, uint8_t king);

          static void gen_wkg_castling_moves(cMatch &match, vector<cMove> &moves,  uint8_t kg_x, uint8_t kg_y);

          static void gen_bkg_castling_moves(cMatch &match, vector<cMove> &moves, uint8_t kg_x, uint8_t kg_y);

          static void gen_kg_supporting_moves(cMatch &match, vector<cMove> &moves, const cPiece &attacker); 

          //static void set_prio_for_move(cMatch &match, cMove &move);

          //static void set_prio_for_castling(cMatch &match, cMove &move);

    };

  #endif
