
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


    static map<string, uint8_t> SUPPORT;

    void gen_moves(cMatch &match, list<cGMove *> &moves);

    void gen_kg_moves(uint8_t color, cMatch &match, list<cGMove *> &moves);

    void gen_kg_support_moves(cMatch &match, list<cLink *> &attackers, list<cGMove *> &moves);

    void add_pw_moves(cMatch &match, uint64_t src, uint64_t dst, list<cGMove *> &moves);

    void score_move_presort(cMatch &match, cGMove &move);

    bool does_capture_eliminate_supporter_attacker(cMatch &match, cGMove &move);

    void set_score_for_capture_move(cMatch &match, cGMove &move, uint8_t dst_support);

    void set_score_for_supporting_attacking_move(cGMove &move, uint8_t level_for_supp_att);

    bool is_move_castling(cGMove &move);

    uint8_t search_lowest(cMatch &match, list<uint64_t> &touches);

    uint8_t determine_level_for_move_supportings_or_attackings(cMatch &match, cGMove &move);

    uint8_t determine_level_for_move_dstfield(cMatch &match, cGMove &move, list<uint64_t> &white_touches_on_dst, list<uint64_t> &black_touches_on_dst);

    bool has_piece_double_attack_in_dir(cMatch &match, uint64_t pos, uint8_t dir);

    bool does_move_clear_for_supply(cMatch &match, cGMove &move);

    bool is_move_forking(cMatch &match, cGMove &move);

    bool is_move_defending_fork(cMatch &match, cGMove &move);

    bool is_move_fleeing(cMatch &match, cGMove &move);

    bool is_move_unpin(cMatch &match, cGMove &move);

    bool is_move_blocking(cMatch &match, cGMove &move);

    bool is_move_running_pawn(cMatch &match, cGMove &move);

    bool is_move_tactical_draw(cMatch &match, cGMove &move);

#endif