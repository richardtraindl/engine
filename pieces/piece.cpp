
#include "./piece.hpp"

    array<int, 8> cPiece::DIRS_ARY = {DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"],
                                      DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"]};

    array<int, 8> cPiece::STEPS = {0, 0, 0, 0, 0, 0, 0, 0};

    int cPiece::MAXCNT = 0;

    array<int, 4> cPiece::PROM_PIECES = {mBLK, mBLK, mBLK, mBLK};

    cPiece::cPiece(cBoard *_board, int _pos){
        board = _board;
        pos = _pos;
        piece = board->getfield(pos);
        color = PIECES_COLORS[piece];
    }

    array<int, 8> cPiece::get_dirs_ary() { return DIRS_ARY; }

    array<int, 8> cPiece::get_steps() { return STEPS; }
    
    array<int, 4> cPiece::get_prom_pieces() { return PROM_PIECES; }
    
    int cPiece::get_maxcnt() { return MAXCNT; }

    int cPiece::dir_for_move(int src, int dst){
        return DIRS["undef"];
    }

    int cPiece::step_for_dir(int dir){
        return 0;
    }

    bool cPiece::is_trapped(){
        if(is_field_touched(board, pos, REVERSED_COLORS[color], EVAL_MODES["only-pins-to-king"])){
            return false;
        }
        for(int step : STEPS){
            int pos1 = pos + step;
            if(board->is_inbounds(pos, pos1, step)){
                int dstpiece = board->getfield(pos1);
                if(PIECES_COLORS[dstpiece] == color){
                    continue;
                }
                else{
                    if(dstpiece != PIECES["blk"] && PIECES_RANKS[piece] <= PIECES_RANKS[dstpiece]){
                        return false;
                    }
                    list<cTouch> frdlytouches, enmytouches;
                    collect_touches_for_both_colors(board, pos1, color, &frdlytouches, &enmytouches);
                    bool enmy_is_lower = false;
                    for(const cTouch enmy : enmytouches){
                        if(PIECES_RANKS[enmy.piece] < PIECES_RANKS[piece]){
                            enmy_is_lower = true;
                            break;
                        }
                    }
                    if(frdlytouches.size() >= enmytouches.size() && enmy_is_lower == false){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool cPiece::is_move_stuck(int dst){
        int mv_dir = dir_for_move(pos, dst);
        int pin_dir = board->eval_pin_dir(pos);
        if(pin_dir == DIRS["undef"] || mv_dir == pin_dir || REVERSE_DIRS[mv_dir] == pin_dir){
            return false;
        }
        else{
            return true;
        }
    }

    bool cPiece::is_move_valid(int dst, int prompiece){
        int dir = dir_for_move(pos, dst);
        if(dir == DIRS["undef"]){
            return false;
        }
        int step = step_for_dir(dir);
        if(step == 0){
            int pin_dir = board->eval_pin_dir(pos);
            for(auto &piecedir : get_dirs_ary()){
                if(piecedir == DIRS["undef"]){
                    break;
                }
                if(dir == piecedir){
                    if(pin_dir != piecedir && pin_dir != REVERSE_DIRS[piecedir] && pin_dir != DIRS["undef"]){
                        return false;
                    }
                }
            }
        }
        int newpos = pos + step;
        while(board->is_inbounds(pos, newpos, 0)){
            int newpiece = board->getfield(newpos);
            if(newpos == dst){
                if(PIECES_COLORS[newpiece] == color){
                    return false;
                }
                else{
                    return true;
                }
            }
            if(newpiece != PIECES["blk"]){
                return false;
            }
            newpos += step;
        }
        return false;
    }

    cMove *cPiece::do_move(int dst, int prompiece, int movecnt, int *score){
        int dstpiece = board->getfield(dst);
        cMove *move = new cMove(board->fields, pos, dst, prompiece);
        board->setfield(move->src, PIECES["blk"]);
        board->setfield(move->dst, piece);
        *score += SCORES[dstpiece];
        return move;
    }

    bool cPiece::undo_move(cMove *move, int movecnt, int *score){
        move->copyprevfields(board->fields);
        int dstpiece_after_undo_mv = board->getfield(move->dst);
        *score -= SCORES[dstpiece_after_undo_mv];
        return true;
    }


    void cPiece::find_attacks_and_supports(list<cTouch> *attacked, list<cTouch> *supported){
        int opp_color = REVERSED_COLORS[color];
        for(const int step : STEPS){
            int dst2 = board->search(pos, step, MAXCNT);
            if(dst2 != PIECES["blk"]){
                if(is_move_stuck(dst2)){
                    continue;
                }
                int piece = board->getfield(dst2);
                if(PIECES_COLORS[piece] == opp_color){
                    cTouch *ctouch = new cTouch(piece, dst2);
                    attacked->push_back(*ctouch);
                    //###
                    board->setfield(dst2, PIECES["blk"]);
                    add_field_touches_beyond(board, opp_color, ctouch);
                    board->setfield(dst2, piece);
                }
                if(PIECES_COLORS[piece] == color){
                    if(piece == PIECES["wKg"] || piece == PIECES["bKg"]){
                        continue;
                    }
                    cTouch *ctouch = new cTouch(piece, dst2);
                    supported->push_back(*ctouch);
                    //###
                    board->setfield(dst2, PIECES["blk"]);
                    add_field_touches_beyond(board, color, ctouch);
                    board->setfield(dst2, piece);
                }
            }
        }
    }


    int cPiece::score_touches(){
        int score = 0;
        list<cTouch>friends, enmies;
        collect_touches_for_both_colors(board, pos, color, &friends, &enmies);
        if(board->eval_pin_dir(pos) != DIRS["undef"]){
            score += ATTACKED_SCORES[piece];
        }
        if(friends.size() < enmies.size()){
            score += ATTACKED_SCORES[piece];
        }
        if(friends.size() > enmies.size()){
            score += SUPPORTED_SCORES[piece];
        }
        return score;
    }

    list<cMove> *cPiece::generate_moves(list<cMove> *minutes){
        list<cMove> *moves = new list<cMove>;
        for(auto &step : get_steps()){
            if(step == 0){ 
                break; 
            }
            int count = 0;
            int dst = pos + step;
            while(board->is_inbounds(pos, dst, step && count < get_maxcnt())){
                count += 1;
                for(auto &prompiece : get_prom_pieces()){
                    if(board->is_move_valid(pos, dst, prompiece, minutes)){
                        moves->push_back(cMove(board->fields, pos, dst, prompiece));
                    }
                    if(prompiece == mBLK){
                        break;
                    }
                }
                dst += step;
            }
        }
        return moves;
    }

    list<cPrioMove> *cPiece::generate_priomoves(list<cMove> *minutes, cMove *candidate, cMove *dbggmove, bool search_for_mate){
        list<cPrioMove> *priomoves = new list<cPrioMove>;
        list<cPrioMove> excludes;
        for(auto &step : get_steps()){
            if(step == 0){ 
                break; 
            }
            int count = 0;
            int dst = pos + step;
            while(board->is_inbounds(pos, dst, step && count < get_maxcnt())){
                cout << "1\n" << endl;
                count += 1;
                for(auto &prompiece : get_prom_pieces()){
                    cout << "2\n" << endl;
                    if(board->is_move_valid(pos, dst, prompiece, minutes)){
                        cout << "3\n" << endl;
                        cPrioMove *priomove = new cPrioMove(board->fields, pos, dst, prompiece, cPrioMove::PRIOS["prio3"]);
                        //excluded = add_tactics(priomove, self.match, candidate, dbggmove, search_for_mate)
                        //if(len(excluded) > 0):
                            //excludes.extend(excluded)
                        priomoves->push_back(*priomove);
                    }
                    if(prompiece == mBLK){
                        break;
                    }
                }
                dst += step;
            }
        }/*
        if(excludes.size() > 0){
            includes = []
            sorted(excludes, key=lambda x: x.tactic.weight)
            for excl in excludes:
                if(includes is None):
                    includes.append(excl)
                else:
                    for incl in includes:
                        if(incl.tactic.domain == excl.tactic.domain and 
                           incl.tactic.addition is not None and
                           incl.tactic.addition == excl.tactic.addition):
                            excl.priomove.downgrade(excl.tactic)
                            excl.priomove.evaluate_priority()
                        else:
                            includes.append(excl)*/
        return priomoves;
    }
