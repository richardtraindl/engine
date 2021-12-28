#ifndef BOARD_HPP
    #define BOARD_HPP

    #include <iostream>
    #include <cstdint>
    #include <vector>
    #include "./values.hpp"
    #include "./piece.hpp"


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

            static const uint8_t ENDGAME_STAT_0 =     0;
            static const uint8_t ENDGAME_STAT_100 = 100;
            static const uint8_t ENDGAME_STAT_110 = 110;
            static const uint8_t ENDGAME_STAT_120 = 120;
            static const uint8_t ENDGAME_STAT_130 = 130;

            static const uint8_t PINNED_NO = 0;
            static const uint8_t PINNED_SOFT = 1;
            static const uint8_t PINNED_HARD = 2;

            cBoard();

            // copy constructor
            cBoard(const cBoard &board);

            uint8_t getfield(uint8_t x, uint8_t y) const;

            void setfield(uint8_t x, uint8_t y, uint8_t value);

            static bool is_inbounds(uint8_t x, uint8_t y);

            bool is_king_attacked(uint8_t attacker_color) const;

            static uint8_t max_diff(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

            static uint8_t diff_to_margin(uint8_t x, uint8_t y);

            static bool is_margin_frame_pos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

            static bool is_margin_frame_ypos(uint8_t y1, uint8_t y2);

            static bool is_corner_pos(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

            bool is_within_two_squares(uint8_t piece, uint8_t src_x, uint8_t src_y) const;

            bool is_running_pawn(uint8_t piece, uint8_t src_x, uint8_t src_y) const;

            uint8_t search_dir_for_piece(uint8_t &dst_x, uint8_t &dst_y, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y, uint8_t maxcnt) const;

            void search_dir_for_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y) const;

            bool search_for_touching_piece(uint8_t src_x, uint8_t src_y, uint8_t color) const;

            void search_for_touching_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, uint8_t color, bool touching_only) const;

            void search_for_all_touching_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, uint8_t src_x, uint8_t src_y) const;

            void search_for_touched_pieces(vector<cPiece> &pieces, uint8_t piece, uint8_t piece_x, uint8_t piece_y, uint8_t color, uint8_t excl_dir) const;

            static uint8_t eval_dir(uint8_t src_x1, uint8_t src_y1, uint8_t src_x2, uint8_t src_y2);
            
            uint8_t eval_pindir(uint8_t src_x, uint8_t src_y, uint8_t color) const;

            uint8_t eval_pin_state(uint8_t piece, uint8_t x, uint8_t y) const;

            int8_t eval_field_state(uint8_t x, uint8_t y) const;

            bool compare_fields(uint8_t fields[8][8]) const;

            void prnt() const;

            void copy_fields(uint8_t newfields[8][8]) const;

            bool debug_check_flags() const;

    };

#endif