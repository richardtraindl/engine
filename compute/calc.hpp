#ifndef CALC_HPP
    #define CALC_HPP

    #include <iostream>
    #include <list> 
    #include <string>
    #include <ctime>
    #include "./board.hpp"
    #include "./move.hpp"
    #include "./match.hpp"

    using namespace std;

    void prnt_priomoves(cMatch *match, list<cPrioMove> *priomoves);

    void prnt_search(cMatch *match, string label, int score, cMove *move, int candidates[]);

    string concat_fmtmoves(cMatch *match, list<cMove> *moves);

    list<cPrioMove> generate_moves(cMatch *match, int candidate, cMove *dbggmove, bool search_for_mate, int mode);

    void append_newmove(cMove *move, list<cMove> candidates, list<cMove> newcandidates);

    alphabeta(cMatch *match, int depth, cSlimits *slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, int *score, list<cPrioMove> *candidates);

    class SearchLimitsLevelBlitz{
        public:
            static int add_mvcnt;
            static int dpth_max;
            static int dpth_stage1;
            static int dpth_stage2;
            static int mvcnt_stage1;
            static int mvcnt_stage2;
    };

    class SearchLimitsLevelBlitz : public SearchLimitsLevelLow{
    };

    class SearchLimitsLevelBlitz : public SearchLimitsLevelMedium{
    };

    class SearchLimitsLevelBlitz : public SearchLimitsLevelHigh{
    };

    int count_up_to_prio(list<cPrioMove> *priomoves, int priolimit);

    int count_up_within_stormy(list<cPrioMove> *priomoves);

    bool resort_exchange_or_stormy_moves(list<cPrioMove> *priomoves, int new_prio, cPrioMove *last_pmove, bool only_exchange);

    int select_movecnt(cMatch *match, list<cPrioMove> *priomoves, int depth, cSlimits *slimits, cPrioMove *last_pmove);

    string concat_fmtmoves(cMatch *match, list<cPrioMove> *moves);

    void prnt_fmttime(string msg, int seconds);

    list<cPrioMove> *calc_move(cMatch *match, cMove *candidate);

