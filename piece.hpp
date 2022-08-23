


    #ifndef PIECE_HPP

      #define PIECE_HPP


      #include <cstdint>
      #include <array>
      #include "./values.hpp"


      using namespace std;


      class cDir{

        public:
          uint16_t m_dir;

          uint16_t m_cardinale;

          int8_t m_xstep;

          int8_t m_ystep;

          cDir(uint16_t dir, uint16_t cardinale, int8_t xstep, int8_t ystep);

      };


      class cPiece{

        public:
          uint8_t m_piece;

          uint8_t m_xpos;

          uint8_t m_ypos;

          cPiece(uint8_t piece, uint8_t xpos, uint8_t ypos);

          cPiece(const cPiece &piece);

          static cDir all_steps[20];

          static cDir rk_steps[4];

          static cDir bp_steps[4];

          static cDir qu_steps[8];

          static cDir kn_steps[8];

          static cDir wpw_steps[2];

          static cDir wpw_capture_steps[2];

          static cDir bpw_steps[2];

          static cDir bpw_capture_steps[2];
          
          static void get_range(uint8_t range[3], uint8_t piece);

          static bool is_touching_dir(uint8_t piece, uint16_t dir, uint8_t src_x, uint8_t src_y, uint8_t dst_x, uint8_t dst_y);

          string format() const;

      };

    #endif
