
#include "./match.hpp"
#include "./helper.hpp"
#include "./values.hpp"
// #include <Python.h>

using namespace std;


    cMatch::cMatch(){ 
        created_at = time(0); 
        status = STATUS["active"];
        score = 0;
        level = LEVELS["blitz"];
        // board = new cBoard(); 
    } 

    void cMatch::update_attributes(){
        for(list<cMove>::iterator it = minutes.begin(); it != minutes.end(); ++it){
            if(board.wKg_first_move_on == -1 && 
               it->srcx == board.COLS["E"] && it->srcy == board.RANKS["1"]){
                board.wKg_first_move_on = movecnt();
                continue;
            }
            if(board.bKg_first_move_on == -1 && 
               it->srcx == board.COLS["E"] && it->srcy == board.RANKS["8"]){
                board.bKg_first_move_on = movecnt();
                continue;
            }
            if(board.wRkA_first_move_on == -1 && 
               it->srcx == board.COLS["A"] && it->srcy == board.RANKS["1"]){
                board.wRkA_first_move_on = movecnt();
                continue;
            }
            if(board.wRkH_first_move_on == -1 && 
               it->srcx == board.COLS["H"] && it->srcy == board.RANKS["1"]){
                board.wRkH_first_move_on = movecnt();
                continue;
            }
            if(board.bRkA_first_move_on == -1 && 
               it->srcx == board.COLS["A"] && it->srcy == board.RANKS["8"]){
                board.bRkA_first_move_on = movecnt();
                continue;
            }
            if(board.bRkH_first_move_on == -1 && 
               it->srcx == board.COLS["H"] && it->srcy == board.RANKS["8"]){
                board.bRkH_first_move_on = movecnt();
                continue;
            }
        }
        score = 0;
        for(int y = 0; y < 8; ++y){
            for(int x = 0; x < 8; ++x){
                int piece = board.fields[y][x];
                score -= SCORES[piece];
                if(piece == PIECES["wKg"]){
                    board.wKg_x = x;
                    board.wKg_y = y;
                    continue;
                }
                if(piece == PIECES["bKg"]){
                    board.bKg_x = x;
                    board.bKg_y = y;
                }
            }
        }
    }

    int cMatch::color_of_piece(int piece){
        return PIECES_COLOR[piece];
    }

    int cMatch::oppcolor_of_piece(int piece){
        int color = PIECES_COLOR[piece];
        return REVERSED_COLORS[color];
    }

    int cMatch::movecnt(){
        return minutes.size();
    }

    int cMatch::next_color(){
        if(minutes.size() % 2 == 0){
            return COLORS["white"];
        }
        else{
            return COLORS["black"];
        }
    }

    bool cMatch::is_opening(){
        return minutes.size() <= 20;
    }

    bool cMatch::is_endgame(){
        return minutes.size() >= 60;
    }



/*

    # 100 Züge davor kein Bauernzug und keine Figur geschlagen
    def is_fifty_moves_rule(self):
        cnt = 0
        maxlen = len(self.minutes)
        for move in reversed(self.minutes):
            srcpiece = move.getprevfield(move.src)
            dstpiece = move.getprevfield(move.dst)
            if(srcpiece == PIECES['wPw'] or srcpiece == PIECES['bPw'] or
               dstpiece != PIECES['blk']):
                cnt = 0
            else:
                cnt += 1
                if(cnt > 100):
                    return True
                if(maxlen - cnt < 100):
                    return False
        return False

    def is_move_repetition(self):
        newmatch = copy.deepcopy(self)
        board = newmatch.board.fields
        count = 0
        for i in range(8):
            if(newmatch.undo_move() is None):
                return count >= 2
            if(board == newmatch.board.fields):
                count += 1
        return count >= 2

    def is_move_valid(self, src, dst, prompiece):
        piece = self.board.getfield(src)
        cpiece = self.obj_for_piece(piece, src)
        if(cpiece is None):
            return False, self.RETURN_CODES['general-error']
        ###
        direction = cpiece.dir_for_move(src, dst)
        step = cpiece.step_for_dir(direction)
        if(not self.board.is_inbounds(src, dst, step)):
            return False, self.RETURN_CODES['out-of-bounds']
        if(self.next_color() != self.color_of_piece(piece)):
            return False, self.RETURN_CODES['wrong-color']
        if(piece != PIECES['wKg'] and piece != PIECES['bKg']):
            if(self.is_king_after_move_attacked(src, dst)):
                return False, self.RETURN_CODES['king-attacked-error']
        if(cpiece.is_move_valid(dst, prompiece)):
            return True, self.RETURN_CODES['ok']
        else:
            return False, self.RETURN_CODES['piece-error']

    def is_soft_pin(self, src):
        piece = self.board.getfield(src)
        color = self.color_of_piece(piece)
        opp_color = self.oppcolor_of_piece(piece)
        faces = [[cSearchForRook, cRook], [cSearchForBishop, cBishop]]
        for i in range(2):
            enemies = faces[i][0].list_field_touches(self, src, opp_color)        
            for enemy in enemies:
                enemy_dir = faces[i][1].dir_for_move(src, enemy.field)
                step = faces[i][1].step_for_dir(REVERSE_DIRS[enemy_dir])
                if(step is not None):
                    dst = self.board.search(src, step, faces[i][1].MAXCNT)
                    if(dst is not None):
                        friend = self.board.getfield(dst)
                        if(self.color_of_piece(friend) == color and 
                           PIECES_RANK[friend] > PIECES_RANK[piece] and 
                           PIECES_RANK[friend] > PIECES_RANK[enemy.piece]):
                            return True, enemy_dir
            enemies.clear()
        return False, None

    def is_king_after_move_attacked(self, src, dst):
        srcpiece = self.board.getfield(src)
        if(self.color_of_piece(srcpiece) == COLORS['white']):
            kg = self.board.wKg
        else:
            kg = self.board.bKg
        pawnenmy = None
        if(srcpiece == PIECES['wPw']):
            cpawn = cWhitePawn(self, src)
            if(cpawn.is_ep_move_ok(dst)):
                pawnenmy = self.board.getfield(dst)
                self.board.setfield(dst, PIECES['blk'])
        elif(srcpiece == PIECES['bPw']):
            cpawn = cBlackPawn(self, src)
            if(cpawn.is_ep_move_ok(dst)):
                pawnenmy = self.board.getfield(dst)
                self.board.setfield(dst, PIECES['blk'])
        self.board.setfield(src, PIECES['blk'])
        dstpiece = self.board.getfield(dst)
        self.board.setfield(dst, srcpiece)
        if(self.color_of_piece(srcpiece) == COLORS['white']):
            flag = is_field_touched(self, self.board.wKg, COLORS['black'], self.EVAL_MODES['ignore-pins'])
        else:
            flag = is_field_touched(self, self.board.bKg, COLORS['white'], self.EVAL_MODES['ignore-pins'])
        self.board.setfield(dst, dstpiece)
        self.board.setfield(src, srcpiece)
        if(pawnenmy):
            self.board.setfield(dst, pawnenmy)
        return flag

    def do_move(self, src, dst, prompiece):
        piece = self.board.getfield(src)
        cpiece = self.obj_for_piece(piece, src)
        if(cpiece is not None):
            move = cpiece.do_move(dst, prompiece, self.movecnt() + 1)
            self.minutes.append(move)
            return move
        else:
            return None

    def undo_move(self):
        movecnt = self.movecnt()
        if(len(self.minutes) > 0):
            move = self.minutes.pop()
        else:
            return None
        piece = self.board.getfield(move.dst)
        if(move.prompiece and move.prompiece != PIECES['blk']):
            if(self.color_of_piece(piece) == COLORS['white']):
                cpawn = cWhitePawn(self, move.dst)
            else:
                cpawn = cBlackPawn(self, move.dst)
            return cpawn.undo_move(move, movecnt)
        else:
            cpiece = self.obj_for_piece(piece, move.dst)
            if(cpiece):
                return cpiece.undo_move(move, movecnt)
            else:
                return None

    def is_move_available(self):
        color = self.next_color()
        fields = self.board.fields
        for idx in range(63, -1, -1):
            piece = fields & 0xF
            fields = fields >> 4
            #for idx in range(64):
            #piece = self.board.getfield(idx)
            if(piece != PIECES['blk'] and color == self.color_of_piece(piece)):
                cpiece = self.obj_for_piece(piece, idx)
                for step in cpiece.MV_STEPS:
                    count = 0
                    dst = cpiece.pos + step[0]
                    while(self.board.is_inbounds(cpiece.pos, dst, step[0]) and count < cpiece.MAXCNT):
                        count += 1
                        if(self.is_move_valid(cpiece.pos, dst, step[1])[0]):
                            return True
                        dst += step[0]
        return False

    def evaluate_status(self):
        if(self.is_move_available()):
            return self.STATUS['active']
        else:
            if(self.next_color() == COLORS['white']):
                if(is_field_touched(self, self.board.wKg, COLORS['black'], self.EVAL_MODES['ignore-pins'])):
                    return self.STATUS['winner_black']
            else:
                if(is_field_touched(self, self.board.bKg, COLORS['white'], self.EVAL_MODES['ignore-pins'])):
                    return self.STATUS['winner_white']
        return self.STATUS['draw']
 
    def eval_pin_dir(self, src):
        cpieces = [cRook, cBishop]
        white_faces = [PIECES['wRk'], PIECES['wBp']]
        black_faces = [PIECES['bRk'], PIECES['bBp']]
        for idx in range(2):
            piece = self.board.getfield(src)
            color = self.color_of_piece(piece)
            if(color == COLORS['white']):
                kg = self.board.wKg
            else:
                kg = self.board.bKg
            direction = cpieces[idx].dir_for_move(src, kg)
            if(direction != DIRS['undef']):
                step = cpieces[idx].step_for_dir(direction)
                if(step is not None):
                    dst = self.board.search(src, step, cpieces[idx].MAXCNT)
                    if(dst is not None):
                        piece = self.board.getfield(dst)
                        if( (color == COLORS['white'] and piece == PIECES['wKg']) or
                            (color == COLORS['black'] and piece == PIECES['bKg']) ):
                            reverse_dir = REVERSE_DIRS[direction]
                            step2 = cpieces[idx].step_for_dir(reverse_dir)
                            if(step2 is not None):
                                dst2 = self.board.search(src, step2, cpieces[idx].MAXCNT)
                                if(dst2 is not None):
                                    piece2 = self.board.getfield(dst2)
                                    if(color == COLORS['white']):
                                        if(piece2 == PIECES['bQu'] or piece2 == black_faces[idx]):
                                            return direction
                                        else:
                                            return DIRS['undef']
                                    else:
                                        if(piece2 == PIECES['wQu'] or piece2 == white_faces[idx]):
                                            return direction
                                        else:
                                            return DIRS['undef']
        return DIRS['undef']
 
    def obj_for_piece(self, piece, pos):
        if(piece == PIECES['wPw']):
            return cWhitePawn(self, pos)
        if(piece == PIECES['bPw']):
            return cBlackPawn(self, pos)
        elif(piece == PIECES['wKn'] or piece == PIECES['bKn']):
            return cKnight(self, pos)
        elif(piece == PIECES['wBp'] or piece == PIECES['bBp']):
            return cBishop(self, pos)
        elif(piece == PIECES['wRk'] or piece == PIECES['bRk']):
            return cRook(self, pos)
        elif(piece == PIECES['wQu'] or piece == PIECES['bQu']):
            return cQueen(self, pos)
        elif(piece == PIECES['wKg'] or piece == PIECES['bKg']):
            return cKing(self, pos)
        else:
            return None

# class end
*/