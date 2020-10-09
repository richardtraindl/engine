
#ifndef GENERATOR_HPP
    #define GENERATOR_HPP

    #include <map>
    #include <string>
    #include <cstdint>
    #include <list> 
    #include "./helper.hpp"
    #include "./match.hpp"
    #include "./move.hpp"
    #include "./values.hpp"


    class cGenerator{
        public:
            cMatch *match;
            
            cGenerator(cMatch *match);
            
            static map<string, uint8_t> SUPPORT;

            void gen_moves(list<cGMove> &moves);

        private:
            void gen_kg_moves(uint8_t color, list<cGMove> &moves);

            void gen_kg_support_moves(list<cLink *> &attackers, list<cGMove> &moves);

            void add_pw_moves(uint64_t src, uint64_t dst, list<cGMove> &moves);

            void score_move_presort(cGMove &move);

            void set_score_for_capture_move(cGMove &move, uint8_t dst_support, bool exchange);

            void set_score_for_supporting_move(cGMove &move, uint8_t dst_support, uint8_t level_for_supp_att);

            void set_score_for_attacking_move(cGMove &move, uint8_t dst_support, uint8_t level_for_supp_att);

            bool is_move_castling(cGMove &move);

            uint8_t determine_level_for_move_supportings_or_attackings(cGMove &move);

            uint8_t search_lowest(list<uint64_t> &touches);

            uint8_t determine_level_for_move_dstfield(cGMove &move, list<uint64_t> &white_touches_on_dst, list<uint64_t> &black_touches_on_dst);

            bool does_move_clear_for_supply(cGMove &move);

            bool is_move_forking(cGMove &move);

            bool is_move_defending_fork(cGMove &move);
 
            bool is_move_fleeing(cGMove &move);

            bool is_move_unpin(cGMove &move);

            bool is_move_blocking(cGMove &move);

            bool is_move_running_pawn(cGMove &move);

            bool is_move_tactical_draw(cGMove &move);
    };

#endif