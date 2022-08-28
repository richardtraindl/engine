

  #ifndef BOARD_HPP

    #define BOARD_HPP


    #include <vector>
    #include "./move.hpp"
    #include "./piece.hpp"
    #include "./values.hpp"


    using namespace std;


    class cBoard{

      public:
        uint8_t m_fields[8][8] = { 
            { mWRK, mWKN, mWBP, mWQU, mWKG, mWBP, mWKN, mWRK },
            { mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW },
            { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
            { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
            { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
            { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
            { mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW }, 
            { mBRK, mBKN, mBBP, mBQU, mBKG, mBBP, mBKN, mBRK } 
        };

        uint8_t m_wKg_x = 4;

        uint8_t m_wKg_y = 0;

        uint8_t m_bKg_x = 4;

        uint8_t m_bKg_y = 7;

        uint8_t m_wKg_has_moved_at = 0;

        uint8_t m_bKg_has_moved_at = 0;

        uint8_t m_wRkA_has_moved_at = 0;

        uint8_t m_wRkH_has_moved_at = 0;

        uint8_t m_bRkA_has_moved_at = 0;

        uint8_t m_bRkH_has_moved_at = 0;

        cBoard();

        // copy constructor
        cBoard(const cBoard &board);

        uint8_t getfield(uint8_t x, uint8_t y) const;

        void setfield(uint8_t x, uint8_t y, uint8_t value);

        void copy_fields(uint8_t newfields[8][8]) const;

        bool compare_fields(uint8_t fields[8][8]) const;

        static bool is_inbounds(const uint8_t x, const uint8_t y);

        void do_move_on_board_only(const cMove &move);

        static uint16_t eval_dir(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2);
        
        static uint16_t eval_cardinale(const uint8_t x1, const uint8_t y1, const uint8_t x2, const uint8_t y2);

        static bool eval_steps(int8_t &step_x, int8_t &step_y, const uint16_t dir);

        uint16_t eval_pindir(const uint8_t src_x, const uint8_t src_y) const;

        bool get_soft_pinned(vector<cPiece> &targets, const uint8_t src_x, const uint8_t src_y) const;

        bool is_soft_pinned(const uint8_t src_x, const uint8_t src_y) const;

        bool is_piece_behind_soft_pinned(const uint8_t src_x, const uint8_t src_y) const;

        bool is_field_touched(const uint8_t src_x, const uint8_t src_y, const uint8_t color) const;

        uint8_t search_dir(uint8_t &dst_x, uint8_t &dst_y, const uint8_t src_x, const uint8_t src_y, const int8_t step_x, const int8_t step_y, const uint8_t maxcnt) const;

        void search_cardinale(vector <cPiece> &targets, const uint8_t src_x, const uint8_t src_y, const uint16_t cardinale) const;

        void search_vertical_for_straight_pawns(vector<cPiece> &wpieces, vector<cPiece> &bpieces, const uint8_t src_x, const uint8_t src_y) const;

        void search_all_dirs_for_touching_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, const uint8_t src_x, const uint8_t src_y, const cMove *last_move, const bool observe_pins) const;

        void search_from_piece(vector<cPiece> &wpieces, vector<cPiece> &bpieces, const uint8_t piece, const uint8_t src_x, const uint8_t src_y, const bool observe_pins) const;

        bool is_passed_pawn(const uint8_t piece, const uint8_t src_x, const uint8_t src_y) const;

        void prnt() const;

        static void coord_to_indices(uint8_t &x, uint8_t &y, string coord);

        static string indices_to_coord(uint8_t x, uint8_t y);

    };

  #endif
