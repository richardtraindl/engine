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

    bool is_dst_field_ok(int piece, list<cTouch> friends_on_dstfield, list<cTouch>enmies_on_dstfield);

    void find_touches_on_dstfield_after_move(cMatch &match, int piece, cPrioMove &priomove, list<cTouch> &friends, list<cTouch> &enmies);

    bool is_supporter_lower_attacker(cMatch &match, int piece, cPrioMove &priomove, cTouch &supported);

    bool is_move_out_of_soft_pin(cMatch &match, int piece, cPrioMove &priomove);

    bool is_supply(cMatch &match, int piece, cPrioMove &priomove);

    bool are_move_dirs_equal(int piece1, int src1, int dst1, int piece2, int src2, int dst2);

    int search_for_checkmate(cMatch &match);

    int _search_for_checkmate(cMatch &match, int count, int maxcnt);

    bool find_excluded(list<cExclude*> &excludes, int pos, int touch_pos, int tactic_domain);

    int weight_for_standard(cMatch &match, int piece, cPrioMove &priomove);

    int weight_for_capture(cMatch &match, int piece, int dstpiece, cPrioMove &priomove);

    int weight_for_flee(cMatch &match, int piece, cPrioMove &priomove, int weight);

    int weight_for_running_pawn(cMatch &match, int piece, cPrioMove &priomove, int weight);

    int weight_for_discl_supporting(cTouch &discl_supported, int weight);

    int weight_for_discl_attacking(cTouch &discl_attacked, int weight);

    int weight_for_supporting(cMatch &match, int piece, cPrioMove &priomove, cTouch &supported, int weight);

    //int weight_for_attacking_king(cMatch &match, cPrioMove &priomove, int weight);

    int weight_for_attacking(cMatch &match, int piece, cPrioMove &priomove, cTouch &attacked, int weight);

#endif
