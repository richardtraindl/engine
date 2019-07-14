#ifndef CALC_HPP
    #define CALC_HPP

    #include <iostream>
    #include <list> 
    #include <string>
    #include <ctime>
    #include "../board.hpp"
    #include "../move.hpp"
    #include "../match.hpp"

    using namespace std;

    class cSearchLimits{
        public:
            static int add_mvcnt;
            static int dpth_max;
            static int dpth_stage1;
            static int dpth_stage2;
            static int mvcnt_stage1;
            static int mvcnt_stage2;
    };

    class cSearchLimitsBlitz : public cSearchLimits{
    };

    class cSearchLimitsLow : public cSearchLimits{
    };

    class cSearchLimitsMedium : public cSearchLimits{
    };

    class cSearchLimitsHigh : public cSearchLimits{
    };


    void prnt_priomoves(cMatch *match, list<cPrioMove> *priomoves);

    void prnt_search(cMatch *match, string label, int score, cMove *move, list<cMove> *candidates);

    string concat_fmtmoves(cMatch *match, list<cMove> *moves);

    list<cMove> *generate_moves(cMatch *match);
    
    list<cPrioMove> *generate_priomoves(cMatch *match, cMove *candidate, cMove *dbggmove, bool search_for_mate);

    //void append_newmove(cMove *move, list<cMove> candidates, list<cMove> newcandidates);

    //void alphabeta(cMatch *match, int depth, cSearchLimits *slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, int *score, list<cPrioMove> *candidates);

    //int count_up_to_prio(list<cPrioMove> *priomoves, int priolimit);

    //int count_up_within_stormy(list<cPrioMove> *priomoves);

    //bool resort_exchange_or_stormy_moves(list<cPrioMove> *priomoves, int new_prio, cPrioMove *last_pmove, bool only_exchange);

    //int select_movecnt(cMatch *match, list<cPrioMove> *priomoves, int depth, cSearchLimits *slimits, cPrioMove *last_pmove);

    //string concat_fmtmoves(cMatch *match, list<cPrioMove> *moves);

    //void prnt_fmttime(string msg, int seconds);

    //list<cPrioMove> *calc_move(cMatch *match, cMove *candidate);
#endif
