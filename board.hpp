
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
            uint8_t dir;
            bool rightshift;
            uint8_t shiftcnt;
            uint8_t stepcnt;
            uint64_t border;

            cStep(uint8_t newowner,
                  uint8_t newdir, 
                  bool newrightshift, 
                  uint8_t newshiftcnt, 
                  uint8_t newstepcnt, 
                  uint64_t newborder);

            cStep();
    };


    class cLink{
        public:
            uint64_t posA;
            uint8_t pieceA;
            uint64_t posB;
            uint8_t pieceB;
            uint8_t dirAB;

            cLink(uint64_t newposA, 
                  uint8_t newpieceA, 
                  uint64_t newposB, 
                  uint8_t newpieceB,
                  uint8_t newdirAB);

            cLink();
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
                cStep(STEP_OWNER["rook"], mEST, true, 1, 7, mEST_BORDER),
                cStep(STEP_OWNER["rook"], mWST, false, 1, 7, mWST_BORDER),
                cStep(STEP_OWNER["rook"], mNTH, true, 8, 7, mNTH_BORDER),
                cStep(STEP_OWNER["rook"], mSTH, false, 8, 7, mSTH_BORDER) 
            };

            const cStep bp_steps[4] = { 
                cStep(STEP_OWNER["bishop"], mNTH_EST, true, 9, 7, mNTH_EST_BORDER),
                cStep(STEP_OWNER["bishop"], mSTH_WST, false, 9, 7, mSTH_WST_BORDER),
                cStep(STEP_OWNER["bishop"], mNTH_WST, true, 7, 7, mNTH_WST_BORDER),
                cStep(STEP_OWNER["bishop"], mSTH_EST, false, 7, 7, mSTH_EST_BORDER)
            };

            const cStep qu_steps[8] = { 
                rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3], 
                bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3] 
            };

            const cStep kg_steps_generic[8] = { 
                cStep(STEP_OWNER["king"], mEST, true, 1, 1, mEST_BORDER),
                cStep(STEP_OWNER["king"], mWST, false, 1, 1, mWST_BORDER),
                cStep(STEP_OWNER["king"], mNTH, true, 8, 1, mNTH_BORDER),
                cStep(STEP_OWNER["king"], mSTH, false, 8, 1, mSTH_BORDER), 
                cStep(STEP_OWNER["king"], mNTH_EST, true, 9, 1, mNTH_EST_BORDER),
                cStep(STEP_OWNER["king"], mSTH_WST, false, 9, 1, mSTH_WST_BORDER),
                cStep(STEP_OWNER["king"], mNTH_WST, true, 7, 1, mNTH_WST_BORDER),
                cStep(STEP_OWNER["king"], mSTH_EST, false, 7, 1, mSTH_EST_BORDER) 
            };

            const cStep wkg_steps_and_castl[10] = { 
                kg_steps_generic[0], kg_steps_generic[1], 
                kg_steps_generic[2], kg_steps_generic[3], 
                kg_steps_generic[4], kg_steps_generic[5], 
                kg_steps_generic[6], kg_steps_generic[7],
                cStep(STEP_OWNER["king"], m2EST, true, 2, 1, mWHITE_CASTL_BORDER),
                cStep(STEP_OWNER["king"], m2WST, false, 2, 1, mWHITE_CASTL_BORDER) 
            };

            const cStep bkg_steps_and_castl[10] = { 
                kg_steps_generic[0], kg_steps_generic[1], 
                kg_steps_generic[2], kg_steps_generic[3], 
                kg_steps_generic[4], kg_steps_generic[5], 
                kg_steps_generic[6], kg_steps_generic[7],
                cStep(STEP_OWNER["king"], m2EST, true, 2, 1, mBLACK_CASTL_BORDER),
                cStep(STEP_OWNER["king"], m2WST, false, 2, 1, mBLACK_CASTL_BORDER) 
            };

            const cStep kn_steps[8] = { 
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, true, 6, 1, mNTH1_WST2_BORDER),
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, true, 15, 1, mNTH2_WST1_BORDER),
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, true, 17, 1, mNTH2_EST1_BORDER),
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, true, 10, 1, mNTH1_EST2_BORDER), 
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, false, 6, 1, mSTH1_EST2_BORDER),
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, false, 15, 1, mSTH2_EST1_BORDER),
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, false, 17, 1, mSTH2_WST1_BORDER),
                cStep(STEP_OWNER["knight"], mKNIGHT_DIRS, false, 10, 1, mSTH1_WST2_BORDER) 
            };

            const cStep wpw_steps_attack[2] = { 
                cStep(STEP_OWNER["wpawn"], mNTH_EST, true, 9, 1, mNTH_EST_BORDER),
                cStep(STEP_OWNER["wpawn"], mNTH_WST, true, 7, 1, mNTH_WST_BORDER) 
            };

            const cStep wpw_steps[3] = { 
                cStep(wpw_steps_attack[0]),
                cStep(wpw_steps_attack[1]),
                cStep(STEP_OWNER["wpawn"], mNTH, true, 8, 2, mNTH_BORDER) 
            };

            const cStep bpw_steps_attack[2] = {
                cStep(STEP_OWNER["bpawn"], mSTH_WST, false, 9, 1, mSTH_WST_BORDER),
                cStep(STEP_OWNER["bpawn"], mSTH_EST, false, 7, 1, mSTH_EST_BORDER) 
            };

            const cStep bpw_steps[3] = { 
                cStep(bpw_steps_attack[0]),
                cStep(bpw_steps_attack[1]),
                cStep(STEP_OWNER["bpawn"], mSTH, false, 8, 2, mSTH_BORDER) 
            };

            const cStep steps_for_pin_search[8] = { 
                rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
                bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3]
            };

            const cStep steps_for_white_enemies_search[27] = { 
                rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
                bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
                kg_steps_generic[0], kg_steps_generic[1],
                kg_steps_generic[2], kg_steps_generic[3],
                kg_steps_generic[4], kg_steps_generic[5],
                kg_steps_generic[6], kg_steps_generic[7],
                kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
                kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
                wpw_steps_attack[0], wpw_steps_attack[1], wpw_steps_attack[2]
            };

            const cStep steps_for_black_enemies_search[27] = { 
                rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
                bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
                kg_steps_generic[0], kg_steps_generic[1],
                kg_steps_generic[2], kg_steps_generic[3],
                kg_steps_generic[4], kg_steps_generic[5],
                kg_steps_generic[6], kg_steps_generic[7],
                kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
                kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
                bpw_steps_attack[0], bpw_steps_attack[1], bpw_steps_attack[2]
            };

 
            const cStep steps_for_search_white_checks[19] = { 
                rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
                bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
                kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
                kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
                wpw_steps_attack[0], wpw_steps_attack[1], wpw_steps_attack[2]
            };


            const cStep steps_for_search_black_checks[19] = { 
                rk_steps[0], rk_steps[1], rk_steps[2], rk_steps[3],
                bp_steps[0], bp_steps[1], bp_steps[2], bp_steps[3],
                kn_steps[0], kn_steps[1], kn_steps[2], kn_steps[3],
                kn_steps[4], kn_steps[5], kn_steps[6], kn_steps[7],
                bpw_steps_attack[0], bpw_steps_attack[1], bpw_steps_attack[2]
            };
 
  
            cBoard();

            cBoard(const cBoard &board);
            // copy constructor

            uint8_t read(uint64_t pos);

            void write(uint64_t pos, uint8_t piece);

            bool is_square_blank(uint64_t pos);

            bool is_square_white_occupied(uint64_t pos);

            bool is_square_black_occupied(uint64_t pos);
 
            uint64_t read_wkg_pos();

            uint64_t read_bkg_pos();

            static bool is_piece_blank(uint8_t piece);
 
            static bool is_piece_white(uint8_t piece);

            static bool is_piece_black(uint8_t piece);

            bool verify();
 
            void eval_count_of_officers(int &wofficers, int &bofficers);

            bool compare(cBoard &newboard);

            void gen_moves(list<cMove> &minutes);

            void prnt();

            void prnt_pos(uint64_t pos);
 
        private:
            bool is_square_enemy_touched(uint64_t pos);

            bool tst_en_passant(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_wpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_bpw_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_castling(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_kg_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            cPin *determine_pins(uint8_t color);

            void determine_checks(uint8_t color, list<cLink *> &attackers);

            void gen_kg_moves(list<cMove> &minutes);

            void gen_kg_support_moves(list<cMove> &minutes);

    };

#endif
