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
            int maxdpth;
            int mvcnt_stage1;
            int mvcnt_stage2;

            cSearchLimits(int level, bool is_endgame);
    };

    void prnt_priomoves(list<cPrioMove *> &priomoves);
    
    void prnt_priomoves_for_thread(list<cPrioMove *> &priomoves, int startidx, int endidx);

    string concat_fmtmoves(list<cPrioMove> &priomoves);

    bool sortByPrio(cPrioMove *A, cPrioMove *B);

    void generate_moves(cMatch &match, list<cMove *> &moves);
    
    void generate_priomoves(cMatch &match, cMove *dbggmove, list<cPrioMove *> &priomoves);

    void append_newmove(cPrioMove *priomove, list<cPrioMove> &newcandidates, list<cPrioMove> &rcandidates);

    int count_up_to_prio(list<cPrioMove *> &priomoves, int priolimit);

    int resort_exchange_moves(list<cPrioMove *> &priomoves, cPrioMove *last_pmove);

    void resort_exchange_move(list<cPrioMove *> &priomoves, cPrioMove *lastpmove);

    int select_movecnt(cMatch &match, list<cPrioMove *> &priomoves, int depth, cSearchLimits &slimits, cPrioMove *last_pmove);

    void clean_priomoves(list<cPrioMove *> priomoves);

    int alphabeta(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &candidates);

    int start_alphabeta_threads(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &rcandidates);

    void alphabeta_as_thread(const int threadcnt, const int maxthreads, cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &rcandidates, int &rscore);

    int calc_move(cMatch &match, list<cPrioMove> &candidates);

#endif
