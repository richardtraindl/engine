#ifndef CALC_HPP
    #define CALC_HPP

    #include <algorithm>
    #include <ctime>
    #include <iostream>
    #include <list> 
    #include <string>
    #include "./generator.hpp"
    #include "./helper.hpp"
    #include "./match.hpp"
    #include "./move.hpp"
    #include "./values.hpp"


    using namespace std;


    void prnt_moves(list<cGMove> &moves);

    string concat_fmtmoves(list<cGMove> &moves);

    void append_newmove(cGMove &move, list<cGMove> &newcandidates, list<cGMove> &rcandidates);

    uint8_t count_up_to_prio(list<cGMove> &moves, int limit);

    bool sortByPresort(cGMove &A, cGMove &B);

    uint8_t select_movecnt(cMatch &match, list<cGMove> &moves, uint8_t depth, cGMove *last_move);

    // void clean_moves(list<cGMove> &moves);

    int16_t alphabeta(cMatch &match, uint8_t depth, uint8_t maxdepth, int16_t alpha, int16_t beta, bool maximizing, cGMove *last_move, list<cGMove> &rcandidates);

    int16_t calc_move(cMatch &match, list<cGMove> &rcandidates);

#endif
