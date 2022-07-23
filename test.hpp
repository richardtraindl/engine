#ifndef TEST_HPP
    #define TEST_HPP

    #include <iostream>
    #include "./match.hpp"


    using namespace std;

    void play100(cMatch &match, uint8_t engine_color);

    void prnt_eval_field_states(const cMatch &match);

    void prnt_moves_score(cMatch &match);

    void prnt_eval_pin_state(const cMatch &match, const cPiece &piece);

    void prnt_does_move_touch_weak_piece(cMatch &match, const cMove &move);

    void prnt_moves_touch_weak_piece(cMatch &match);

    void prnt_moves_weak_state(cMatch &match);

    void prnt_moves_support_supply(cMatch &match);

    void prnt_test(cMatch &match);

#endif
