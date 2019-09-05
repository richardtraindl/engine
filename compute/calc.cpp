
    #include <iostream>
    #include <thread>
    #include <algorithm>
    #include <vector>
    #include "./calc.hpp"
    #include "../pieces/piece.hpp"
    #include "./analyze_position.hpp"
    #include "../helper.hpp"
    #include "../values.hpp"


    cSearchLimits::cSearchLimits(int level, bool is_endgame){
        if(level == cMatch::LEVELS["blitz"]){
            dpth_stage1 = 1;
            if(is_endgame){
                dpth_stage2 = 5;
                //dpth_max = 9;
            }
            else{
                dpth_stage2 = 3;
                //dpth_max = 7;
            }
            dpth_max = 7;
            mvcnt_stage1 = 8;
            mvcnt_stage2 = 6;
            return;
        }
        if(level == cMatch::LEVELS["low"]){
            dpth_stage1 = 3;
            if(is_endgame){
                dpth_stage2 = 7;
                //dpth_max = 11;
            }
            else{
                dpth_stage2 = 5;
                //dpth_max = 9;
            }
            dpth_max = 9;
            mvcnt_stage1 = 10;
            mvcnt_stage2 = 6;
            return;
        }
        //medium high
        dpth_stage1 = 3;
        if(is_endgame){
            dpth_stage2 = 7;
            //dpth_max = 13;
        }
        else{
            dpth_stage2 = 5;
            //dpth_max = 11;
        }
        dpth_max = 11;
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
            idx += 1;
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


    void prnt_fmttime(string msg, int seconds){
        int sec, minute, hour;
        hour = seconds / (60 * 60);
        int remaining = seconds % (60 * 60);
        minute = remaining / 60;
        sec = remaining % 60;
        cout << msg << hour << minute << sec << endl;
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
            if(priomove->prio <= priolimit || priomove->is_tactic_stormy()){
                count += 1;
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
    //bool sortByPrio(cPrioMove &A, cPrioMove &B){
    //    return (A.prio < B.prio);
    //}


    bool resort_exchange_moves(list<cPrioMove *> &priomoves, int new_prio, cPrioMove *last_pmove, bool with_stormy){
        if(with_stormy == false && 
           (last_pmove == NULL || (last_pmove != NULL && last_pmove->has_domain(cTactic::DOMAINS["captures"]) == false))){
            return false;
        }
        list<cPrioMove *> goodes;
        cPrioMove *first_silent = NULL;
        cPrioMove *first_bad_capture = NULL;
        for(cPrioMove *priomove : priomoves){
            if(with_stormy && priomove->is_tactic_stormy()){
                goodes.push_back(priomove);
                continue;
            }
            if(priomove->has_domain(cTactic::DOMAINS["captures"])){
                if(priomove->fetch_weight(cTactic::DOMAINS["captures"]) == cTactic::WEIGHTS["bad-deal"]){
                    if(first_bad_capture == NULL){
                        first_bad_capture = priomove;
                    }
                }
                else{
                    goodes.push_back(priomove);
                }
            }
            else{
                if(first_silent == NULL){
                    first_silent = priomove;
                }
            }
        }
        if(goodes.size() > 0){
            for(cPrioMove *priomove : goodes){
                priomove->prio = min(priomove->prio, (new_prio + priomove->prio % 10) - 20);
            }
            priomoves.sort(sortByPrio);
            return true;
        }
        if(last_pmove != NULL && last_pmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["bad-deal"])){
            if(first_bad_capture != NULL){
                first_bad_capture->prio = min(first_bad_capture->prio, (new_prio + first_bad_capture->prio % 10) - 20);
                if(first_silent != NULL){
                    first_silent->prio = min(first_silent->prio, (new_prio + first_silent->prio % 10) - 10);
                }
                priomoves.sort(sortByPrio);
                return true;
            }
        }
        return false;
    }


    int select_movecnt(cMatch &match, list<cPrioMove *> &priomoves, int depth, cSearchLimits &slimits, cPrioMove *last_pmove){
        int count;
        if(priomoves.size() == 0 || depth > slimits.dpth_max){
            return 0;
        }
        if(depth <= slimits.dpth_stage1 && last_pmove != NULL && last_pmove->has_domain(cTactic::DOMAINS["attacks-king"])){
        //if(depth <= slimits.dpth_stage1 && priomoves.front()->has_domain(cTactic::DOMAINS["defends-check"])){
            cout << "movecnt defends-check: " << priomoves.size() << endl;
            return priomoves.size();
        }
        
        if(depth <= slimits.dpth_stage1){
            resort_exchange_moves(priomoves, cPrioMove::PRIOS["prio1"], last_pmove, true);
            //int stormycnt = count_up_within_stormy(priomoves);
            count = count_up_to_prio(priomoves, cPrioMove::PRIOS["prio2"]);
            if(count == 0){
                count = slimits.mvcnt_stage1;
            }
            else{
                count = min(count, slimits.mvcnt_stage1);
            }
            return count; //max(stormycnt, count);
        }
        if(depth <= slimits.dpth_stage2){
            resort_exchange_moves(priomoves, cPrioMove::PRIOS["prio1"], last_pmove, true);
            //int stormycnt = count_up_within_stormy(priomoves);
            count = count_up_to_prio(priomoves, cPrioMove::PRIOS["prio2"]);
            return count; //max(stormycnt, count);
        }
        else{
            if(resort_exchange_moves(priomoves, cPrioMove::PRIOS["prio0"], last_pmove, false)){
                return count_up_to_prio(priomoves, cPrioMove::PRIOS["prio0"]);
            }
            else{
                return 0;
            }
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


    int alphabeta(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, list<cPrioMove> &rcandidates){
        list<cPrioMove> newcandidates;
        int newscore;
        int bestscore;
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
                cout << "\nCURRENT SEARCH: " << " [" + priomove->format() + "] " << concat_fmtmoves(newcandidates) << endl;
            }
            if(depth > slimits.mvcnt_stage2){
                cout << depth << ".";
            }

            match.do_move(priomove->src, priomove->dst, priomove->prompiece);
            newscore = alphabeta(match, depth + 1, slimits, alpha, beta, !maximizing, priomove, NULL, newcandidates);
            match.undo_move();

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

 
    void alphabeta_for_thread(cMatch &match, int depth, int threadcnt, int offset, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, list<cPrioMove> &rcandidates, int &rscore){
        list<cPrioMove> newcandidates;
        int newscore;
        int count = 0;

        if(maximizing){
            rscore = SCORES[mWKG] * 10;;
        }
        else{
            rscore = SCORES[mBKG] * 10;;
        }

        cMove *dbggmove = NULL; //new cMove(0x0, coord_to_index("d1"), coord_to_index("d7"), mBLK);
        list<cPrioMove *> priomoves;
        generate_priomoves(match, dbggmove, priomoves);
        priomoves.sort(sortByPrio);
        int maxcnt = select_movecnt(match, priomoves, depth, slimits, NULL);

        if(depth == 1){
            cout << "thread #" << offset << " - priomoves count: " << priomoves.size() << endl;
            //prnt_priomoves(priomoves);
        }

        if(priomoves.size() == 0 || maxcnt == 0){
            rscore = score_position(match, priomoves.size());
            clean_priomoves(priomoves);
            return;
        }

        for(cPrioMove *priomove : priomoves){
            newcandidates.clear();

            if(count >= maxcnt){
                break;
            }

            if(depth == 1 && count % threadcnt != offset){
                count++;
                continue;
            }
            count++;

            if(depth == 1){
                cout << "\nthread #" << offset << " CURRENT SEARCH: " << " [" + priomove->format() + "] " << concat_fmtmoves(newcandidates) << endl;
            }
            if(depth > slimits.mvcnt_stage2){
                cout << ".";
            }
            
            if(priomove->has_domain(cTactic::DOMAINS["is-tactical-draw"])){
                newscore = 0;
                if(maximizing && newscore > rscore){
                    rscore = newscore;
                    append_newmove(priomove, newcandidates, rcandidates);
                    alpha = max(rscore, alpha);
                    if(alpha >= beta){
                        break;
                    }
                }
                if(!maximizing && newscore < rscore){
                    rscore = newscore;
                    append_newmove(priomove, newcandidates, rcandidates);
                    beta = min(rscore, beta);
                    if(beta <= alpha){
                        break;
                    }
                }
            }

            match.do_move(priomove->src, priomove->dst, priomove->prompiece);
            alphabeta_for_thread(match, depth + 1, threadcnt, 0, slimits, alpha, beta, !maximizing, priomove, newcandidates, newscore);
            match.undo_move();

            if(maximizing){
                if(depth == 1 && count % threadcnt == 1){
                    cout << (count % threadcnt) << " TEST1 rscore:" << rscore << " alpha:" << alpha << " beta:" << beta << " newscore:" << newscore << " newcandidates.size:" << newcandidates.size() << " " << &rcandidates<< endl;
                }
                if(newscore > rscore){
                    rscore = newscore;
                    append_newmove(priomove, newcandidates, rcandidates);
                    alpha = max(rscore, alpha);
                    if(depth == 1 && count % threadcnt == 1){
                        cout << (count % threadcnt) << " TEST2 rscore:" << rscore << " alpha:" << alpha << " beta:" << beta << " newscore:" << newscore << " rcandidates.size:" << rcandidates.size() << " " << &rcandidates<< endl;
                    }
                    if(depth == 1){
                        cout << "\nthread #" << offset << " CANDIDATE:      " << dec << rscore << concat_fmtmoves(rcandidates) << endl;
                    }
                    if(alpha >= beta){
                        if(depth == 1 && count % threadcnt == 1){
                            cout << (count % threadcnt) << " TEST3 rscore:" << rscore << " alpha:" << alpha << " beta:" << beta << " newscore:" << newscore << " rcandidates.size:" << rcandidates.size() << " " << &rcandidates << endl;
                        }
                        break;
                    }
                }
            }
            else{
                if(newscore < rscore){
                    rscore = newscore;
                    append_newmove(priomove, newcandidates, rcandidates);
                    if(depth == 1){
                        cout << "\nthread #" << offset << " CANDIDATE:      " << dec << rscore << concat_fmtmoves(rcandidates) << endl;
                    }
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


    int calc_move(cMatch &match, list<cPrioMove> &rcandidates, int threadcnt){
        time_t time_start = time(0);
        // move = retrieve_move(match)
        //if(move):
        //    candidates.append(move)
        //    score = match.score
        //else:
        cSearchLimits slimits(match.level, match.is_endgame());
        bool maximizing = match.next_color() == COLORS["white"];
        int alpha = SCORES[mWKG] * 10;
        int beta = SCORES[mBKG] * 10;
        int rscore = alphabeta(match, 1, slimits, alpha, beta, maximizing, NULL, NULL, rcandidates);
        cout << "result: " << rscore << " match: " << match.created_at << " " << endl;
        cout << concat_fmtmoves(rcandidates) << endl;
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);
        return rscore;
    }


    int calc_move_old(cMatch &match, list<cPrioMove> &rcandidates, int threadcnt){
        time_t time_start = time(0);
        // move = retrieve_move(match)
        //if(move):
        //    candidates.append(move)
        //    score = match.score
        //else:
        cSearchLimits slimits(match.level, match.is_endgame());
        bool maximizing = match.next_color() == COLORS["white"];
        int alpha = SCORES[mWKG] * 10;
        int beta = SCORES[mBKG] * 10;
        thread threads[threadcnt];
        cPrioMove *last_priomove = NULL;
        //vector<list<cPrioMove>> candidates;
        int scores[threadcnt];
        for(int idx = 0; idx < threadcnt; ++idx){
            //candidates.push_back(*(new list<cPrioMove>));
            threads[idx] = thread(alphabeta_for_thread, ref(*(new cMatch(match))), 1, threadcnt, idx, ref(slimits), alpha, beta, maximizing, last_priomove, ref(*(new list<cPrioMove>)), ref(*(scores + idx))); //candidates.back()
        }
        for(int idx = 0; idx < threadcnt; ++idx){
            threads[idx].join();
        }
        int rscore;
        if(maximizing){
            rscore = SCORES[mWKG] * 10;
        }
        else{
            rscore = SCORES[mBKG] * 10;
        }
        for(int idx = 0; idx < threadcnt; ++idx){
            cout << "rscore " << rscore << " scores " << scores[idx] << endl; //" candidates: " << candidates[idx].size() << " rcandidates: " << &(candidates[idx])
            /*if(candidates[idx].size() > 0){
                if((maximizing && scores[idx] > rscore) || (!maximizing && scores[idx] < rscore)){
                    rscore = scores[idx];
                    rcandidates.clear();
                    rcandidates.assign(candidates[idx].begin(), candidates[idx].end());
                }
            }*/
        }

        cout << "result: " << rscore << " match: " << match.created_at << " " << endl;
        cout << concat_fmtmoves(rcandidates) << endl;
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);
        return rscore;
    }

