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
            uint8_t fields[8][8] = { { mWRK, mWKN, mWBP, mWQU, mWKG, mWBP, mWKN, mWRK },
                                     { mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW, mWPW },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK, mBLK },
                                     { mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW, mBPW }, 
                                     { mBRK, mBKN, mBBP, mBQU, mBKG, mBBP, mBKN, mBRK } };
            uint8_t wKg_x;
            uint8_t wKg_y;
            uint8_t bKg_x;
            uint8_t bKg_y;
            uint8_t wKg_has_moved_at;
            uint8_t bKg_has_moved_at;
            uint8_t wRkA_has_moved_at;
            uint8_t wRkH_has_moved_at;
            uint8_t bRkA_has_moved_at;
            uint8_t bRkH_has_moved_at;

            cBoard();

            cBoard(const cBoard &board);
            // copy constructor

            uint8_t getfield(uint8_t x, uint8_t y);

            void setfield(uint8_t x, uint8_t y, uint8_t value);

            static bool is_inbounds(uint8_t x, uint8_t y);

            uint8_t search_dir_for_piece(uint8_t &dst_x, uint8_t &dst_y, uint8_t src_x, uint8_t src_y, int8_t step_x, int8_t step_y, uint8_t maxcnt);

            bool search_for_touching_piece(uint8_t src_x, uint8_t src_y, uint8_t color);

            void search_for_touching_pieces(vector<cPiece> &pieces, uint8_t src_x, uint8_t src_y, uint8_t color, bool touching_only);

            void piece_search_for_touched_pieces(vector<cPiece> &pieces, uint8_t piece, uint8_t piece_x, uint8_t piece_y, uint8_t color, uint8_t excl_dir);

            static uint8_t eval_dir(uint8_t src_x1, uint8_t src_y1, uint8_t src_x2, uint8_t src_y2);
            
            uint8_t eval_pindir(uint8_t src_x, uint8_t src_y, uint8_t color);

            void prnt();

            bool debug_compare_fields(uint8_t fields[8][8]);

            void debug_copy_fields(uint8_t newfields[8][8]);

            bool debug_check_flags();

    };

#endif