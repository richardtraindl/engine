
#include "./calc.hpp"

    void prnt_priomoves(cMatch *match, list<cPrioMove> *priomoves){
        cout << "------------------------------------------------\n ";
        int idx = 1;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            cout << idx << ". ";
            cout << it->format() << " prio: " << it->prio << " is_tactic_stormy: " << it->is_tactic_stormy() << "\n";
            cout << it->concat_fmttactics() << "\n";
            idx += 1;
        cout << "------------------------------------------------\n ";
    }


    void prnt_search(cMatch *match, string label, int score, cMove *move, int candidates[]){
        string str_gmove;
        if(move != NULL){
            str_gmove = " [" + move->format() + "] ";
        else:
            str_gmove = "";
        cout << label << score << str_gmove << concat_fmtmoves(match, candidates));
    }


    sting concat_fmtmoves(cMatch match, list<cMove> *moves){
        string str_gmoves = "";
        for(list<cMove>::iterator it = moves.begin(); it != moves.end(); ++it){
            if(*it != NULL){
                str_moves += " [" + it->format() + "] ";
        return str_moves;
    }


    list<cPrioMove> generate_moves(cMatch *match, int candidate, cMove *dbggmove, bool search_for_mate, int mode){
        int color = match.next_color();
        list<cPrioMove> moves;
        for(int idx = 0; idx < 64; ++i){
            int piece = match->board.getfield(idx);
            if(piece == PIECES['blk'] || color != match->color_of_piece(piece)){
                continue;
            }
            else{
                cPiece cpiece = match->obj_for_piece(piece, idx);
                cpiece.generate_moves(candidate, dbggmove, search_for_mate, mode, moves);
            }
        return moves;
    }


    void append_newmove(cMove *move, list<cMove> candidates, list<cMove> newcandidates){
        // candidates.clear()
        candidates.push_back(move);
        for(list<cMove>::iterator it = newcandidates.begin(); it != newcandidates.end(); ++it){
            candidates.push_back(newcandidate);


    void alphabeta(cMatch *match, int depth, cSlimits *slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, int *result_score, list<cPrioMove> *result_candidates){
        int color = match->next_color();
        list<cPrioMove> newcandidates;
        int newscore;
        int score;
        int count = 0;
        time_t starttime = time.time();

        if(maximizing):
            score = alpha;
        else:
            score = beta;

        cMove dbggmove = cMove(0x0, 3, 51, PIECES["blk"]);
        bool search_for_mate = match->is_endgame();
        list<cPrioMove> priomoves = generate_moves(match, candidate, dbggmove, search_for_mate, true);
        // priomoves.sort(key = attrgetter('prio'))
        int maxcnt = select_movecnt(match, priomoves, depth, slimits, last_pmove);

        if(priomoves.size() == 0 || maxcnt == 0){
            *result_score = score_position(match, priomoves.size();
            for(list<cMove>::iterator it = candidates.begin(); it != candidates.end(); ++it){                          
                result_candidates->push_back(*it);
            }
        }

        for(list<cMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            count += 1

            match->do_move(it->src, it->dst, it->prompiece);
            if(maximizing){
                newscore, newcandidates = alphabeta(match, depth + 1, slimits, maxscore, beta, False, priomove, None);
            }
            else{
                newscore, newcandidates = alphabeta(match, depth + 1, slimits, alpha, minscore, True, priomove, None);
            }
            match->undo_move();

            if(depth == 1){
                prnt_search(match, "CURRENT SEARCH: ", newscore, move, newcandidates);
                if(candidates != NULL){
                    prnt_search(match, "CANDIDATE:      ", score, None, candidates);

            if(maximizing){
                if(newscore > core){
                   score = newscore;
                   if(score >= beta){
                       break; // beta cut-off
                   }
                   else{
                       append_newmove(move, candidates, newcandidates);
                   }
                }
            else{
                if(newscore < score){
                    score = newscore;
                    if(score <= alpha){
                        break; // alpha cut-off
                    }
                    else{
                        append_newmove(move, candidates, newcandidates);
                    }
                }
            }
            if(count >= maxcnt){
                break;

        *result_score = score;
        for(list<cMove>::iterator it = candidates.begin(); it != candidates.end(); ++it){                          
            result_candidates->push_back(*it);
        }
    }

    int SearchLimitsLevelBlitz::add_mvcnt = 2;
    int SearchLimitsLevelBlitz::dpth_max = 8;
    int SearchLimitsLevelBlitz::dpth_stage1 = 2;
    int SearchLimitsLevelBlitz::dpth_stage2 = 4;
    int SearchLimitsLevelBlitz::mvcnt_stage1 = 6;
    int SearchLimitsLevelBlitz::mvcnt_stage2 = 6;

    int SearchLimitsLevelLow::add_mvcnt = 2;
    int SearchLimitsLevelLow::dpth_max = 12;
    int SearchLimitsLevelLow::dpth_stage1 = 2;
    int SearchLimitsLevelLow::dpth_stage2 = 5;
    int SearchLimitsLevelLow::mvcnt_stage1 = 8;
    int SearchLimitsLevelLow::mvcnt_stage2 = 6;

    int SearchLimitsLevelMedium::add_mvcnt = 2;
    int SearchLimitsLevelMedium::dpth_max = 16;
    int SearchLimitsLevelMedium::dpth_stage1 = 2;
    int SearchLimitsLevelMedium::dpth_stage2 = 6;
    int SearchLimitsLevelMedium::mvcnt_stage1 = 10;
    int SearchLimitsLevelMedium::mvcnt_stage2 = 6;

    int SearchLimitsLevelHigh::add_mvcnt = 2;
    int SearchLimitsLevelHigh::dpth_max = 20;
    int SearchLimitsLevelHigh::dpth_stage1 = 3;
    int SearchLimitsLevelHigh::dpth_stage2 = 6;
    int SearchLimitsLevelHigh::mvcnt_stage1 = 12;
    int SearchLimitsLevelHigh::mvcnt_stage2 = 6;


    int count_up_to_prio(list<cPrioMove> *priomoves, int priolimit){
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


    int count_up_within_stormy(list<cPrioMove> *priomoves){
        int count = 0;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            if(it->is_tactic_stormy()){
                count += 1;
            }
        }
        return count;
    }


    bool resort_exchange_or_stormy_moves(list<cPrioMove> *priomoves, int new_prio, cPrioMove *last_pmove, bool only_exchange){
        if(only_exchange && last_pmove != NULL && last_pmove->has_domain(cTactic.DOMAINS["captures"]) == false){
            return false;
        }
        int last_pmove_capture_bad_deal;
        if(last_pmove != NULL && last_pmove->has_tactic_ext(cTactic(cTactic.DOMAINS["captures"], cTactic.WEIGHTS["bad-deal"]))){
            last_pmove_capture_bad_deal = true;
        }
        else{
            last_pmove_capture_bad_deal = false;
        }
        int count_of_stormy = 0;
        int count_of_good_captures = 0;
        cPrioMove first_silent = NULL;
        list<cPrioMove> bad_captures;
        for(list<cMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            if(only_exchange == false && it->is_tactic_stormy()){
                count_of_stormy += 1;
                it->prio = min(it->prio, (new_prio + it->prio % 10) - 13);
                continue;
            }
            if(it->has_domain(cTactic.DOMAINS["captures"])){
                weight = (it->fetch_weight(cTactic.DOMAINS["captures"]);
                if(weight > cTactic.WEIGHTS["bad-deal"]){
                    count_of_good_captures += 1;
                    (it->prio = min((it->prio, (new_prio  + (it->prio % 10) - 12);
                }
                if(last_pmove_capture_bad_deal){
                    bad_captures.push_back(*it);
                    // count_of_bad_captures += 1
                    // priomove.prio = min(priomove.prio, new_prio)
                }
            if(first_silent == NULL){
                first_silent = *it;
            }
        }
        if(bad_captures.size() > 0 && count_of_good_captures == 0 && count_of_stormy == 0){
            if(first_silent != NULL){
                first_silent.prio = min(first_silent.prio, (new_prio + first_silent.prio % 10) - 10);
            }
            for(list<cMove>::iterator it = bad_captures.begin(); it != bad_captures.end(); ++it){
                it->prio = min(it->prio, new_prio + it->prio % 10);
            }
        }
        // priomoves.sort(key=attrgetter('prio'))
        return true;
    }


    int select_movecnt(cMatch *match, list<cPrioMove> *priomoves, int depth, cSlimits *slimits, cPrioMove *last_pmove){
        if(priomoves.size() == 0 || depth > slimits->dpth_max){
            return 0;
        }
        int count;
        if(depth <= slimits->dpth_stage1 && (*priomoves).has_domain(cTactic.DOMAINS["defends-check"])){
            return priomoves.size();
        }
        int stormycnt = count_up_within_stormy(priomoves);
        if(depth <= slimits->dpth_stage1){
            resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS["prio1"], last_pmove, false);
            count = count_up_to_prio(priomoves, cPrioMove.PRIOS["prio2"]);
            if(count == 0){
                count = slimits->mvcnt_stage1;
            }
            else{
                count = min(count, slimits->mvcnt_stage1);
            }
            return max(stormycnt, count);
        if(depth <= slimits->dpth_stage2){
            resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS["prio1"], last_pmove, false);
            count = count_up_to_prio(priomoves, cPrioMove.PRIOS["prio2"]);
            if(count == 0){
                count = slimits.mvcnt_stage2;
            }
            else{
                count = min(count, slimits.mvcnt_stage2);
            }
            return max(stormycnt, count);
            /*
            if(depth <= slimits.dpth_stage3):
                resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS["prio0"], last_pmove, false);
                count = count_up_to_prio(priomoves, cPrioMove.PRIOS["prio0"]);
                if(count == 0){
                    count = slimits->mvcnt_stage3;
                }
                else{
                    count = min(count, slimits.mvcnt_stage3);
                }
                return max(stormycnt, count) ;
            }
            */
        else:
            if(resort_exchange_or_stormy_moves(priomoves, cPrioMove.PRIOS["prio0"], last_pmove, True)){
                return count_up_to_prio(priomoves, cPrioMove.PRIOS["prio0"]);
                // return min(slimits.mvcnt_stage3, count)
                // return min(2, count)
            }
            else{
                return 0;
            }
        }
    }


    string concat_fmtmoves(cMatch *match, list<cPrioMove> *moves){
        string str_moves = "";
        for(list<cMove>::iterator it = moves.begin(); it != moves.end(); ++it){
            str_moves += " [" + it->format() + "] ";
        return str_moves;
    }

    void prnt_fmttime(string msg, int seconds){
        int sec, minute, hour;
        hour = seconds / (60 * 60);
        int remaining = seconds % (60 * 60);
        min = remaining / 60;
        sec = remaining % 60;
        print( msg + "%02d:%02d:%02d" % (hour, minute, sec));
    }


    list<cPrioMove> *calc_move(cMatch *match, cMove *candidate){
        t_time time_start = time(0);
        // move = retrieve_move(match)
        list<cPrioMove> candidates;

        //if(move):
        //    candidates.append(move)
        //    score = match.score
        //else:
        cSearchLimits slimits = cSearchLimits(match);
        bool maximizing = match->next_color() == COLORS["white"];
        int alpha = SCORES[PIECES["wKg"]] * 10;
        int beta = SCORES[PIECES["bKg"]] * 10;
        int result_score;
        list<cPrioMove> result_candidates;

        alphabeta(match, depth, slimits, alpha, beta, maximizing, NULL, NULL, &result_score, &result_candidates);

        cout << "result: " << score << " match: " << match->created_at << " ";
        cout << concat_fmtmoves(match, candidates));
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);
        return candidates
    }
