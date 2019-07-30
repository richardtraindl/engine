#ifndef ANALYZE_HELPER_HPP
    #define ANALYZE_HELPER_HPP

    #include <list> 
    #include <string>
    #include "../match.hpp"
    #include "../move.hpp"
    #include "../pieces/touch.hpp"

    using namespace std;

    int lowest_piece(list<cTouch> &touches);

    bool are_fairy_equal(int piece1, int piece2);

    void find_touches_on_dstfield_after_move(cMatch &match, int piece, cPrioMove &priomove, list<cTouch> &friends, list<cTouch> &enmies);

    bool is_supporter_lower_attacker(cMatch &match, int piece, cPrioMove &priomove, list<cTouch> &supported);

    bool check_mates_deep_search(cMatch &match, cPrioMove &priomove);

    bool calc_checks(cMatch &match, int maxcnt, int count);

    bool is_soft_pinned_move(cMatch &match, int piece, cPrioMove &priomove);

    bool is_supply(cMatch &match, int piece, cPrioMove &priomove);

    bool is_touched_field_within_move(cMatch &match, int piece, cPrioMove &priomove, int touched_field);

    int weight_for_standard(cMatch &match, int piece, cPrioMove &priomove);

    int weight_for_capture(cMatch &match, int piece, cPrioMove &priomove, int weight);

    int weight_for_flee(cMatch &match, int piece, cPrioMove &priomove, int weight);

    int weight_for_running_pawn(cMatch &match, int piece, cPrioMove &priomove, int weight);

    int weight_for_discl_supporting(cTouch &discl_supported, int weight);

    int weight_for_discl_attacking(cTouch &discl_attacked, int weight);

    int weight_for_supporting(cMatch &match, int piece, cPrioMove &priomove, cTouch &supported, int weight);

    int weight_for_attacking_king(cMatch &match, cPrioMove &priomove, int weight);

    int weight_for_attacking(cMatch &match, cPrioMove &priomove, cTouch &attacked, int weight);

#endif
