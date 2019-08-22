
    #include <iostream>    
    #include "./calc.hpp"
    #include "../pieces/piece.hpp"
    #include "./analyze_position.hpp"
    #include "../helper.hpp"
    #include "../values.hpp"


    cSearchLimits::cSearchLimits(int level){
        if(level == cMatch::LEVELS["blitz"]){
            add_mvcnt = 2;
            dpth_stage1 = 3;
            dpth_stage2 = 5;
            dpth_max = 10;
            mvcnt_stage1 = 8;
            mvcnt_stage2 = 6;
        }
        if(level == cMatch::LEVELS["low"]){
            add_mvcnt = 2;
            dpth_stage1 = 3;
            dpth_stage2 = 7;
            dpth_max = 13;
            mvcnt_stage1 = 10;
            mvcnt_stage2 = 6;
        }
        else{
            add_mvcnt = 2;
            dpth_max = 12;
            dpth_stage1 = 3;
            dpth_stage2 = 5;
            mvcnt_stage1 = 10;
            mvcnt_stage2 = 6;
        }
    }


    void prnt_priomoves(list<cPrioMove> &priomoves){
        cout << "-------------------------------------------" << endl;
        int idx = 1;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            cout << "\n" << dec << idx << ". ";
            cout << it->format() << " prio: " << it->prio << " is_tactic_stormy: " << it->is_tactic_stormy() << endl;
            cout << it->fmt_tactics() << endl;
            idx += 1;
        }
        cout << "-------------------------------------------" << endl;
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
                cPiece cpiece(match.board, idx);
                cpiece.generate_moves(match, moves);
            }
        }
    }


    void generate_priomoves(cMatch &match, cMove *candidate, cMove *dbggmove, list<cPrioMove> &priomoves){
        int color = match.next_color();
        for(int idx = 0; idx < 64; ++idx){
            int piece = match.board.getfield(idx);
            if(piece == mBLK || color != PIECES_COLORS[piece]){
                continue;
            }
            else{
                cPiece cpiece(match.board, idx);
                cpiece.generate_priomoves(match, candidate, dbggmove, priomoves);
            }
        }
    }


    void append_newmove(cPrioMove &move, list<cPrioMove> &newcandidates, list<cPrioMove> &rcandidates){
        rcandidates.clear();
        rcandidates.push_back(move);
        for(list<cPrioMove>::iterator it = newcandidates.begin(); it != newcandidates.end(); ++it){
            rcandidates.push_back((*it));
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


    bool sortByPrio(cPrioMove &A, cPrioMove &B){
        return (A.prio < B.prio);
    }


    bool resort_exchange_moves(list<cPrioMove> &priomoves, int new_prio, cPrioMove *last_pmove, bool with_stormy){
        if(with_stormy == false && 
           (last_pmove == NULL || (last_pmove != NULL && last_pmove->has_domain(cTactic::DOMAINS["captures"]) == false))){
            return false;
        }
        list<cPrioMove> goodes;
        cPrioMove *first_silent = NULL;
        cPrioMove *first_bad_capture = NULL;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            if(with_stormy == true && it->is_tactic_stormy()){
                goodes.push_back(*it);
                continue;
            }
            if(it->has_domain(cTactic::DOMAINS["captures"])){
                if(it->fetch_weight(cTactic::DOMAINS["captures"]) == cTactic::WEIGHTS["bad-deal"]){
                    if(first_bad_capture == NULL){
                        first_bad_capture = &(*it);
                    }
                }
                else{
                    goodes.push_back(*it);
                }
            }
            else{
                if(first_silent == NULL){
                    first_silent = &(*it);
                }
            }
        }
        if(goodes.size() > 0){
            for(list<cPrioMove>::iterator it = goodes.begin(); it != goodes.end(); ++it){
                it->prio = min(it->prio, (new_prio + it->prio % 10) - 20);
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


    int select_movecnt(cMatch &match, list<cPrioMove> &priomoves, int depth, cSearchLimits &slimits, cPrioMove *last_pmove){
        int count;
        if(priomoves.size() == 0 || depth > slimits.dpth_max){
            return 0;
        }
        if(depth <= slimits.dpth_stage1 && priomoves.front().has_domain(cTactic::DOMAINS["defends-check"])){
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


    void clean_priomoves(list<cPrioMove> &priomoves){
        cPrioMove *priomove;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            priomove = &(*it);
            delete priomove;
        }
    }


    int alphabeta(cMatch &match, int depth, cSearchLimits &slimits, int alpha, int beta, bool maximizing, cPrioMove *last_pmove, cPrioMove *candidate, list<cPrioMove> &rcandidates){
        list<cPrioMove> newcandidates;
        int newscore;
        int bestscore;
        int count = 0;
        
        if(maximizing){
            bestscore = SCORES[mWKG] * 10;;
        }
        else{
            bestscore = SCORES[mBKG] * 10;;
        }

        cMove *dbggmove = NULL; //new cMove(0x0, coord_to_index("d1"), coord_to_index("d7"), mBLK);
        list<cPrioMove> priomoves;
        generate_priomoves(match, candidate, dbggmove, priomoves);
        priomoves.sort(sortByPrio);
        int maxcnt = select_movecnt(match, priomoves, depth, slimits, last_pmove);
        if(depth == 1){
            cout << "priomoves count: " << priomoves.size() << endl;
            prnt_priomoves(priomoves);
        }

        if(priomoves.size() == 0 || maxcnt == 0){
            return score_position(match, priomoves.size());
            //if(match.board.fields == 0x60000011100D1105000000000010000300900000000900900004090000EB0C_cppui){
                //match.prnt_minutes();
            //}
        }

        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            count += 1;
            newcandidates.clear();

            if(depth == 1){
                cout << "\nCURRENT SEARCH: " << " [" + it->format() + "] " << concat_fmtmoves(newcandidates) << endl;
            }
            if(depth > slimits.mvcnt_stage2){
                cout << ".";
            }

            match.do_move(it->src, it->dst, it->prompiece);
            newscore = alphabeta(match, depth + 1, slimits, alpha, beta, !maximizing, &(*it), NULL, newcandidates);
            match.undo_move();

            if(maximizing){
                if(newscore > bestscore){
                    bestscore = newscore;
                    alpha = max(bestscore, alpha);
                    append_newmove((*it), newcandidates, rcandidates);
                    if(depth == 1){
                        cout << "\nCANDIDATE:      " << dec << bestscore << concat_fmtmoves(rcandidates) << endl;
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
                    append_newmove((*it), newcandidates, rcandidates);
                    if(depth == 1){
                        cout << "\nCANDIDATE:      " << dec << bestscore << concat_fmtmoves(rcandidates) << endl;
                    }
                    if(beta <= alpha){
                        break;
                    }
                }
            }
            if(count >= maxcnt){
                break;
            }
        }
        return bestscore;
    }


    int calc_move(cMatch &match, cPrioMove *candidate, list<cPrioMove> &rcandidates){
        time_t time_start = time(0);
        // move = retrieve_move(match)
        int score;

        //if(move):
        //    candidates.append(move)
        //    score = match.score
        //else:
        cSearchLimits slimits(cMatch::LEVELS["blitz"]);
        bool maximizing = match.next_color() == COLORS["white"];
        int alpha = SCORES[mWKG] * 10;
        int beta = SCORES[mBKG] * 10;
        score = alphabeta(match, 1, slimits, alpha, beta, maximizing, NULL, candidate, rcandidates);

        cout << "result: " << score << " match: " << match.created_at << " ";
        cout << concat_fmtmoves(rcandidates) << endl;
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);
        return score;
    }
