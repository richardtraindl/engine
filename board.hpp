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
            uint8_t m_fields[8][8] = { { mWRK, mWKN, mWBP, mWQU, mWKG, mWBP, mWKN, mWRK },
                                     { mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW }, 
                                     { mBRK, mBKN, mBBP, mBQU, mBKG, mBBP, mBKN, mBRK } };
            uint8_t m_wKg_x;
            uint8_t m_wKg_y;
            uint8_t m_bKg_x;
            uint8_t m_bKg_y;
            uint8_t m_wKg_has_moved_at;
            uint8_t m_bKg_has_moved_at;
            uint8_t m_wRkA_has_moved_at;
            uint8_t m_wRkH_has_moved_at;
            uint8_t m_bRkA_has_moved_at;
            uint8_t m_bRkH_has_moved_at;

            static const uint8_t SQUARE_UNDEF = 0;
            static const uint8_t SQUARE_STH_EST = 1;
            static const uint8_t SQUARE_STH_WST = 2;
            static const uint8_t SQUARE_NTH_EST = 3;
            static const uint8_t SQUARE_NTH_WST = 4;

            static const uint8_t ENDGAME_STAT_0 =     0;
            static const uint8_t ENDGAME_STAT_100 = 100;
            static const uint8_t ENDGAME_STAT_110 = 110;
            static const uint8_t ENDGAME_STAT_120 = 120;
            static const uint8_t ENDGAME_STAT_130 = 130;

            cBoard();

            cBoard(const cBoard &board);
            // copy constructor

            uint8_t getfield(uint8_t x, uint8_t y);

            void setfield(uint8_t x, uint8_t y, uint8_t value);

            static bool is_inbounds(uint8_t x, uint8_t y);

            static uint8_t max_diff(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

            static uint8_t diff_to_margin(uint8_t x, uint8_t y);

            static bool is_margin_pos(uint8_t x, uint8_t y);

            static bool is_opposition(uint8_t wkg_x, uint8_t wkg_y, uint8_t bkg_x, uint8_t bkg_y);

            uint8_t kings_square_pos();

            bool is_within_two_squares(uint8_t piece, uint8_t src_x, uint8_t src_y);

            bool is_exit_closed(uint8_t status);

            bool is_running_pawn(uint8_t piece, uint8_t src_x, uint8_t src_y);

            uint8_t search_dir_for_piece(uint8_t &dst_x, uint8_t &dst_y, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y, uint8_t maxcnt);

            void search_dir_for_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y);

            bool search_for_touching_piece(uint8_t src_x, uint8_t src_y, uint8_t color);

            void search_for_touching_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, uint8_t color, bool touching_only);

            void search_for_all_touching_pieces(vector<cPiece> &wpieces, vector<cPiece> &bpieces, uint8_t src_x, uint8_t src_y);

            void search_for_touched_pieces(vector<cPiece> &pieces, uint8_t piece, uint8_t piece_x, uint8_t piece_y, uint8_t color, uint8_t excl_dir);

            static uint8_t eval_dir(uint8_t src_x1, uint8_t src_y1, uint8_t src_x2, uint8_t src_y2);
            
            uint8_t eval_pindir(uint8_t src_x, uint8_t src_y, uint8_t color);

            bool is_piece_soft_pinned(uint8_t piece, uint8_t piece_x, uint8_t piece_y);

            bool compare_fields(uint8_t fields[8][8]);

            void prnt();

            void copy_fields(uint8_t newfields[8][8]);

            bool debug_check_flags();

    };

#endif