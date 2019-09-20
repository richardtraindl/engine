
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
        if(level == cMatch::LEVELS["blitz"]){
            dpth_stage1 = 3;
            if(is_endgame){
                dpth_stage2 = 5;
                //dpth_max = 9;
            }
            else{
                dpth_stage2 = 5;
                //dpth_max = 7;
            }
            dpth_max = 20;
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
                dpth_stage2 = 7;
                //dpth_max = 9;
            }
            dpth_max = 20;
            mvcnt_stage1 = 10;
            mvcnt_stage2 = 6;
            return;
        }
        //medium high
        dpth_stage1 = 5;
        if(is_endgame){
            dpth_stage2 = 7;
            //dpth_max = 13;
        }
        else{
            dpth_stage2 = 7;
            //dpth_max = 11;
        }
        dpth_max = 20;
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


    void prnt_priomoves_for_thread(list<cPrioMove *> &priomoves, int threadcnt, int offset){
        cout << "-------------------------------------------" << endl;
        int idx = 0;
        for(cPrioMove *priomove : priomoves){
            if(idx % threadcnt == offset){ 
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


    void resort_exchange_and_stormy_moves(list<cPrioMove *> &priomoves, int new_prio, cPrioMove *last_pmove, bool only_exchange, list<cPrioMove*> &exchanges, list<cPrioMove*> &stormies){
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
                    priomove->prio = min(priomove->prio, (new_prio + priomove->prio % 10) - 20);
                    exchanges.push_back(priomove);
                }
                continue;
            }
            if(priomove->has_weight(cTactic::WEIGHTS["stormy"])){
                if(only_exchange){
                    priomove->prio = min(priomove->prio, (new_prio + priomove->prio % 2) - 20);
                }
                else{
                    priomove->prio = min(priomove->prio, (new_prio + priomove->prio % 10) - 20);
                }
                stormies.push_back(priomove);
                continue;
            }
            if(first_silent == NULL){
                first_silent = priomove;
                continue;
            }
        }

        if(exchanges.size() > 0){
            priomoves.sort(sortByPrio);
            return;
        }
        if(last_pmove != NULL && 
           (last_pmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["vague-deal"]) ||
            last_pmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["bad-deal"]))){
            if(first_bad_capture != NULL){
                first_bad_capture->prio = min(first_bad_capture->prio, (new_prio + first_bad_capture->prio % 10) - 20);
                exchanges.push_back(first_bad_capture);
                if(first_silent != NULL && stormies.size() == 0){
                    first_silent->prio = min(first_silent->prio, (new_prio + first_silent->prio % 10) - 10);
                    stormies.push_back(first_silent);
                }
            }
        }
        priomoves.sort(sortByPrio);
        return;
    }


    int select_movecnt(cMatch &match, list<cPrioMove *> &priomoves, int depth, cSearchLimits &slimits, cPrioMove *last_pmove){
        //int count;
        list<cPrioMove*> exchanges, stormies;
        if(priomoves.size() == 0 || depth > slimits.dpth_max){
            return 0;
        }
        if(depth <= slimits.dpth_stage1 && last_pmove != NULL && last_pmove->has_domain(cTactic::DOMAINS["attacks-king"])){
            return priomoves.size();
        }
        
        if(depth <= slimits.dpth_stage1){
            resort_exchange_and_stormy_moves(priomoves, cPrioMove::PRIOS["prio1"], last_pmove, false, exchanges, stormies);
            return max(slimits.mvcnt_stage1, (int)(exchanges.size() + stormies.size())); // min(slimits.mvcnt_stage1, resort_exchange_and_stormy_moves(priomoves, cPrioMove::PRIOS["prio1"], last_pmove, true));
        }
        if(depth <= slimits.dpth_stage2){
            resort_exchange_and_stormy_moves(priomoves, cPrioMove::PRIOS["prio1"], last_pmove, false, exchanges, stormies);
            return max(slimits.mvcnt_stage2, (int)(exchanges.size() + stormies.size()));
        }
        else{
            if(last_pmove != NULL && last_pmove->has_domain(cTactic::DOMAINS["captures"])){
                resort_exchange_and_stormy_moves(priomoves, cPrioMove::PRIOS["prio0"], last_pmove, true, exchanges, stormies);
                return min(2, (int)(exchanges.size() + 1));
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
                cout << depth << ".";
            }

            if(priomove->has_domain(cTactic::DOMAINS["is-tactical-draw"])){
                newscore = 0;
            }
            else{
                match.do_move(priomove->src, priomove->dst, priomove->prompiece);
                newscore = alphabeta(match, depth + 1, slimits, alpha, beta, !maximizing, priomove, NULL, newcandidates);
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
        int maxcnt = select_movecnt(match, priomoves, depth, slimits, last_pmove);

        if(depth == 1){
            cout << "thread #" << offset << " - priomoves count: " << priomoves.size() << endl;
            prnt_priomoves_for_thread(priomoves, threadcnt, offset);
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
            }
            else{
                match.do_move(priomove->src, priomove->dst, priomove->prompiece);
                newscore = alphabeta(match, depth + 1, slimits, alpha, beta, !maximizing, priomove, NULL, newcandidates);
                //alphabeta_for_thread(match, depth + 1, threadcnt, 0, slimits, alpha, beta, !maximizing, priomove, newcandidates, newscore);
                match.undo_move();
            }

            if(maximizing){
                if(newscore > rscore){
                    rscore = newscore;
                    append_newmove(priomove, newcandidates, rcandidates);
                    alpha = max(rscore, alpha);
                    if(depth == 1){
                        cout << "\nthread #" << offset << " CANDIDATE:      " << dec << rscore << concat_fmtmoves(rcandidates) << endl;
                    }
                    if(alpha >= beta){
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


    int calc_move(cMatch &match, list<cPrioMove> &rcandidates){
        time_t time_start = time(0);
        
        cMove *cmove = retrieve_move(match);
        if(cmove != NULL){
            int prev_dstpiece = match.board.getfield(cmove->dst);
            cPrioMove priomove(cmove->src, cmove->dst, prev_dstpiece, cmove->prompiece, 0);
            rcandidates.push_back(priomove);
            return match.score;
        }
        else{
            cSearchLimits slimits(match.level, match.is_endgame());
            bool maximizing = match.next_color() == COLORS["white"];
            int alpha = SCORES[mWKG] * 10;
            int beta = SCORES[mBKG] * 10;
            int rscore = alphabeta(match, 1, slimits, alpha, beta, maximizing, NULL, NULL, rcandidates);
            cout << "\nresult: " << rscore << " match: " << match.created_at << " ";
            cout << concat_fmtmoves(rcandidates) << endl;
            prnt_fmttime("\ncalc-time: ", time(0) - time_start);
            return rscore;
        }
    }


    int calc_move2(cMatch &match, list<cPrioMove> &rcandidates){
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
        int threadcnt = 4;
        thread t1, t2, t3, t4; //threads[threadcnt];
        cPrioMove *last_priomove = NULL;
        //vector<list<cPrioMove>> candidates;
        list<cPrioMove> candidates1, candidates2, candidates3, candidates4;
        int rscore, score1, score2, score3, score4; //scores[threadcnt];
        cMatch *match1 = new cMatch(match);
        cMatch *match2 = new cMatch(match);
        cMatch *match3 = new cMatch(match);
        cMatch *match4 = new cMatch(match);
        t1 = thread(alphabeta_for_thread, ref(*match1), 1, threadcnt, 0, ref(slimits), alpha, beta, maximizing, last_priomove, ref(candidates1), ref(score1));
        t2 = thread(alphabeta_for_thread, ref(*match2), 1, threadcnt, 1, ref(slimits), alpha, beta, maximizing, last_priomove, ref(candidates2), ref(score2));
        t3 = thread(alphabeta_for_thread, ref(*match3), 1, threadcnt, 2, ref(slimits), alpha, beta, maximizing, last_priomove, ref(candidates3), ref(score3));
        t4 = thread(alphabeta_for_thread, ref(*match4), 1, threadcnt, 3, ref(slimits), alpha, beta, maximizing, last_priomove, ref(candidates4), ref(score4));

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        
        delete match1;
        delete match2;
        delete match3;
        delete match4;

        rscore = score1;
        rcandidates.assign(candidates1.begin(), candidates1.end());
        if(maximizing){
            if(score2 > rscore){
                rscore = score2;
                rcandidates.clear();
                rcandidates.assign(candidates2.begin(), candidates2.end());
            }
            if(score3 > rscore){
                rscore = score3;
                rcandidates.clear();
                rcandidates.assign(candidates3.begin(), candidates3.end());
            }
            if(score4 > rscore){
                rscore = score4;
                rcandidates.clear();
                rcandidates.assign(candidates4.begin(), candidates4.end());
            }
        }
        else{
            if(score2 < rscore){
                rscore = score2;
                rcandidates.clear();
                rcandidates.assign(candidates2.begin(), candidates2.end());
            }
            if(score3 < rscore){
                rscore = score3;
                rcandidates.clear();
                rcandidates.assign(candidates3.begin(), candidates3.end());
            }
            if(score4 < rscore){
                rscore = score4;
                rcandidates.clear();
                rcandidates.assign(candidates4.begin(), candidates4.end());
            }
        }

        cout << "result: " << rscore << " match: " << match.created_at << " " << endl;
        cout << concat_fmtmoves(rcandidates) << endl;
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);
        return rscore;
    }

