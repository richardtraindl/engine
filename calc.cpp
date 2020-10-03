
    #include "./calc.hpp"


    uint64_t calccnt;


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


    uint8_t count_up_to_limit(list<cGMove> &moves, uint8_t limit){
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


    uint8_t determine_movecnt(cMatch &match, list<cGMove> &moves, uint8_t depth, uint8_t maxdepth){
        if(moves.size() == 0){
            return 0;
        }

        if(depth <= 2){
            return moves.size();
        }

        if(depth <= 4){
            uint8_t count = count_up_to_limit(moves, cGMove::PRESORT_HIGH);
            uint8_t mincnt = min(3, (int)moves.size());

            if(count > 0){ 
                return max(mincnt, count);
            }
            else{
                return mincnt;
            }
        }

        if(depth <= 7){
            uint8_t silentcnt = 1;

            if(moves.size() >= 1){ 
                cMove move = match.minutes.back();
                if(move.type != MOVE_TYPE["capture"] && move.type != MOVE_TYPE["en-passant"]){
                    silentcnt = 0;
                }
            }

            uint8_t count = count_up_to_limit(moves, cGMove::PRESORT_STORMY);
            if(count > 0){
                if((count + silentcnt) <= (uint8_t)moves.size()){
                    return count + silentcnt;
                }
                else{
                    return count;
                }
            }
        }

        return 0;
    }


    int16_t alphabeta(cMatch &match, uint8_t depth, uint8_t maxdepth, int16_t alpha, int16_t beta, bool maximizing, list<cGMove> &rcandidates){
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
            return match.score;
        }

        for(cGMove move : moves){
            count++;

            newcandidates.clear();

            if(depth == 1){ 
                cout << endl << to_string(count) << ": " << move.format() << endl; 
            }
            /*switch(depth){
                case 9: cout << "."; break;
                case 7: cout << "?"; break;
                case 5: cout << "!"; break;
                case 1: 
                    cout << endl << to_string(count) << ": " << move.format() << endl;
                    break;
            }*/

            // ??? tactical draw

            calccnt += 1;

            int16_t tmpscore = match.score;
            match.do_move(move.src, move.dst, move.prompiece);

            newscore = alphabeta(match, depth + 1, maxdepth, alpha, beta, !maximizing, newcandidates);

            match.undo_move();
            if(tmpscore != match.score){
                cout << endl << to_string(tmpscore) << " " << to_string(match.score);
                cout << concat_fmtmoves(rcandidates) << endl;
            }
            

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

        return bestscore;
    }


    int16_t calc_move(cMatch &match, list<cGMove> &rcandidates){
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

