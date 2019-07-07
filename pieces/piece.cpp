
#include "./piece.hpp"
#include "./searchforpiece.hpp"
#include "../values.hpp"
#include "../helper.hpp"


    cPiece::cPiece(cMatch *_match, int _pos){
        match = _match;
        pos = _pos;
        piece = match->board.getfield(pos);
        color = match->color_of_piece(piece);
    }

    int cPiece::DIRS_ARY[1] = {0};
    int cPiece::STEPS[1] = {0};
    int cPiece::MV_STEPS[1] = {0};
    int cPiece::MAXCNT = 7;

    int cPiece::dir_for_move(int src, int dst){
        return DIRS["undef"];
    }

    int cPiece::step_for_dir(int dir){
        return 0;
    }

    bool cPiece::is_trapped(){
        if(is_field_touched(match, pos, REVERSED_COLORS[color], match->EVAL_MODES["only-pins-to-king"])){
            return false;
        }
        for(const int step : STEPS){
            int pos1 = pos + step;
            if(match->board.is_inbounds(pos, pos1, step)){
                int dstpiece = match->board.getfield(pos1);
                if(match->color_of_piece(dstpiece) == color){
                    continue;
                }
                else{
                    if(dstpiece != PIECES["blk"] && PIECES_RANK[piece] <= PIECES_RANK[dstpiece]){
                        return false;
                    }
                    list<cTouch> frdlytouches, enmytouches;
                    field_touches_for_both(match, pos1, color, &frdlytouches, &enmytouches);
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

    bool cPiece::is_move_stuck(int dst){
        int mv_dir = dir_for_move(pos, dst);
        int pin_dir = 0; // match->eval_pin_dir(pos);
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
            int pin_dir = 0; // match->eval_pin_dir(pos);
            for(const int piecedir : DIRS_ARY){
                if(dir == piecedir){
                    if(pin_dir != piecedir && pin_dir != REVERSE_DIRS[piecedir] && pin_dir != DIRS["undef"]){
                        return false;
                    }
                }
            }
        }
        int newpos = pos + step;
        while(match->board.is_inbounds(pos, newpos, 0)){
            int newpiece = match->board.getfield(newpos);
            if(newpos == dst){
                if(match->color_of_piece(newpiece) == color){
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

    cMove cPiece::do_move(int dst, int prompiece, int movecnt){
        int dstpiece = match->board.getfield(dst);
        cMove move = cMove(match->board.fields, pos, dst, prompiece);
        match->board.setfield(move.src, PIECES["blk"]);
        match->board.setfield(move.dst, piece);
        match->score += SCORES[dstpiece];
        return move;
    }

    bool cPiece::undo_move(cMove *move, int movecnt){
        move->copyprevfields(match->board.fields);
        int dstpiece_after_undo_mv = match->board.getfield(move->dst);
        match->score -= SCORES[dstpiece_after_undo_mv];
        return true;
    }

/*
    def find_attacks_and_supports(self, attacked, supported):
        opp_color = REVERSED_COLORS[self.color]
        for step in self.STEPS:
            dst2 = self.match.board.search(self.pos, step, self.MAXCNT)
            if(dst2 is not None):
                if(self.is_move_stuck(dst2)):
                    continue
                piece = self.match.board.getfield(dst2)
                if(self.match.color_of_piece(piece) == opp_color):
                    ctouch = cTouch(piece, dst2)
                    attacked.append(ctouch)
                    ###
                    self.match.board.setfield(dst2, PIECES['blk'])
                    list_field_touches_beyond(self.match, ctouch, opp_color)
                    self.match.board.setfield(dst2, piece)
                    ###
                elif(self.match.color_of_piece(piece) == self.color):
                    if(piece == PIECES['wKg'] or piece == PIECES['bKg']):
                        continue
                    ctouch = cTouch(piece, dst2)
                    supported.append(ctouch)
                    ###
                    self.match.board.setfield(dst2, PIECES['blk'])
                    list_field_touches_beyond(self.match, ctouch, self.color)
                    self.match.board.setfield(dst2, piece)
                    ###

    def score_touches(self):
        match = self.match
        score = 0
        frdlytouches, enmytouches = list_all_field_touches(match, self.pos, self.color)
        if(match.is_soft_pin(self.pos)[0]):
            score += ATTACKED_SCORES[self.piece]
        if(len(frdlytouches) < len(enmytouches)):
            score += ATTACKED_SCORES[self.piece]
        elif(len(frdlytouches) > len(enmytouches)):
            score += SUPPORTED_SCORES[self.piece]
        return score

    def generate_moves(self, candidate, dbggmove, search_for_mate, mode):
        from ..compute.analyze_move import add_tactics
        moves = []
        for step in self.MV_STEPS:
            count = 0
            excludes = []
            dst = self.pos + step[0]
            while(self.match.board.is_inbounds(self.pos, dst, step[0]) and count < self.MAXCNT):
                count += 1
                flag, errcode = self.match.is_move_valid(self.pos, dst, step[1])
                if(flag):
                    if(mode):
                        move = cMove(self.match.board.fields, self.pos, dst, step[1])
                        priomove = cPrioMove(move)
                        excluded = add_tactics(priomove, self.match, candidate, dbggmove, search_for_mate)
                        if(len(excluded) > 0):
                            excludes.extend(excluded)
                        moves.append(priomove)
                    else:
                        moves.append(cMove(self.match.board.fields, self.pos, dst, step[1]))
                dst += step[0]

        if(mode and len(excludes) > 0):
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
                            includes.append(excl)

        return moves

# class end
*/