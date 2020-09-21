
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

            const cStep step_for_dir(uint8_t dir);

            uint8_t reverse_dir(uint8_t dir);
    };


    class cPin{
        public:
            uint64_t pins[5] = { 0x0, 0x0, 0x0, 0x0, 0x0 };

            cPin();

            bool is_pinned(uint64_t pos, uint8_t dir);
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

            static const cStep rk_steps[];

            static const cStep bp_steps[];

            static const cStep qu_steps[];

            static const cStep kg_steps_generic[];

            static const cStep wkg_steps_and_castl[];

            static const cStep bkg_steps_and_castl[];

            static const cStep kn_steps[];

            static const cStep wpw_steps_attack[];
            
            static const cStep wpw_steps_attack_search[];

            static const cStep wpw_steps_support_search[];

            static const cStep wpw_steps[];

            static const cStep bpw_steps_attack[];

            static const cStep bpw_steps_attack_search[];

            static const cStep bpw_steps_support_search[];

            static const cStep bpw_steps[];

            static const cStep steps_for_pin_search[];

            static const cStep steps_for_white_enemies_search[];

            static const cStep steps_for_black_enemies_search[];
 
            static const cStep steps_for_white_checks_search[];

            static const cStep steps_for_black_checks_search[];

            static const cStep steps_for_wkg_support_search[];

            static const cStep steps_for_bkg_support_search[];

            cBoard();

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

            void gen_moves(list<cMove> &minutes, list<cGMove> &moves);

            void prnt();

            void prnt_16hex(uint64_t pos);
 
        private:
            bool is_square_enemy_touched(uint64_t pos);

            bool tst_wpw_move(uint64_t pos, uint64_t newpos);

            bool tst_bpw_move(uint64_t pos, uint64_t newpos);

            bool tst_en_passant(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_kg_move(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            bool tst_castling(uint64_t pos, uint64_t newpos, list<cMove> &minutes);

            cPin *determine_pins(uint8_t color);

            void determine_checks(uint8_t color, list<cLink *> &attackers);

            void gen_kg_moves(uint8_t color, list<cGMove> &moves);

            void gen_kg_support_moves(list<cLink *> &attackers, list<cGMove> &moves);

            void add_pw_moves(uint64_t src, uint64_t dst, list<cGMove> &moves);
    };

#endif
