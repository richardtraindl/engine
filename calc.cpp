
    #include "./calc.hpp"


    uint64_t calccnt;


    void prnt_moves(list<cGMove *> &moves){
        uint8_t idx = 1;

        cout << "-------------------------------------------" << endl;

        for(cGMove *move : moves){
            cout << "\n" << dec << to_string(idx) << ". ";
            cout << move->format(false) << endl;
            idx++;
        }
        cout << "-------------------------------------------" << endl;
    }


    string concat_fmtmoves(list<cGMove *> &moves){
        string str_moves = "";
        for(cGMove *move : moves){
            str_moves += " [" + move->format(true) + "] ";
        }
        return str_moves;
    }


    void append_newmove(cGMove *move, list<cGMove *> &newcandidates, list<cGMove *> &rcandidates){
        rcandidates.clear();
        rcandidates.push_back(new cGMove(move->src, move->dst, move->prompiece));
        for(cGMove *newmove : newcandidates){
            rcandidates.push_back(new cGMove(newmove->src, newmove->dst, newmove->prompiece));
        }
    }


    void clean_moves(list<cGMove *> &moves){
        for(cGMove *move : moves){
            delete move;
        }
    }


    void count_limits(list<cGMove *> &moves, uint8_t &badcnt, uint8_t &mediumcnt, uint8_t &highcnt, uint8_t &stormycnt){
        badcnt = 0;
        mediumcnt = 0;
        highcnt = 0;
        stormycnt = 0;

        for(cGMove *move : moves){
            if(move->presort < cGMove::PRESORT_HIGH){
                stormycnt++;
                continue;
            }
            else if(move->presort < cGMove::PRESORT_MEDIUM){
                highcnt++;
                continue;
            }
            else if(move->presort < cGMove::PRESORT_LOW){
                mediumcnt++;
                continue;
            }
            else{
                badcnt++;
                continue;
            }
        }
    }


    bool sortByPresort(cGMove *A, cGMove *B){
        return (A->presort < B->presort);
    }


    uint8_t determine_movecnt(cMatch &match, list<cGMove *> &moves, uint8_t depth, uint8_t maxdepth){
        if(moves.size() == 0){
            return 0;
        }
        else if(depth == 1){
            return moves.size();
        }
        else{ 
            uint8_t badcnt, mediumcnt, highcnt, stormycnt;

            count_limits(moves, badcnt, mediumcnt, highcnt, stormycnt);

            if(depth <= 3){
                if((highcnt + stormycnt) > 12){
                    return (highcnt + stormycnt);
                }
                else{
                    return min(12, (mediumcnt + highcnt + stormycnt));
                }
            }
            else if(depth <= 12){
                bool exchange = false;
                uint8_t idx = 0;
                for(list<cMove>::reverse_iterator it = match.minutes.rbegin(); it != match.minutes.rend(); ++it){
                    if(it->type == MOVE_TYPE["capture"] || it->type == MOVE_TYPE["en-passant"]){
                        exchange = true;
                        break;
                    }
                    ++idx;
                    if(idx >= 2){
                        break;
                    }
                }

                if(exchange){
                    return min(6, (highcnt + stormycnt));
                }
                else{
                    return 0;
                }
            }
            /*else if(depth <= 12){
                if(moves.size() >= 1){ 
                    cMove move = match.minutes.back();
                    if(move.type == MOVE_TYPE["capture"] || move.type == MOVE_TYPE["en-passant"]){
                        return min((stormycnt + 2), (int)moves.size());
                    }
                }

                return stormycnt;
            }*/
            else{
                return 0;
            }
        }
    }


    int16_t alphabeta(cMatch &match, uint8_t depth, uint8_t maxdepth, int16_t alpha, int16_t beta, bool maximizing, list<cGMove *> &rcandidates){
        list<cGMove *> newcandidates;
        int16_t newscore, bestscore;
        uint8_t count = 0;

        if(maximizing){
            bestscore = SCORES[mWKG] * 10;
        }
        else{
            bestscore = SCORES[mBKG] * 10;
        }

        list<cGMove *> moves;
        cGenerator generator(&match);
        generator.gen_moves(moves);
        moves.sort(sortByPresort);
        uint8_t maxcnt = determine_movecnt(match, moves, depth, maxdepth);

        if(depth == 1){
            cout << "moves count: " << moves.size() << " maxcnt: " << to_string(maxcnt) << endl;
            prnt_moves(moves);
        }

        if(moves.size() == 0){
            if(match.next_color() == mBLACK){
                uint64_t kg_pos = match.board.read_bkg_pos();
                if(match.board.is_square_enemy_touched(mWHITE, kg_pos)){
                    return SCORES[mBKG] * 3;
                }
            }
            else{
                uint64_t kg_pos = match.board.read_wkg_pos();
                if(match.board.is_square_enemy_touched(mBLACK, kg_pos)){
                    return SCORES[mWKG] * 3;
                }
            }

            return 0;
        }
        
        if(maxcnt == 0){
            clean_moves(moves);
            return match.score;
        }

        for(cGMove *move : moves){
            ++count;

            newcandidates.clear();

            if(depth == 1){ 
                cout << endl << to_string(count) << ": " << move->format(false) << endl; 
            }
            else if(depth == 2){
                cout << "2*******************************" << endl;
            }
            /*else if(depth == 3){
                cout << "3..............................." << endl;
            }
            else if(depth == 5){
                cout << "5";
            }
            else if(depth == 6){ 
                cout << "6";
            }
            else if(depth == 7){ 
                cout << "7";
            }*/

            // ??? tactical draw
            // clean_moves(moves);

            ++calccnt;

            match.do_move(move->src, move->dst, move->prompiece);

            newscore = alphabeta(match, depth + 1, maxdepth, alpha, beta, !maximizing, newcandidates);

            match.undo_move();

            if(maximizing){
                if(newscore > bestscore){
                    bestscore = newscore;
                    alpha = max(bestscore, alpha);
                    append_newmove(move, newcandidates, rcandidates);
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
                    append_newmove(move, newcandidates, rcandidates);
                    if(depth == 1){
                        cout << "\n!!!CANDIDATE:      " << dec << bestscore << concat_fmtmoves(rcandidates) << endl;
                    }
                    if(beta <= alpha){
                        break;
                    }
                }
            }

            if(count > maxcnt){
                break;
            }
        }

        clean_moves(moves);
        return bestscore;
    }


    int16_t calc_move(cMatch &match, list<cGMove *> &rcandidates){
        time_t time_start = time(0);
        bool maximizing = match.next_color() == mWHITE;
        int16_t alpha = SCORES[mWKG] * 10;
        int16_t beta = SCORES[mBKG] * 10;
        calccnt = 0;

        int16_t rscore = alphabeta(match, 1, 5, alpha, beta, maximizing, rcandidates);

        cout << "\nresult: " << to_string(rscore) << endl;
        cout << concat_fmtmoves(rcandidates) << endl;
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);
        cout << "calccnt: " << to_string(calccnt) << endl;

        return rscore;
    }

