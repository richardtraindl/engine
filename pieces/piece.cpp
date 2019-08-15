
    #include <ctime>
    #include "./piece.hpp"
    #include "./piece_ext1.hpp"
    #include "./piece_ext2.hpp"
    #include "./searchforpiece.hpp"
    #include "../compute/analyze_move.hpp"
    #include "../values.hpp"

    cPiece::cPiece(cBoard &newboard, int newpos){
        board = &newboard;
        pos = newpos;
        piece = board->getfield(pos);
        color = PIECES_COLORS[piece];
    }

    cPiece::~cPiece(){ }

    array<int, 8> cPiece::get_dirs_ary() { 
        if(piece == mWRK || piece == mBRK){
            return {DIRS["nth"], DIRS["sth"], DIRS["est"], DIRS["wst"],
                    DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"]};
        }
        if(piece == mWBP || piece == mBBP){
            return {DIRS["nth-est"], DIRS["sth-wst"], DIRS["nth-wst"], DIRS["sth-est"],
                    DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"]};
        }
        if(piece == mWQU || piece == mBQU){
            return {DIRS["nth"], DIRS["sth"], DIRS["est"], DIRS["wst"],
                    DIRS["nth-est"], DIRS["sth-wst"], DIRS["nth-wst"], DIRS["sth-est"]};
        }
        else{ // mWPW, mBPW, mWKN, mBKN, mWKG, mBKG
            return {DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"], 
                    DIRS["undef"], DIRS["undef"], DIRS["undef"], DIRS["undef"]};
        }
    }

    array<int, 8> cPiece::get_steps(){
        if(piece == mWPW){
            return {9, 7, 0, 0, 0, 0, 0, 0};
        }
        if(piece == mBPW){
            return {-9, -7, 0, 0, 0, 0, 0, 0};
        }
        if(piece == mWRK || piece == mBRK){
            return {8, -8, 1, -1, 0, 0, 0, 0};
        }
        if(piece == mWBP || piece == mBBP){
            return {9, -9, 7, -7, 0, 0, 0, 0};
        }
        if(piece == mWKN || piece == mBKN){
            return {8, -8, 1, -1, 0, 0, 0, 0};
        }
        if(piece == mWQU || piece == mBQU){
            return {8, -8, 1, -1, 9, -9, 7, -7};
        }
        if(piece == mWKG || piece == mBKG){
            return {8, 9, 1, -7, -8, -9, -1, 7};
        }
        else{
            return {0, 0, 0, 0, 0, 0, 0, 0};
        }
    }


    array<int, 10> cPiece::get_mv_steps(){
        if(piece == mWPW){
            return {8, 16, 9, 7, 0, 0, 0, 0, 0, 0};
        }
        if(piece == mBPW){
            return {-8, -16, -7, -9, 0, 0, 0, 0, 0, 0};
        }
        if(piece == mWRK || piece == mBRK){
            return {8, -8, 1, -1, 0, 0, 0, 0, 0, 0};
        }
        if(piece == mWBP || piece == mBBP){
            return {9, -9, 7, -7, 0, 0, 0, 0, 0, 0};
        }
        if(piece == mWKN || piece == mBKN){
            return {17, 10, -6, -15, -17, -10, 6, 15, 0, 0};
        }
        if(piece == mWQU || piece == mBQU){
            return {8, -8, 1, -1, 9, -9, 7, -7, 0, 0};
        }
        if(piece == mWKG || piece == mBKG){
            return {8, 9, 1, -7, -8, -9, -1, 7, 2, -2};
        }
        else{
            return {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        }
    }

    int cPiece::get_maxcnt(){
        if(piece == mWPW || piece == mBPW || 
           piece == mWKN || piece == mBKN ||
           piece == mWKG || piece == mBKG){
               return 1;
        }
        else{
            return 7;
        }
    }

    array<int, 4> cPiece::get_prom_pieces(){ 
        if(piece == mWPW && pos >= 48){
            return {mWQU, mWRK, mWBP, mWKN};
        }
        if(piece == mBPW  && pos <= 15){
            return {mBQU, mBRK, mBBP, mBKN};
        }
        return {mBLK, mBLK, mBLK, mBLK}; 
    }

    int cPiece::dir_for_move(int piece, int src, int dst){
        if(src == dst){ return DIRS["undef"]; }
        if(cBoard::is_inbounds_core(src, dst) == false){ return DIRS["undef"]; }

        if(piece == mWPW){
            if(src + 8 == dst || src + 16 == dst){ return DIRS["nth"]; }
            if(src + 9 == dst && src % 8 < dst % 8){ return DIRS["nth-est"]; }
            if(src + 7 == dst && src % 8 > dst % 8){ return DIRS["nth-wst"]; }
            return DIRS["undef"];
        }
        if(piece == mBPW){
            if(src - 8 == dst || src - 16 == dst){ return DIRS["sth"]; }
            if(src - 9 == dst && src % 8 > dst % 8){ return DIRS["sth-wst"]; }
            if(src - 7 == dst && src % 8 < dst % 8){ return DIRS["sth-est"]; }
            return DIRS["undef"];
        }
        if(piece == mWRK || piece == mBRK || piece == mWQU || piece == mBQU){
            if(cBoard::is_nth(src, dst)){ return DIRS["nth"]; }
            if(cBoard::is_sth(src, dst)){ return DIRS["sth"]; }
            if(cBoard::is_est(src, dst)){ return DIRS["est"]; }
            if(cBoard::is_wst(src, dst)){ return DIRS["wst"]; }
            if(piece == mWRK || piece == mBRK){ return DIRS["undef"]; }
        }
        if(piece == mWBP || piece == mBBP || piece == mWQU || piece == mBQU){
            if(cBoard::is_nth_est(src, dst)){ return DIRS["nth-est"]; }
            if(cBoard::is_sth_wst(src, dst)){ return DIRS["sth-wst"]; }
            if(cBoard::is_sth_est(src, dst) == true){ return DIRS["sth-est"]; }
            if(cBoard::is_nth_wst(src, dst) == true){ return DIRS["nth-wst"]; }
            return DIRS["undef"];
        }
        else{ // mWKN mBKN, mWKG, mBKG ???
            return DIRS["undef"];
        }
    }

    int cPiece::step_for_dir(int piece, int dir){
        if(piece == mWPW){
            if(dir == DIRS["nth"]){ return 8; }
            if(dir == DIRS["nth-est"]){ return 9; }
            if(dir == DIRS["nth-wst"]){ return 7; }
            return 0;
        }
        if(piece == mBPW){
            if(dir == DIRS["sth"]){ return -8; }
            if(dir == DIRS["sth-wst"]){ return -9; }
            if(dir == DIRS["sth-est"]){ return -7; }
            return 0;
        }
        if(piece == mWRK || piece == mBRK || piece == mWQU || piece == mBQU){
            if(dir == DIRS["nth"]){ return 8; }
            if(dir == DIRS["sth"]){ return -8; }
            if(dir == DIRS["est"]){ return 1; }
            if(dir == DIRS["wst"]){ return -1; }
            if(piece == mWRK || piece == mBRK){ return 0; }
        }
        if(piece == mWBP || piece == mBBP || piece == mWQU || piece == mBQU){
            if(dir == DIRS["nth-est"]){ return 9; }
            if(dir == DIRS["sth-wst"]){ return -9; }
            if(dir == DIRS["nth-wst"]){ return 7; }
            if(dir == DIRS["sth-est"]){ return -7; }
            return 0;
        }
        else{ // mWKN, mBKN, mWKG, mBKG
            return 0;
        }
    }

    bool cPiece::is_queen_or_rook(int piece){
        return PIECES_BARES[piece] == PIECES_BARES[mWQU] ||
               PIECES_BARES[piece] == PIECES_BARES[mWRK];
    }

    bool cPiece::is_queen_or_bishop(int piece){
        return PIECES_BARES[piece] == PIECES_BARES[mWQU] ||
               PIECES_BARES[piece] == PIECES_BARES[mWBP];
    }

    bool cPiece::is_trapped(){
        return is_trapped_from_ext(this);
    }

    bool cPiece::is_move_stuck(int dst){
        return is_move_stuck_from_ext(this, dst);
    }

    bool cPiece::is_move_valid(int dst, int prompiece, list<cMove> &minutes){
        return is_move_valid_from_ext(this, dst, prompiece, minutes);
    }

    cMove *cPiece::do_move(int dst, int prompiece, int movecnt, int &score){
        return do_move_from_ext(this, dst, prompiece, movecnt, score);
    }

    void cPiece::undo_move(cMove &move, int movecnt, int &score){
        undo_move_from_ext(this, move, movecnt, score);
    }

    void cPiece::find_supported_and_attacked(list<cTouch> &supported, list<cTouch> &attacked){

        for(auto &step : get_steps()){
            if(step == mBLK){ return; }
            int dst2 = board->search(pos, step, get_maxcnt());
            if(dst2 != cBoard::SNOK){
                if(is_move_stuck(dst2)){
                    continue;
                }
                int piece2 = board->getfield(dst2);
                if(PIECES_COLORS[piece2] == color){
                    if(piece2 == mWKG || piece2 == mBKG){
                        continue;
                    }
                    cTouch *ctouch = new cTouch(piece2, dst2);
                    supported.push_back(*ctouch);
                    board->setfield(dst2, mBLK);
                    add_field_touches_beyond(*board, color, *ctouch);
                    board->setfield(dst2, piece2);
                }
                if(PIECES_COLORS[piece2] == REVERSED_COLORS[color]){
                    cTouch *ctouch = new cTouch(piece2, dst2);
                    attacked.push_back(*ctouch);
                    board->setfield(dst2, mBLK);
                    add_field_touches_beyond(*board, REVERSED_COLORS[color], *ctouch);
                    board->setfield(dst2, piece2);
                }
            }
        }
    }


    int cPiece::score_touches(){
        if(piece == mWKG || piece == mBKG){
            return 0; // king touches score 0
        }
        int score = 0;
        list<cTouch>friends, enmies;
        collect_touches_for_both_colors(*board, pos, color, friends, enmies);
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

    void cPiece::generate_moves(cMatch &match, list<cMove> &moves){
        cMove *move;
        for(auto &step : get_mv_steps()){
            if(step == 0){ break; }
            int count = 0;
            int dst = pos + step;
            while(board->is_inbounds(pos, dst, step) && count < get_maxcnt()){
                count += 1;
                for(auto &prompiece : get_prom_pieces()){
                    if(board->is_move_valid(pos, dst, prompiece, match.minutes)){
                        move = new cMove(board->fields, pos, dst, prompiece);
                        moves.push_back(*move);
                    }
                    if(prompiece == mBLK){
                        break;
                    }
                }
                dst += step;
            }
        }
    }

    void cPiece::generate_priomoves(cMatch &match, cMove *candidate, cMove *dbggmove, list<cPrioMove> &priomoves){
        cPrioMove *priomove;
        list<cExclude> excludes;
        for(auto &step : get_mv_steps()){
            if(step == 0){ break; }
            int count = 0;
            int dst = pos + step;
            while(board->is_inbounds(pos, dst, step) && count < get_maxcnt()){
                int dstpiece = board->getfield(dst);
                if(PIECES_COLORS[dstpiece] == color){ break; }
                count += 1;
                for(auto &prompiece : get_prom_pieces()){
                    if(board->is_move_valid(pos, dst, prompiece, match.minutes)){
                        priomove = new cPrioMove(board->fields, pos, dst, prompiece, cPrioMove::PRIOS["prio3"]);
                        add_tactics(*priomove, match, candidate, dbggmove, excludes);
                        priomoves.push_back(*priomove);
                    }
                    if(prompiece == mBLK){ break; }
                }
                dst += step;
            }
        }
    }

    bool cPiece::is_running_pawn(){
        if(piece == mWPW){
            int step = 8;
            int opp_pawn = mBPW;
            for(int idx = -1; idx < 2; ++idx){
                int src = pos + idx;
                int dst = board->search(src, step, 5);
                while(dst != cBoard::SNOK){
                    int newpiece = board->getfield(dst);
                    if(newpiece == opp_pawn){
                        return false;
                    }
                    dst = board->search(dst, step, 5);
                }
            }
            return true;
        }
        if(piece == mBPW){
            int step = -8;
            int opp_pawn = mWPW;
            for(int idx = -1; idx < 2; ++idx){
                int src = pos + idx;
                int dst = board->search(src, step, 5);
                while(dst != cBoard::SNOK){
                    int newpiece = board->getfield(dst);
                    if(newpiece == opp_pawn){
                        return false;
                    }
                    dst = board->search(dst, step, 5);
                }
            }
            return true;
        }
        return false;
    }


    bool cPiece::is_king_safe(){
        int count = 0;
        for(auto &step : get_mv_steps()){
            if(step == 0){ break; }
            int dst = pos + step;
            if(board->is_inbounds(pos, dst, step)){
                list<cTouch> friends, enemies;
                collect_touches_for_both_colors(*board, dst, color, friends, enemies);
                if(friends.size() < enemies.size()){
                    return false;
                }
                if(enemies.size() > 0){
                    count += 1;
                }
            }
        }

        if(count > 2){
            return false;
        }
        list<cTouch> friends, enemies;
        collect_touches_for_both_colors(*board, pos, color, friends, enemies);
        if(enemies.size() >= 2){
            return false;
        }
        else{
            return true;
        }
    }
