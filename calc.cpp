
    #include <iostream>
    #include <thread>
    #include <algorithm>
    #include <vector>
    #include "./calc.hpp"
    #include "./openings.hpp"
    #include "../pieces/piece.hpp"
    #include "./analyze_position.hpp"
    #include "../helper.hpp"
    #include "../values.hpp"


    cSearchLimits::cSearchLimits(int level, bool is_endgame){
        maxdpth = 15;
        if(level == cMatch::LEVELS["blitz"]){
            dpth_stage1 = 3;
            dpth_stage2 = 5;
            mvcnt_stage1 = 8;
            mvcnt_stage2 = 4;
            return;
        }
        if(level == cMatch::LEVELS["low"]){
            dpth_stage1 = 3;
            dpth_stage2 = 7;
            mvcnt_stage1 = 10;
            mvcnt_stage2 = 5;
            return;
        }
        //medium high
        dpth_stage1 = 5;
        dpth_stage2 = 9;
        mvcnt_stage1 = 12;
        mvcnt_stage2 = 6;
    }


    void prnt_priomoves(list<cPrioMove *> &priomoves){
        cout << "-------------------------------------------" << endl;
        int idx = 1;
        for(cPrioMove *priomove : priomoves){
            cout << "\n" << dec << idx << ". ";
            cout << priomove->format() << " prio: " << priomove->prio << " is_tactic_stormy: " << priomove->is_tactic_stormy() << endl;
            cout << priomove->fmt_tactics() << endl;
            idx++;
        }
        cout << "-------------------------------------------" << endl;
    }


    void prnt_priomoves_for_thread(list<cPrioMove *> &priomoves, int threadcnt){
        cout << "-------------------------------------------" << endl;
        int idx = 0;
        for(cPrioMove *priomove : priomoves){
            if(idx % 4 == (threadcnt - 1)){ 
                cout << "\n" << dec << (idx + 1) << ". ";
                cout << priomove->format() << " prio: " << priomove->prio << " is_tactic_stormy: " << priomove->is_tactic_stormy() << endl;
                cout << priomove->fmt_tactics() << endl;
            }
            idx++;
        }
        cout << "-------------------------------------------" << endl;
    }


    string concat_fmtmoves(list<cPrioMove> &priomoves){
        string str_moves = "";
        for(cPrioMove priomove : priomoves){
            str_moves += " [" + priomove.format() + "] ";
        }
        return str_moves;
    }


    void generate_moves(cMatch &match, list<cMove *> &moves){
        int color = match.next_color();
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK || color != PIECES_COLORS[piece]){
                continue;
            }
            else{
                cPiece cpiece(match.board, idx);
                cpiece.generate_moves(match, moves);
            }
        }
    }


    void generate_priomoves(cMatch &match, cMove *dbggmove, list<cPrioMove *> &priomoves){
        int color = match.next_color();
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK || color != PIECES_COLORS[piece]){
                continue;
            }
            else{
                cPiece cpiece(match.board, idx);
                cpiece.generate_priomoves(match, dbggmove, priomoves);
            }
        }
    }


    void append_newmove(cPrioMove *priomove, list<cPrioMove> &newcandidates, list<cPrioMove> &rcandidates){
        rcandidates.clear();
        rcandidates.push_back(*priomove);
        for(cPrioMove pmove : newcandidates){
            rcandidates.push_back(pmove);
        }
    }


    int count_up_to_prio(list<cPrioMove *> &priomoves, int priolimit){
        int count = 0;
        for(cPrioMove *priomove : priomoves){
            if(priomove->prio <= priolimit ){ // || priomove->is_tactic_stormy()
                count++;
            }
            else{
                break;
            }
        }
        return count;
    }


    bool sortByPrio(cPrioMove *A, cPrioMove *B){
        return (A->prio < B->prio);
    }


    int resort_exchange_moves(list<cPrioMove *> &priomoves, cPrioMove *last_pmove){
        int movecnt = 0;

        if(last_pmove != NULL && 
           last_pmove->has_domain(cTactic::DOMAINS["captures"] == false)){
            return movecnt;
        }

        cPrioMove *first_silent = NULL;
        cPrioMove *first_bad_capture = NULL;
        for(cPrioMove *priomove : priomoves){
            if(priomove->has_domain(cTactic::DOMAINS["captures"])){
                if(priomove->fetch_weight(cTactic::DOMAINS["captures"]) == cTactic::WEIGHTS["bad-deal"] ||
                   priomove->fetch_weight(cTactic::DOMAINS["captures"]) == cTactic::WEIGHTS["vague-deal"]){
                    if(first_bad_capture == NULL){
                        first_bad_capture = priomove;
                    }
                }
                else{
                    priomove->prio = min(priomove->prio, (cPrioMove::PRIOS["prio0"] + priomove->prio % 10));
                    movecnt++;
                }
                continue;
            }
            if(first_silent == NULL){
                first_silent = priomove;
                continue;
            }
        }

        if(movecnt > 0){
            priomoves.sort(sortByPrio);
            return movecnt;
        }
        if(last_pmove != NULL && 
           (last_pmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["vague-deal"]) ||
            last_pmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["bad-deal"]))){
            if(first_bad_capture != NULL){
                first_bad_capture->prio = min(first_bad_capture->prio, (cPrioMove::PRIOS["prio0"] + first_bad_capture->prio % 10));
                movecnt++;
                if(first_silent != NULL){
                    first_silent->prio = min(first_silent->prio, (cPrioMove::PRIOS["prio0"] + first_silent->prio % 10) - 10);
                    movecnt++;
                }
            }
        }
        priomoves.sort(sortByPrio);
        return movecnt;
    }


    void resort_moves(list<cPrioMove *> &priomoves, cPrioMove *lastpmove){
        if(lastpmove == NULL ||
           (lastpmove != NULL && 
            lastpmove->has_domain(cTactic::DOMAINS["captures"] == false))){
            return;
        }

        bool capture = false;
        bool silent = true;
        if(lastpmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["bad-deal"]) ||
           lastpmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["vague-deal"])){
            silent = false;
        }
        for(cPrioMove *priomove : priomoves){
            if(priomove->has_domain(cTactic::DOMAINS["captures"]) && capture == false){
                priomove->prio = min(priomove->prio, (0 + priomove->prio % 10));
                capture = true;
            }
            if(priomove->has_domain(cTactic::DOMAINS["captures"]) == false && silent == false){
                priomove->prio = min(priomove->prio, (0 + priomove->prio % 10));
                silent = true;
            }
            if(capture && silent){
                return;
            }
        }
    }


    int select_movecnt(cMatch &match, list<cPrioMove *> &priomoves, int depth, cSearchLimits &slimits, cPrioMove *last_pmove){
        if(priomoves.size() == 0){
            return 0;
        }

        int movecnt = 0;
        if(depth <= slimits.dpth_stage1 && 
           last_pmove != NULL && last_pmove->has_domain(cTactic::DOMAINS["attacks-king"])){
            return priomoves.size();
        }

        resort_moves(priomoves, last_pmove);
        if(depth <= slimits.dpth_stage1){
            movecnt = count_up_to_prio(priomoves, cPrioMove::PRIOS["prio1"]);
            return max(slimits.mvcnt_stage1, movecnt);
        }
        if(depth <= slimits.dpth_stage2){
            movecnt = count_up_to_prio(priomoves, cPrioMove::PRIOS["prio1"]);
            return min(slimits.mvcnt_stage2, movecnt);
        }
        if(depth <= slimits.maxdpth){
            movecnt = count_up_to_prio(priomoves, cPrioMove::PRIOS["prio0"]);
            if(last_pmove != NULL && 
               last_pmove->is_tactic_stormy()){ // || last_pmove->has_domain(cTactic::DOMAINS["attacks-king"])
                return max(2, movecnt);
            }
            else{
                return movecnt;
            }
        }
        else{
            return 0;
        }
    }


    void clean_priomoves(list<cPrioMove *> priomoves){
        for(cPrioMove *priomove : priomoves){
            for(cTactic *tactic : priomove->tactics){
                delete tactic;
            }
            delete priomove;
        }
    }


    int alphabeta(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &rcandidates){
        list<cPrioMove> newcandidates;
        int newscore, bestscore;
        int count = 0;
        
        if(maximizing){
            bestscore = SCORES[mWKG] * 10;
        }
        else{
            bestscore = SCORES[mBKG] * 10;
        }

        cMove *dbggmove = NULL; //new cMove(0x0, coord_to_index("d1"), coord_to_index("d7"), mBLK);
        list<cPrioMove *> priomoves;
        generate_priomoves(match, dbggmove, priomoves);
        priomoves.sort(sortByPrio);
        int maxcnt = select_movecnt(match, priomoves, depth, slimits, last_pmove);
        if(depth == 1){
            cout << "priomoves count: " << priomoves.size() << " maxcnt: " << maxcnt << endl;
            prnt_priomoves(priomoves);
        }

        if(priomoves.size() == 0 || maxcnt == 0){
            bestscore = score_position(match, priomoves.size());
            clean_priomoves(priomoves);
            return bestscore;
        }

        for(cPrioMove *priomove : priomoves){
            count++;
            if(count > maxcnt){
                break;
            }
            newcandidates.clear();

            if(depth == 1){
                cout << "\n" << count << "/" << maxcnt << " CURRENT SEARCH: " << " [" + priomove->format() + "] " << concat_fmtmoves(newcandidates) << endl;
            }
            if(depth > slimits.mvcnt_stage2){
                cout << "-" << depth << "-";
            }

            if(priomove->has_domain(cTactic::DOMAINS["is-tactical-draw"])){
                newscore = 0;
            }
            else{
                match.do_move(priomove->src, priomove->dst, priomove->prompiece);
                //if(depth == 1){
                //    newscore = start_alphabeta_threads(match, depth + 1, slimits, alpha, beta, !maximizing, priomove, newcandidates);
                //}
                //else{
                    newscore = alphabeta(match, depth + 1, slimits, alpha, beta, !maximizing, priomove, newcandidates);
                //}
                match.undo_move();
            }

            if(maximizing){
                if(newscore > bestscore){
                    bestscore = newscore;
                    alpha = max(bestscore, alpha);
                    append_newmove(priomove, newcandidates, rcandidates);
                    if(depth == 1){
                        cout << "\n!!!CANDIDATE:      " << dec << bestscore << concat_fmtmoves(rcandidates) << endl;
                    }
                    if(alpha >= beta){
                        break;
                    }
                }
            }
            else{
                if(newscore < bestscore){
                    bestscore = newscore;
                    beta = min(bestscore, beta);
                    append_newmove(priomove, newcandidates, rcandidates);
                    if(depth == 1){
                        cout << "\n!!!CANDIDATE:      " << dec << bestscore << concat_fmtmoves(rcandidates) << endl;
                    }
                    if(beta <= alpha){
                        break;
                    }
                }
            }
        }
        clean_priomoves(priomoves);
        return bestscore;
    }


    int start_alphabeta_threads(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &rcandidates){
        const int maxthreads = 4;
        array<list<cPrioMove>, maxthreads> candidates;
        array<cMatch*, maxthreads> matches;
        thread threads[maxthreads];
        int rscore;
        int scores[maxthreads];

        for(int idx = 0; idx < maxthreads; ++idx){
            matches[idx] = new cMatch(match);
            threads[idx] = thread(alphabeta_as_thread, (idx + 1), maxthreads, ref(*matches[idx]), depth, ref(slimits), alpha, beta, maximizing, last_pmove, ref(candidates[idx]), ref(scores[idx]));
        }

        for(int idx = 0; idx < maxthreads; ++idx){
            threads[idx].join();
            cout << "\nthread # " << (idx + 1) << " joined ";
            cout << scores[idx] << " " << concat_fmtmoves(candidates[idx]) << endl;
            delete matches[idx];
        }

        rscore = scores[0];
        rcandidates.assign(candidates[0].begin(), candidates[0].end());
        for(int idx = 1; idx < maxthreads; ++idx){
            if(maximizing){
                if(scores[idx] > rscore){
                    rscore = scores[idx];
                    rcandidates.clear();
                    rcandidates.assign(candidates[idx].begin(), candidates[idx].end());
                }
            }
            else{
                if(scores[idx] < rscore){
                    rscore = scores[idx];
                    rcandidates.clear();
                    rcandidates.assign(candidates[idx].begin(), candidates[idx].end());
                }
            }
        }
        return rscore;
    }

        
    void alphabeta_as_thread(const int threadcnt, const int maxthreads, cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &rcandidates, int &rscore){
        list<cPrioMove> newcandidates;
        int newscore;
        int count = 0;

        if(maximizing){
            rscore = SCORES[mWKG] * 10;
        }
        else{
            rscore = SCORES[mBKG] * 10;
        }

        cMove *dbggmove = NULL;
        list<cPrioMove *> priomoves;
        generate_priomoves(match, dbggmove, priomoves);
        priomoves.sort(sortByPrio);
        int maxcnt = select_movecnt(match, priomoves, depth, slimits, last_pmove);

        cout << "thread #" << threadcnt << " - priomoves count: " << priomoves.size() << endl;
        prnt_priomoves_for_thread(priomoves, threadcnt);

        if(priomoves.size() == 0 || maxcnt == 0){
            rscore = score_position(match, priomoves.size());
            clean_priomoves(priomoves);
            return;
        }

        for(cPrioMove *priomove : priomoves){
            count++;
            if(count > maxcnt){
                break;
            }
            
            if((count - 1) % maxthreads != (threadcnt - 1)){
                continue;
            }

            newcandidates.clear();

            if(priomove->has_domain(cTactic::DOMAINS["is-tactical-draw"])){
                newscore = 0;
            }
            else{
                match.do_move(priomove->src, priomove->dst, priomove->prompiece);
                newscore = alphabeta(match, depth + 1, slimits, alpha, beta, !maximizing, priomove, newcandidates);
                match.undo_move();
            }

            if(maximizing){
                if(newscore > rscore){
                    rscore = newscore;
                    append_newmove(priomove, newcandidates, rcandidates);
                    cout << "\nthread #" << threadcnt << "   CANDIDATE:      " << dec << rscore << concat_fmtmoves(rcandidates) << endl;
                    alpha = max(rscore, alpha);
                    if(alpha >= beta){
                        break;
                    }
                }
            }
            else{
                if(newscore < rscore){
                    rscore = newscore;
                    append_newmove(priomove, newcandidates, rcandidates);
                    cout << "\nthread #" << threadcnt << "   CANDIDATE:      " << dec << rscore << concat_fmtmoves(rcandidates) << endl;
                    beta = min(rscore, beta);
                    if(beta <= alpha){
                        break;
                    }
                }
            }
        }
        clean_priomoves(priomoves);
        return;
    }


    int calc_move(cMatch &match, list<cPrioMove> &rcandidates){
        time_t time_start = time(0);

        cMove *cmove = retrieve_move(match);
        if(cmove != NULL){
            int prev_dstpiece = match.board.getfield(cmove->dst);
            cPrioMove priomove(cmove->src, cmove->dst, prev_dstpiece, cmove->prompiece, 0);
            rcandidates.push_back(priomove);
            delete cmove;
            return match.score;
        }
        else{
            cSearchLimits slimits(match.level, match.is_endgame());
            bool maximizing = match.next_color() == COLORS["white"];
            int alpha = SCORES[mWKG] * 10;
            int beta = SCORES[mBKG] * 10;
            //int rscore = start_alphabeta_threads(match, 1, slimits, alpha, beta, maximizing, NULL, rcandidates);
            int rscore = alphabeta(match, 1, slimits, alpha, beta, maximizing, NULL, rcandidates);
            cout << "\nresult: " << rscore << " match: " << match.created_at << " ";
            cout << concat_fmtmoves(rcandidates) << endl;
            prnt_fmttime("\ncalc-time: ", time(0) - time_start);
            return rscore;
        }
    }

