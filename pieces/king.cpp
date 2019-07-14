
#include "./king.hpp"

    using namespace std;

    cKing::cKing(cBoard *board, unsigned pos) : cPiece(board, pos){
    }

    unsigned cKing::DIRS_ARY[1] = {0};
    int cKing::STEPS[8] = {8, 9, 1, -7, -8, -9, -1, 7};
    vector<pair<int, unsigned>> cKing::MV_STEPS = 
        {make_pair(8, PIECES["blk"]),  make_pair(9, PIECES["blk"]), 
         make_pair(1, PIECES["blk"]),  make_pair(-7, PIECES["blk"]),
         make_pair(-8, PIECES["blk"]), make_pair(-9, PIECES["blk"]), 
         make_pair(-1, PIECES["blk"]), make_pair(7, PIECES["blk"])};
    int cKing::MAXCNT = 1;

    bool cKing::is_trapped(){
        return false; //king cannot be trapped
    }
    
    bool cKing::is_move_stuck(unsigned dst){
        return false; //king cannot be trapped
    }


    bool cKing::is_move_valid(unsigned dst, unsigned prompiece){
        if(is_short_castling_ok(dst)){
            return true;
        }
        /*if(is_long_castling_ok(dst)){
            return true;
        }*/
        //###
        unsigned opp_color = REVERSED_COLORS[color];
        bool flag = false;
        for(const int step : STEPS){
            if(pos + step == dst && board->is_inbounds(pos, dst, step)){
                flag = true;
                break;
            }
        }
        if(flag == false){
            return false;
        }
        //###
        unsigned captured = board->getfield(dst);
        board->setfield(pos, PIECES["blk"]);
        board->setfield(dst, piece);
        bool isattacked = is_field_touched(board, dst, REVERSED_COLORS[color], EVAL_MODES["ignore-pins"]);
        board->setfield(pos, piece);
        board->setfield(dst, captured);
        //##
        if(isattacked){
            return false;
        }
        unsigned dstpiece = board->getfield(dst);
        if(PIECES_COLOR[dstpiece] == color){
            return false;
        }
        return true;
    }

    bool cKing::is_short_castling_ok(unsigned dst){
        uint256_t shorttest, shortmask;
        if(pos - dst != -2){
            return false;
        }
        if(color == COLORS["white"]){
            shorttest  = 0x0000600400000000000000000000000000000000000000000000000000000000_cppui;
            shortmask  = 0x0000FFFF00000000000000000000000000000000000000000000000000000000_cppui;
        }
        else{
            shorttest  = 0x000000000000000000000000000000000000000000000000000000000000E00C_cppui;
            shortmask  = 0x000000000000000000000000000000000000000000000000000000000000FFFF_cppui;
        }            
        uint256_t fields = board->fields & shortmask;
        if(fields != shorttest){
            return false;
        }
        if(color == COLORS["white"]){
            if(board->wKg_first_move_on != -1 || board->wRkH_first_move_on != -1){
                return false;
            }
        }
        else{
            if(board->bKg_first_move_on != -1 || board->bRkH_first_move_on != -1){
                return false;
            }
        }
        board->setfield(pos, PIECES["blk"]);
        for(unsigned i = 0; i < 3; ++i){
            unsigned dst2 = pos + i;
            bool isattacked = is_field_touched(board, dst2, REVERSED_COLORS[color], EVAL_MODES["ignore-pins"]);
            if(isattacked){
                board->setfield(pos, piece);
                return false;
            }
        }
        board->setfield(pos, piece);
        return true;
    }

    bool cKing::is_long_castling_ok(unsigned dst){
        uint256_t longtest, longmask;
        if(pos - dst != 2){
            return false;
        }
        if(color == COLORS["white"]){
            longtest  = 0x4000600000000000000000000000000000000000000000000000000000000000_cppui;
            longmask  = 0xFFFFF00000000000000000000000000000000000000000000000000000000000_cppui;
        }
        else{
            longtest  = 0x00000000000000000000000000000000000000000000000000000000C000E000_cppui;
            longmask  = 0x00000000000000000000000000000000000000000000000000000000FFFFF000_cppui;
        }
        uint256_t fields = board->fields & longmask;
        if(fields != longtest){
            return false;
        }
        if(color == COLORS["white"]){
            if(board->wKg_first_move_on != -1 || board->wRkA_first_move_on != -1){
                return false;
            }
        }
        else{
            if(board->bKg_first_move_on != -1 || board->bRkA_first_move_on != -1){
                return false;
            }
        }
        board->setfield(pos, PIECES["blk"]);
        for(unsigned i = 0; i < 3; ++i){
            unsigned dst2 = pos - i;
            bool isattacked = is_field_touched(board, dst2, REVERSED_COLORS[color], EVAL_MODES["ignore-pins"]);
            if(isattacked){
                board->setfield(pos, piece);
                return false;
            }
        }
        board->setfield(pos, piece);
        return true;
    }
        
    cMove *cKing::do_move(unsigned dst, unsigned prompiece, int movecnt, int *score){
        cMove *move = cPiece::do_move(dst, prompiece, movecnt, score);
        unsigned dstpiece = board->getfield(dst);
        if(pos - dst == -2){
            unsigned rook = board->getfield(pos + 3);
            board->setfield(pos + 3, PIECES["blk"]);
            board->setfield(dst - 1, rook);
        }
        if(pos - dst == 2){
            unsigned rook = board->getfield(pos - 4);
            board->setfield(pos - 4, PIECES["blk"]);
            board->setfield(dst + 1, rook);
        }
        if(piece == PIECES["wKg"]){
            if(board->wKg_first_move_on == -1){
                board->wKg_first_move_on = movecnt;
            }
            board->wKg = dst;
        }
        else{
            if(board->bKg_first_move_on == -1){
                board->bKg_first_move_on = movecnt;
            }
            board->bKg = dst;
        }
        return move;
    }

    bool cKing::undo_move(cMove *move, int movecnt, int *score){
        if(cPiece::undo_move(move, movecnt, score) == false){
            return false;
        }
        if(piece == PIECES["wKg"]){
            if(board->wKg_first_move_on != -1 && board->wKg_first_move_on == movecnt){
                board->wKg_first_move_on = -1;
            }
            board->wKg = move->src;
        }
        else{
            if(board->bKg_first_move_on != -1 && board->bKg_first_move_on == movecnt){
                board->bKg_first_move_on = -1;
            }
            board->bKg = move->src;
        }
        return true;
    }


    int cKing::score_touches(){
        return 0;
    }

/* 
    def is_safe(self){
        count = 0
        for step in self.STEPS:
            dst = self.pos + step
            if(board->is_inbounds(self.pos, dst, step)){
                friends, enemies = list_all_field_touches(self.match, dst, self.color)
                if(len(friends) < len(enemies)){
                    return false;
                if(len(enemies) > 0){
                    count += 1
        if(count > 2){
            return false;
        friends.clear()
        enemies.clear()
        friends, enemies = list_all_field_touches(self.match, self.pos, self.color)
        if(len(enemies) == 0){
            return True
        if(len(enemies) >= 2){
            return false;
        else{
            enemy = enemies[0]
            friends_beyond, enemies_beyond = list_all_field_touches(self.match, enemy.field, self.color)
            if(len(friends_beyond) >= len(enemies_beyond)){
                return True
            cenemy = self.match.obj_for_piece(enemy.piece, enemy.field)
            direction = cenemy.dir_for_move(self.pos, enemy.field)
            if(direction == DIRS["undef"]){
                return True
            else{
                step = cenemy.step_for_dir(direction)
            dst = self.pos + step
            while(board->is_inbounds(self.pos, dst, step)){
                blocking_friends, blocking_enemies = list_all_field_touches(self.match, dst, self.color)
                if(len(blocking_friends) > 0){
                    return True
                dst += step
        return false;
*/