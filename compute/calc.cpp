
    #include <iostream>    
    #include "./calc.hpp"
    #include "../piece.hpp"
    #include "../values.hpp"

    cSearchLimits::cSearchLimits(int level){
        if(level == cMatch::LEVELS["blitz"]){
            add_mvcnt = 2;
            dpth_max = 8;
            dpth_stage1 = 2;
            dpth_stage2 = 4;
            mvcnt_stage1 = 6;
            mvcnt_stage2 = 6;
        }
        else{
            add_mvcnt = 2;
            dpth_max = 10;
            dpth_stage1 = 3;
            dpth_stage2 = 5;
            mvcnt_stage1 = 8;
            mvcnt_stage2 = 8;
        }
    }

    void prnt_priomoves(list<cPrioMove> &priomoves){
        cout << "------------------------------------------------\n ";
        int idx = 1;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            cout << idx << ". ";
            cout << it->format() << " prio: " << it->prio << " is_tactic_stormy: " << it->is_tactic_stormy() << "\n";
            cout << it->concat_fmttactics() << "\n";
            idx += 1;
        }
        cout << "------------------------------------------------\n ";
    }


    string concat_fmtmoves(list<cPrioMove> &priomoves){
        string str_moves = "";
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            str_moves += " [" + it->format() + "] ";
        }
        return str_moves;
    }


    void prnt_fmttime(string msg, int seconds){
        int sec, minute, hour;
        hour = seconds / (60 * 60);
        int remaining = seconds % (60 * 60);
        minute = remaining / 60;
        sec = remaining % 60;
        cout << msg << hour << minute << sec;
    }


    void generate_moves(cMatch &match, list<cMove> &moves){
        int color = match.next_color();
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK || color != PIECES_COLORS[piece]){
                continue;
            }
            else{
                cPiece cpiece(&(match.board), idx);
                cpiece.generate_moves(&(match.minutes), &moves);
            }
        }
    }

    void generate_priomoves(cMatch &match, cMove *candidate, cMove *dbggmove, bool search_for_mate, list<cPrioMove> &priomoves){
        int color = match.next_color();
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK || color != PIECES_COLORS[piece]){
                continue;
            }
            else{
                cPiece cpiece(&(match.board), idx);
                cpiece.generate_priomoves(&(match.minutes), candidate, dbggmove, search_for_mate, &priomoves);
            }
        }
    }


    void append_newmove(cPrioMove &move, list<cPrioMove> &candidates, list<cPrioMove> &newcandidates){
        candidates.push_back(move);
        for(list<cPrioMove>::iterator it = newcandidates.begin(); it != newcandidates.end(); ++it){
            candidates.push_back((*it));
        }
    }

    int count_up_to_prio(list<cPrioMove> &priomoves, int priolimit){
        int count = 0;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            if(it->prio <= priolimit || it->is_tactic_stormy()){
                count += 1;
            }
            else{
                break;
            }
        }
        return count;
    }


    int count_up_within_stormy(list<cPrioMove> &priomoves){
        int count = 0;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            if(it->is_tactic_stormy()){
                count += 1;
            }
        }
        return count;
    }

    bool sortByPrio(cPrioMove &A, cPrioMove &B){
        return (A.prio < B.prio);
    }

    bool resort_exchange_or_stormy_moves(list<cPrioMove> &priomoves, int new_prio, cPrioMove *last_pmove, bool only_exchange){
        if(only_exchange && last_pmove != NULL && last_pmove->has_domain(cTactic::DOMAINS["captures"]) == false){
            return false;
        }
        bool last_pmove_capture_bad_deal;
        if(last_pmove != NULL && last_pmove->has_tactic_ext(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["bad-deal"])){
            last_pmove_capture_bad_deal = true;
        }
        else{
            last_pmove_capture_bad_deal = false;
        }
        int count_of_stormy = 0;
        int count_of_good_captures = 0;
        cPrioMove *first_silent = NULL;
        list<cPrioMove> bad_captures;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            if(only_exchange == false && it->is_tactic_stormy()){
                count_of_stormy += 1;
                it->prio = min(it->prio, (new_prio + it->prio % 10) - 13);
                continue;
            }
            cout << "after" << endl;
            if(it->has_domain(cTactic::DOMAINS["captures"])){
                int weight = it->fetch_weight(cTactic::DOMAINS["captures"]);
                if(weight > cTactic::WEIGHTS["bad-deal"]){
                    count_of_good_captures += 1;
                    it->prio = min(it->prio, (new_prio + it->prio % 10) - 12);
                }
                if(last_pmove_capture_bad_deal){
                    bad_captures.push_back(*it);
                    // count_of_bad_captures += 1
                    // priomove.prio = min(priomove.prio, new_prio)
                }
                cout << "RR10" << endl;
            }
            if(first_silent == NULL){
                first_silent = &(*it);
            }
        }
        if(bad_captures.size() > 0 && count_of_good_captures == 0 && count_of_stormy == 0){
            if(first_silent != NULL){
                first_silent->prio = min(first_silent->prio, (new_prio + first_silent->prio % 10) - 10);
            }
            for(list<cPrioMove>::iterator it = bad_captures.begin(); it != bad_captures.end(); ++it){
                it->prio = min(it->prio, new_prio + it->prio % 10);
            }
        }
        priomoves.sort(sortByPrio);
        return true;
    }

    int select_movecnt(cMatch &match, list<cPrioMove> &priomoves, int depth, cSearchLimits &slimits, cPrioMove *last_pmove){
        if(priomoves.size() == 0 || depth > slimits.dpth_max){
            return 0;
        }
        int count;
        if(depth <= slimits.dpth_stage1 && priomoves.front().has_domain(cTactic::DOMAINS["defends-check"])){
            return priomoves.size();
        }
        int stormycnt = count_up_within_stormy(priomoves);
        if(depth <= slimits.dpth_stage1){
            resort_exchange_or_stormy_moves(priomoves, cPrioMove::PRIOS["prio1"], last_pmove, false);
            count = count_up_to_prio(priomoves, cPrioMove::PRIOS["prio2"]);
            if(count == 0){
                count = slimits.mvcnt_stage1;
            }
            else{
                count = min(count, slimits.mvcnt_stage1);
            }
            return max(stormycnt, count);
        }
        if(depth <= slimits.dpth_stage2){
            resort_exchange_or_stormy_moves(priomoves, cPrioMove::PRIOS["prio1"], last_pmove, false);
            count = count_up_to_prio(priomoves, cPrioMove::PRIOS["prio2"]);
            if(count == 0){
                count = slimits.mvcnt_stage2;
            }
            else{
                count = min(count, slimits.mvcnt_stage2);
            }
            return max(stormycnt, count);
            //if(depth <= slimits.dpth_stage3):
            //    resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS["prio0"], last_pmove, false);
            //    count = count_up_to_prio(priomoves, cPrioMove.PRIOS["prio0"]);
            //    if(count == 0){
            //        count = slimits.mvcnt_stage3;
            //    }
            //    else{
            //        count = min(count, slimits.mvcnt_stage3);
            //    }
            //    return max(stormycnt, count) ;
            //}
        }
        else{
            if(resort_exchange_or_stormy_moves(priomoves, cPrioMove::PRIOS["prio0"], last_pmove, true)){
                return count_up_to_prio(priomoves, cPrioMove::PRIOS["prio0"]);
                // return min(slimits.mvcnt_stage3, count)
                // return min(2, count)
            }
            else{
                return 0;
            }
        }
    }


    void alphabeta(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, int &result_score, list<cPrioMove> &result_candidates){
        list<cPrioMove> newcandidates;
        list<cPrioMove> candidates;
        int newscore;
        int score;
        int count = 0;
        //time_t starttime = time(0);

        if(maximizing){
            score = alpha;
        }
        else{
            score = beta;
        }

        cMove *dbggmove = NULL; //new cMove(0x0, 3, 51, PIECES["blk"]);
        bool search_for_mate = match.is_endgame();
        list<cPrioMove> priomoves;
        generate_priomoves(match, candidate, dbggmove, search_for_mate, priomoves);
        priomoves.sort(sortByPrio);
        int maxcnt = select_movecnt(match, priomoves, depth, slimits, last_pmove);
        
        prnt_priomoves(priomoves);
        cout << priomoves.size();

        if(priomoves.size() == 0 || maxcnt == 0){
            result_score = 0; // score_position(match, priomoves.size());
            // result_candidates->clear();
            return;
        }

        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            count += 1;

            match.do_move(it->src, it->dst, it->prompiece);
            if(maximizing){
                alphabeta(match, depth + 1, slimits, newscore, beta, false, &(*it), NULL, newscore, newcandidates);
            }
            else{
                alphabeta(match, depth + 1, slimits, alpha, newscore, true, &(*it), NULL, newscore, newcandidates);
            }
            match.undo_move();

            if(depth == 1){
                cout << "CURRENT SEARCH: " << dec << newscore << " [" + it->format() + "] " << concat_fmtmoves(newcandidates);
                if(candidates.size() > 0){
                    cout << "CANDIDATE:      " << dec << score << concat_fmtmoves(newcandidates);
                }
            }
            if(maximizing){
                if(newscore > score){
                   score = newscore;
                   if(score >= beta){
                       break; // beta cut-off
                   }
                   else{
                       append_newmove((*it), candidates, newcandidates);
                   }
                }
            }
            else{
                if(newscore < score){
                    score = newscore;
                    if(score <= alpha){
                        break; // alpha cut-off
                    }
                    else{
                        append_newmove((*it), candidates, newcandidates);
                    }
                }
            }
            if(count >= maxcnt){
                break;
            }
        }
        result_score = score;
        for(list<cPrioMove>::iterator it = candidates.begin(); it != candidates.end(); ++it){
            cPrioMove *pmv = new cPrioMove(it->prevfields, it->src, it->dst, it->prompiece, it->prio);
            result_candidates.push_back(*pmv);
        }
    }


    list<cPrioMove> *calc_move(cMatch &match, cPrioMove *candidate){
        time_t time_start = time(0);
        // move = retrieve_move(match)
        int result_score;
        list<cPrioMove> result_candidates;

        //if(move):
        //    candidates.append(move)
        //    score = match.score
        //else:
        cSearchLimits slimits(cMatch::LEVELS["blitz"]);
        bool maximizing = match.next_color() == COLORS["white"];
        int alpha = SCORES[PIECES["wKg"]] * 10;
        int beta = SCORES[PIECES["bKg"]] * 10;

        alphabeta(match, 1, slimits, alpha, beta, maximizing, NULL, candidate, result_score, result_candidates);

        cout << "result: " << result_score << " match: " << match.created_at << " ";
        // cout << concat_fmtmoves(match, result_candidates);
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);
        return &result_candidates;
    }
