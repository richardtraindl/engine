#ifndef ANALYZE_MOVE_HPP
    #define ANALYZE_MOVE_HPP

    #include <map>
    #include <string>
    #include <cstdint>
    #include <list> 
    #include "./helper.hpp"
    #include "./generator.hpp"
    #include "./match.hpp"
    #include "./move.hpp"
    #include "./values.hpp"
    

    bool does_capture_eliminate_supporter_attacker(cMatch &match, cGMove &move);

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