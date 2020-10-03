
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

            bool is_move_castling(cGMove &move);

            void score_move_presort(cGMove &move);

            void set_score_for_capture_move(cGMove &move, uint8_t dst_support, bool exchange);

            void set_score_for_supporting_move(cGMove &move, uint8_t dst_support, bool strong_supp_or_att);

            void set_score_for_attacking_move(cGMove &move, uint8_t dst_support, bool strong_supp_or_att);

            bool is_move_strong_supporting_or_attacking(cGMove &move);

            bool is_move_forking(cGMove &move);

            bool is_move_defending_fork(cGMove &move);
 
            bool is_move_fleeing(cGMove &move);

            bool is_move_unpin(cGMove &move);

            bool is_move_blocking(cGMove &move);

            bool is_move_running_pawn(cGMove &move);

            bool is_move_tactical_draw(cGMove &move);
    };

#endif