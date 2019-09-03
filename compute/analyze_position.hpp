#ifndef ANALYZE_POSITION_HPP
    #define ANALYZE_POSITION_HPP

    #include "../match.hpp"

    using namespace std;

    int score_traps_and_touches(cMatch &match);

    int score_controled_horizontal_files(cMatch &match);

    int score_controled_vertical_files(cMatch &match);

    int score_kings_safety(cMatch &match);

    int score_penalty_for_lost_castlings(cMatch &match);

    int score_penalty_for_multiple_moves(cMatch &match);

    int score_penalty_for_knight_bishop_on_baseline(cMatch &match);

    int score_weak_pawns(cMatch &match);

    int score_penalty_for_weak_fianchetto(cMatch &match);

    int score_opening(cMatch &match);

    int score_middlegame(cMatch &match);

    int score_endgame(cMatch &match);

    int score_position(cMatch &match, int movecnt);

    bool is_stormy(cMatch &match);

#endif
