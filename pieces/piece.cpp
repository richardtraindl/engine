
#include "./piece.hpp"


    cPiece::cPiece(cBoard *_board, unsigned _pos){
        board = _board;
        pos = _pos;
        piece = board->getfield(pos);
        color = PIECES_COLOR[piece];
    }

    unsigned cPiece::DIRS_ARY[1] = {0};
    int cPiece::STEPS[1] = {0};
    vector<pair<int, unsigned>> cPiece::MV_STEPS = {make_pair(0, PIECES["blk"])};
    int cPiece::MAXCNT = 7;

    unsigned cPiece::dir_for_move(unsigned src, unsigned dst){
        return DIRS["undef"];
    }

    int cPiece::step_for_dir(unsigned dir){
        return 0;
    }

    bool cPiece::is_trapped(){
        if(is_field_touched(board, pos, REVERSED_COLORS[color], EVAL_MODES["only-pins-to-king"])){
            return false;
        }
        for(const int step : STEPS){
            int pos1 = pos + step;
            if(board->is_inbounds(pos, pos1, step)){
                int dstpiece = board->getfield(pos1);
                if(PIECES_COLOR[dstpiece] == color){
                    continue;
                }
                else{
                    if(dstpiece != PIECES["blk"] && PIECES_RANK[piece] <= PIECES_RANK[dstpiece]){
                        return false;
                    }
                    list<cTouch> frdlytouches, enmytouches;
                    collect_touches_for_both_colors(board, pos1, color, &frdlytouches, &enmytouches);
                    bool enmy_is_lower = false;
                    for(const cTouch enmy : enmytouches){
                        if(PIECES_RANK[enmy.piece] < PIECES_RANK[piece]){
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

    bool cPiece::is_move_stuck(unsigned dst){
        unsigned mv_dir = dir_for_move(pos, dst);
        unsigned pin_dir = board->eval_pin_dir(pos);
        if(pin_dir == DIRS["undef"] || mv_dir == pin_dir || REVERSE_DIRS[mv_dir] == pin_dir){
            return false;
        }
        else{
            return true;
        }
    }

    bool cPiece::is_move_valid(unsigned dst, unsigned prompiece){
        unsigned dir = dir_for_move(pos, dst);
        if(dir == DIRS["undef"]){
            return false;
        }
        int step = step_for_dir(dir);
        if(step == 0){
            unsigned pin_dir = board->eval_pin_dir(pos);
            for(const int piecedir : DIRS_ARY){
                if(dir == piecedir){
                    if(pin_dir != piecedir && pin_dir != REVERSE_DIRS[piecedir] && pin_dir != DIRS["undef"]){
                        return false;
                    }
                }
            }
        }
        int newpos = pos + step;
        while(board->is_inbounds(pos, newpos, 0)){
            unsigned newpiece = board->getfield(newpos);
            if(newpos == dst){
                if(PIECES_COLOR[newpiece] == color){
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

    cMove *cPiece::do_move(unsigned dst, unsigned prompiece, int movecnt, int *score){
        unsigned dstpiece = board->getfield(dst);
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
        unsigned opp_color = REVERSED_COLORS[color];
        for(const int step : STEPS){
            unsigned dst2 = board->search(pos, step, MAXCNT);
            if(dst2 != PIECES["blk"]){
                if(is_move_stuck(dst2)){
                    continue;
                }
                unsigned piece = board->getfield(dst2);
                if(PIECES_COLOR[piece] == opp_color){
                    cTouch *ctouch = new cTouch(piece, dst2);
                    attacked->push_back(*ctouch);
                    //###
                    board->setfield(dst2, PIECES["blk"]);
                    add_field_touches_beyond(board, opp_color, ctouch);
                    board->setfield(dst2, piece);
                }
                if(PIECES_COLOR[piece] == color){
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
        for(vector<pair<int, unsigned>>::iterator it = MV_STEPS.begin(); it != MV_STEPS.end(); ++it){
            int count = 0;
            unsigned dst = pos + it->first;
            while(board->is_inbounds(pos, dst, it->first) && count < MAXCNT){
                count += 1;
                if(board->is_move_valid(pos, dst, it->second, minutes)){
                    moves->push_back(cMove(board->fields, pos, dst, it->second));
                }
                dst += it->first;
            }
        }
        return moves;
    }

    list<cPrioMove> *cPiece::generate_priomoves(list<cMove> *minutes, cMove *candidate, cMove *dbggmove, bool search_for_mate){
        // from ..compute.analyze_move import add_tactics
        list<cPrioMove> *priomoves = new list<cPrioMove>;
        list<cPrioMove> excludes;
        for(vector<pair<int, unsigned>>::iterator it = MV_STEPS.begin(); it != MV_STEPS.end(); ++it){
            int count = 0;
            unsigned dst = pos + it->first;
            while(board->is_inbounds(pos, dst, it->first) && count < MAXCNT){
                count += 1;
                if(board->is_move_valid(pos, dst, it->second, minutes)){
                    cPrioMove *priomove = new cPrioMove(board->fields, pos, dst, it->second, cPrioMove::PRIOS["prio3"]);
                    //excluded = add_tactics(priomove, self.match, candidate, dbggmove, search_for_mate)
                    //if(len(excluded) > 0):
                        //excludes.extend(excluded)
                    priomoves->push_back(*priomove);
                }
                dst += it->first;
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
