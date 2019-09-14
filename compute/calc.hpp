#ifndef CALC_HPP
    #define CALC_HPP

    #include <list> 
    #include <string>
    #include <ctime>
    #include "../match.hpp"
    #include "../move.hpp"

    using namespace std;

    class cSearchLimits{
        public:
            int dpth_stage1;
            int dpth_stage2;
            int dpth_max;
            int mvcnt_stage1;
            int mvcnt_stage2;

            cSearchLimits(int level, bool is_endgame);
    };

    void prnt_priomoves(list<cPrioMove *> &priomoves);
    
    void prnt_priomoves_for_thread(list<cPrioMove *> &priomoves, int threadcnt, int offset);

    string concat_fmtmoves(list<cPrioMove> &priomoves);

    //bool sortByPrio(cPrioMove &A, cPrioMove &B);
    bool sortByPrio(cPrioMove *A, cPrioMove *B);

    void generate_moves(cMatch &match, list<cMove *> &moves);
    
    void generate_priomoves(cMatch &match, cMove *dbggmove, list<cPrioMove *> &priomoves);

    void append_newmove(cPrioMove *priomove, list<cPrioMove> &newcandidates, list<cPrioMove> &rcandidates);

    int count_up_to_prio(list<cPrioMove *> &priomoves, int priolimit);

    void resort_exchange_and_stormy_moves(list<cPrioMove *> &priomoves, int new_prio, cPrioMove *last_pmove, bool only_exchange, list<cPrioMove*> &exchanges, list<cPrioMove*> &stormies);

    int select_movecnt(cMatch &match, list<cPrioMove *> &priomoves, int depth, cSearchLimits &slimits, cPrioMove *last_pmove);

    void clean_priomoves(list<cPrioMove *> priomoves);

    int alphabeta(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, list<cPrioMove> &candidates);

    void alphabeta_for_thread(cMatch &match, int depth, int threadcnt, int offset, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &rcandidates, int &rscore);

    int calc_move(cMatch &match, list<cPrioMove> &candidates);

#endif
