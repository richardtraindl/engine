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


    extern uint64_t calccnt;


    void prnt_moves(list<cGMove> &moves);

    string concat_fmtmoves(list<cGMove> &moves);

    void append_newmove(cGMove &move, list<cGMove> &newcandidates, list<cGMove> &rcandidates);

    void count_limits(list<cGMove> &moves, uint8_t &badcnt, uint8_t &mediumcnt, uint8_t &highcnt, uint8_t &stormycnt);

    bool sortByPresort(cGMove &A, cGMove &B);

    uint8_t determine_movecnt(cMatch &match, list<cGMove> &moves, uint8_t depth, uint8_t maxdepth);

    int16_t alphabeta(cMatch &match, uint8_t depth, uint8_t maxdepth, int16_t alpha, int16_t beta, bool maximizing, list<cGMove> &rcandidates);

    int16_t calc_move(cMatch &match, list<cGMove> &rcandidates);

#endif
