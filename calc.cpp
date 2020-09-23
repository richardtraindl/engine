
    #include "./calc.hpp"


    void prnt_moves(list<cGMove> &moves){
        uint8_t idx = 1;

        cout << "-------------------------------------------" << endl;

        for(cGMove move : moves){
            cout << "\n" << dec << to_string(idx) << ". ";
            cout << move.format() << endl;
            idx++;
        }
        cout << "-------------------------------------------" << endl;
    }


    string concat_fmtmoves(list<cGMove> &moves){
        string str_moves = "";
        for(cGMove move : moves){
            str_moves += " [" + move.format() + "] ";
        }
        return str_moves;
    }


    void append_newmove(cGMove &move, list<cGMove> &newcandidates, list<cGMove> &rcandidates){
        rcandidates.clear();
        rcandidates.push_back(move);
        for(cGMove newmove : newcandidates){
            rcandidates.push_back(newmove);
        }
    }


    uint8_t count_up_to_prio(list<cGMove> &moves, uint8_t limit){
        uint8_t count = 0;

        for(cGMove move : moves){
            if(move.presort <= limit){
                count++;
            }
            else{
                break;
            }
        }
        return count;
    }


    bool sortByPresort(cGMove &A, cGMove &B){
        return (A.presort < B.presort);
    }


    uint8_t select_movecnt(cMatch &match, list<cGMove> &moves, uint8_t depth, cGMove *last_move){
        if(moves.size() == 0){
            return 0;
        }

        uint8_t movecnt = 0;
        if(depth <= 3){
            return moves.size();
        }

        if(depth <= 5){
            movecnt = count_up_to_prio(moves, cGMove::PRESORT_HIGH);
            return movecnt;
        }

        movecnt = count_up_to_prio(moves, cGMove::PRESORT_VERY_HIGH);
        return movecnt;
    }


    /*void clean_moves(list<cGMove> &moves){
        for(cGMove move : moves){
            delete move;
        }
    }*/


    int16_t alphabeta(cMatch &match, uint8_t depth, uint8_t maxdepth, int16_t alpha, int16_t beta, bool maximizing, cGMove *last_move, list<cGMove> &rcandidates){
        list<cGMove> newcandidates;
        int16_t newscore, bestscore;
        int count = 0;

        if(maximizing){
            bestscore = SCORES[mWKG] * 10;
        }
        else{
            bestscore = SCORES[mBKG] * 10;
        }

        list<cGMove> moves;
        cGenerator generator(&match);
        generator.gen_moves(moves);
        moves.sort(sortByPresort);
        uint8_t maxcnt = select_movecnt(match, moves, depth, last_move);

        if(depth == 1){
            cout << "moves count: " << moves.size() << " maxcnt: " << endl;
            prnt_moves(moves);
        }

        if(moves.size() == 0 || maxcnt == 0){
            bestscore = 0; // score_position(match, priomoves.size());
            //clean_moves(moves);
            return bestscore;
        }

        for(cGMove move : moves){
            count++;

            newcandidates.clear();

            // ??? tactical draw

            match.do_move(move.src, move.dst, move.prompiece);
            newscore = alphabeta(match, depth + 1, maxdepth, alpha, beta, !maximizing, &move, newcandidates);
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
        }
        //clean_moves(moves);
        return bestscore;
    }


    int16_t calc_move(cMatch &match, list<cGMove> &rcandidates){
        time_t time_start = time(0);

        bool maximizing = match.next_color() == mWHITE;
        int16_t alpha = SCORES[mWKG] * 10;
        int16_t beta = SCORES[mBKG] * 10;
        int16_t rscore = alphabeta(match, 1, 5, alpha, beta, maximizing, NULL, rcandidates);
        
        cout << "\nresult: " << rscore << " match: " << match.created_at << " ";
        cout << concat_fmtmoves(rcandidates) << endl;
        prnt_fmttime("\ncalc-time: ", time(0) - time_start);

        return rscore;
    }

