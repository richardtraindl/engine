
#ifndef BOARD_HPP
    #define BOARD_HPP

    #include <map>
    #include <string>
    #include <cstdint>
    #include <list> 
    #include <iostream>
    #include  <iomanip>
    #include "./move.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"


    using namespace std;


    class cStep{
        public:
            uint8_t owner;
            bool rightshift;
            uint8_t shiftcnt;
            uint8_t stepcnt;
            uint64_t border;

            cStep(uint8_t newowner, bool newrightshift, uint8_t newshiftcnt, uint8_t newstepcnt, uint64_t newborder);

            cStep();
    };


    class cPin{
        public:
            uint64_t pins[5] = { 0x0, 0x0, 0x0, 0x0, 0x0 };

            cPin();

            bool is_pinned(uint64_t pos);
    };


    class cBoard{
        public:
            uint64_t field[8] = { 0b1111111111111111000000000000000000000000000000000000000000000000,
                                   0b0000000000000000000000000000000000000000000000001111111111111111,
                                   0b0000100000000000000000000000000000000000000000000000000000001000,
                                   0b0001000000000000000000000000000000000000000000000000000000010000,
                                   0b1000000100000000000000000000000000000000000000000000000010000001,
                                   0b0010010000000000000000000000000000000000000000000000000000100100,
                                   0b0100001000000000000000000000000000000000000000000000000001000010, 
                                   0b0000000011111111000000000000000000000000000000001111111100000000 };

            const cStep rk_steps[4] = { 
                cStep(STEP_OWNER["rook"], true, 1, 7, mEST_BORDER),
                cStep(STEP_OWNER["rook"], false, 1, 7, mWST_BORDER),
                cStep(STEP_OWNER["rook"], true, 8, 7, mNTH_BORDER),
                cStep(STEP_OWNER["rook"], false, 8, 7, mSTH_BORDER) 
            };

            const cStep bp_steps[4] = { 
                cStep(STEP_OWNER["bishop"], true, 9, 7, mNTH_EST_BORDER),
                cStep(STEP_OWNER["bishop"], false, 9, 7, mSTH_WST_BORDER),
                cStep(STEP_OWNER["bishop"], true, 7, 7, mNTH_WST_BORDER),
                cStep(STEP_OWNER["bishop"], false, 7, 7, mSTH_EST_BORDER) 
            };

            const cStep qu_steps[8] = { 
                rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3], 
                bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3] 
            };

            const cStep kg_steps_generic[8] = { 
                cStep(STEP_OWNER["king"], true, 1, 1, mEST_BORDER),
                cStep(STEP_OWNER["king"], false, 1, 1, mWST_BORDER),
                cStep(STEP_OWNER["king"], true, 8, 1, mNTH_BORDER),
                cStep(STEP_OWNER["king"], false, 8, 1, mSTH_BORDER), 
                cStep(STEP_OWNER["king"], true, 9, 1, mNTH_EST_BORDER),
                cStep(STEP_OWNER["king"], false, 9, 1, mSTH_WST_BORDER),
                cStep(STEP_OWNER["king"], true, 7, 1, mNTH_WST_BORDER),
                cStep(STEP_OWNER["king"], false, 7, 1, mSTH_EST_BORDER) 
            };

            const cStep wkg_steps_and_castl[10] = { 
                kg_steps_generic[0], kg_steps_generic[1], 
                kg_steps_generic[2], kg_steps_generic[3], 
                kg_steps_generic[4], kg_steps_generic[5], 
                kg_steps_generic[6], kg_steps_generic[7],
                cStep(STEP_OWNER["king"], true, 2, 1, mWHITE_CASTL_BORDER),
                cStep(STEP_OWNER["king"], false, 2, 1, mWHITE_CASTL_BORDER) 
            };

            const cStep bkg_steps_and_castl[10] = { 
                kg_steps_generic[0], kg_steps_generic[1], 
                kg_steps_generic[2], kg_steps_generic[3], 
                kg_steps_generic[4], kg_steps_generic[5], 
                kg_steps_generic[6], kg_steps_generic[7],
                cStep(STEP_OWNER["king"], true, 2, 1, mBLACK_CASTL_BORDER),
                cStep(STEP_OWNER["king"], false, 2, 1, mBLACK_CASTL_BORDER) 
            };

            const cStep kn_steps[8] = { 
                cStep(STEP_OWNER["knight"], true, 6, 1, mNTH1_WST2_BORDER),
                cStep(STEP_OWNER["knight"], true, 15, 1, mNTH2_WST1_BORDER),
                cStep(STEP_OWNER["knight"], true, 17, 1, mNTH2_EST1_BORDER),
                cStep(STEP_OWNER["knight"], true, 10, 1, mNTH1_EST2_BORDER), 
                cStep(STEP_OWNER["knight"], false, 6, 1, mSTH1_EST2_BORDER),
                cStep(STEP_OWNER["knight"], false, 15, 1, mSTH2_EST1_BORDER),
                cStep(STEP_OWNER["knight"], false, 17, 1, mSTH2_WST1_BORDER),
                cStep(STEP_OWNER["knight"], false, 10, 1, mSTH1_WST2_BORDER) 
            };

            const cStep wpw_steps_attack[2] = { 
                cStep(STEP_OWNER["wpawn"], true, 9, 1, mNTH_EST_BORDER),
                cStep(STEP_OWNER["wpawn"], true, 7, 1, mNTH_WST_BORDER) 
            };

            const cStep wpw_steps[3] = { 
                cStep(wpw_steps_attack[0]),
                cStep(wpw_steps_attack[1]),
                cStep(STEP_OWNER["wpawn"], true, 8, 2, mNTH_BORDER) 
            };

            const cStep bpw_steps_attack[2] = {
                cStep(STEP_OWNER["bpawn"], false, 9, 1, mSTH_WST_BORDER),
                cStep(STEP_OWNER["bpawn"], false, 7, 1, mSTH_EST_BORDER) 
            };

            const cStep bpw_steps[3] = { 
                cStep(bpw_steps_attack[0]),
                cStep(bpw_steps_attack[1]),
                cStep(STEP_OWNER["bpawn"], false, 8, 2, mSTH_BORDER) 
            };

            cBoard();

            cBoard(const cBoard &board);
            // copy constructor

            uint8_t read(uint64_t pos);

            void write(uint64_t pos, uint8_t piece);

            bool is_square_blank(uint64_t pos);

            bool is_square_white_busy(uint64_t pos);

            bool is_square_black_busy(uint64_t pos);
 
            uint64_t read_wk_pos();

            uint64_t read_bk_pos();

            static bool is_piece_blank(uint8_t piece);
 
            static bool is_piece_white(uint8_t piece);

            static bool is_piece_black(uint8_t piece);

            bool verify();
 
            void eval_count_of_officers(int &wofficers, int &bofficers);

            void prnt();

            bool compare(cBoard &newboard);
        
            void gen_moves(list<cMove> &minutes);
 
        private:
            bool is_square_enemy_touched(uint64_t pos);

            bool tst_en_passant(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_wpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_bpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_castling(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_kg_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            cPin *determine_pins(uint8_t color);
        
            void determine_checks(unint8_t color, uint64_t &fst_enemy, uint64_t &sec_enemy);
    };

#endif
