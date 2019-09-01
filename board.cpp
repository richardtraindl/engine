
    #include "./board.hpp"
    #include "./pieces/searchforpiece.hpp"
    #include "./pieces/piece.hpp"
    #include "./pieces/piece_ext2.hpp"
    #include "./values.hpp"
    #include "./helper.hpp"


    cBoard::cBoard(){ 
        fields = BASE;
        wKg = COLS["E"] + RANKS["1"] * 8;
        bKg = COLS["E"] + RANKS["8"] * 8;
        wKg_first_move_on = -1;
        bKg_first_move_on = -1;
        wRkA_first_move_on = -1;
        wRkH_first_move_on = -1;
        bRkA_first_move_on = -1;
        bRkH_first_move_on = -1;
    }

    cBoard::cBoard(const cBoard &board){
    } // copy constructor

    const int cBoard::SNOK = -1;

    map<string, int> cBoard::RANKS = {
        {"1", 0},
        {"2", 1},
        {"3", 2},
        {"4", 3},
        {"5", 4},
        {"6", 5},
        {"7", 6},
        {"8", 7}
    };

    map<string, int> cBoard::COLS = {
        {"A", 0},
        {"B", 1},
        {"C", 2},
        {"D", 3},
        {"E", 4},
        {"F", 5},
        {"G", 6},
        {"H", 7}
    };


    int cBoard::getfield(int idx){
        if(idx < 0 || idx > 63){
            cout << "getfield: " << idx;
        }
        return (int)((fields >> ((63 - idx) * 4)) & 0xF);
    }

    void cBoard::setfield(int idx, int value){
        if(idx < 0 || idx > 63){
            cout << "setfield: " << idx;
        }
        uint256_t _value = value;
        uint256_t tmpfields = SINGLE >> (idx * 4);
        tmpfields = tmpfields ^ FULL;
        tmpfields = tmpfields & fields;
        _value = (_value << ((63 - idx) * 4));
        fields = tmpfields | _value;
    }

    bool cBoard::verify(){
        int wKg_cnt = 0;
        int bKg_cnt = 0;
        int wPw_cnt = 0;
        int bPw_cnt = 0;
        int wOfficer_cnt = 0;
        int bOfficer_cnt = 0;
        for(int idx = 0; idx < 64; ++idx){
            int piece = getfield(idx);
            if(piece == mWKG){
                wKg_cnt += 1; 
                continue;
            }
            if(piece == mBKG){
                bKg_cnt += 1;
                continue;
            }
            if(piece == mWPW){
                wPw_cnt += 1;
                continue;
            }
            if(piece == mBPW){
                bPw_cnt += 1;
                continue; 
            }
            if(piece == mWRK || piece == mWBP || piece == mWKN || piece == mWQU){
                wOfficer_cnt += 1;
                continue;
            }
            if(piece == mBRK || piece == mBBP || piece == mBKN || piece == mBQU){
                bOfficer_cnt += 1;
                continue;
            }
        }
        if(wKg_cnt != 1 || bKg_cnt != 1){
            return false;
        }
        if(wPw_cnt > 8 || bPw_cnt > 8){
            return false;
        }
        if(wPw_cnt + wOfficer_cnt > 15){
            return false;
        }
        if(bPw_cnt + bOfficer_cnt > 15){
            return false;
        }
        if(wKg == -1 || bKg == -1){
            return false;
        }
        if(abs(wKg / 8 - bKg / 8) < 2 && abs(wKg % 8 - bKg % 8) < 2){
            return false;
        }
        return true;
    }

    void cBoard::eval_count_of_officers(int &wofficers, int &bofficers){
        wofficers = 0;
        bofficers = 0;
        for(int idx = 0; idx < 64; ++idx){
            int piece = getfield(idx);
            if(piece == mWRK || piece == mWBP || piece == mWKN || piece == mWQU){
                wofficers += 1;
                continue;
            }
            if(piece == mBRK || piece == mBBP || piece == mBKN || piece == mBQU){
                bofficers += 1;
                continue;
            }
        }
    }

    bool cBoard::is_inbounds_core(int src, int dst){
        if(src < 0 || src > 63 || dst < 0 || dst > 63){
            return false;
        }
        else{
            return true;
        }
    }

    bool cBoard::is_inbounds(int src, int dst, int step){
        if(src < 0 || src > 63 || dst < 0 || dst > 63){
            return false;
        }
        if(step == 0){
            return true;
        }
        int dir = DIRS_FOR_STEP[step];
        if(dir == 0){
            return false;
        }
        if(dir == DIRS["nth"] || dir == DIRS["sth"]){
            return true;
        }
        if(dir == DIRS["est"] || dir == DIRS["nth-est"] || dir == DIRS["sth-est"]){
            return (src % 8) < (dst % 8);
        }
        if(dir == DIRS["wst"] || dir == DIRS["sth-wst"] || dir == DIRS["nth-wst"]){
            return (src % 8) > (dst % 8);
        }
        if(dir == DIRS["2nth-est"] || dir == DIRS["nth-2est"] || 
           dir == DIRS["sth-2est"] || dir == DIRS["2sth-est"]){
            return (src % 8) < (dst % 8);
        }
        if(dir == DIRS["2sth-wst"] || dir == DIRS["sth-2wst"] || 
           dir == DIRS["nth-2wst"] || dir == DIRS["2nth-wst"]){
            return (src % 8) > (dst % 8);
        }
        return false;
    }

    int cBoard::search(int src, int step, int maxcnt){
        int cnt = 0;
        int dst = src + step;
        while(is_inbounds(src, dst, step) && cnt < maxcnt){
            int piece = getfield(dst);
            if(piece != mBLK){
                return dst;
            }
            cnt += 1;
            dst += step;
        }
        return SNOK;
    }

    bool cBoard::search_for_two_pieces_within_dir(int &first, int &second, int src, int step, int maxcnt){
        int cnt = 0;
        first = SNOK;
        int dst = src + step;
        while(is_inbounds(src, dst, step) && cnt < maxcnt){
            int piece = getfield(dst);
            if(piece != mBLK){
                if(first == SNOK){
                    first = dst;
                    continue;
                }
                else{
                    second = dst;
                    return true;
                }
            }
            cnt += 1;
            dst += step;
        }
        return false;
    }

    bool cBoard::search_bi_dirs(int &first, int &second, int src, int step, int maxcnt){
        int cnt = 0;
        first = SNOK;
        int bisteps[2] = {step, (step * -1)};
        for(const int bistep : bisteps){
            int dst = src + bistep;
            while(is_inbounds(src, dst, bistep) && cnt < maxcnt){
                int piece = getfield(dst);
                if(piece != mBLK){
                    if(first == SNOK){
                        first = dst;
                        break;
                    }
                    else{
                        second = dst;
                        return true;
                    }
                }
                cnt += 1;
                dst += bistep;
            }
            if(first == SNOK){
                break;
            }
        }
        return false;
    }

    bool cBoard::is_nth(int src, int dst){
        if(src < dst && abs((int)src - (int)dst) % 8 == 0){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_sth(int src, int dst){
        if(src > dst && abs((int)src - (int)dst) % 8 == 0){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_est(int src, int dst){
        if(src % 8 < dst % 8 && src / 8 == dst / 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_wst(int src, int dst){
        if(src % 8 > dst % 8 && src / 8 == dst / 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_nth_est(int src, int dst){
        if(abs((int)src - (int)dst) % 9 == 0 && src < dst && src % 8 < dst % 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_sth_wst(int src, int dst){
        if(abs((int)src - (int)dst) % 9 == 0 && src > dst && src % 8 > dst % 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_nth_wst(int src, int dst){
        if(abs((int)src - (int)dst) % 7 == 0 && src < dst && src % 8 > dst % 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_sth_est(int src, int dst){
        if(abs((int)src - (int)dst) % 7 == 0 && src > dst && src % 8 < dst % 8){
            return true;
        }
        else{
            return false;
        }
    }

    int cBoard::eval_pin_dir(int src){
        int piece = getfield(src);
        int color = PIECES_COLORS[piece];
        int kg_pos;
        int first, second, fstpiece, sndpiece;
        int enmyfaces[2];
        int dir_ary[2];
        int step;
        if(color == COLORS["white"]){
            kg_pos = wKg;
        }
        else{
            kg_pos = bKg;
        }        
        for(int j = 0; j < 2; ++j){
            if(j == 0){
                dir_ary[0] = DIRS["nth"];
                dir_ary[1] = DIRS["est"];
                if(color == COLORS["white"]){
                    enmyfaces[0] = mBRK;
                    enmyfaces[1] = mBQU;
                }
                else{
                    enmyfaces[0] = mWRK;
                    enmyfaces[1] = mWQU;
                }
            }
            else{
                dir_ary[0] = DIRS["nth-est"];
                dir_ary[1] = DIRS["nth-wst"];
                if(color == COLORS["white"]){
                    enmyfaces[0] = mBBP;
                    enmyfaces[1] = mBQU;
                }
                else{
                    enmyfaces[0] = mWBP;
                    enmyfaces[1] = mWQU;
                }
            }
            for(int i = 0; i < 2; ++i){
                if(j == 0){
                    step = cPiece::step_for_dir(mWRK, dir_ary[i]);
                }
                else{
                    step = cPiece::step_for_dir(mWBP, dir_ary[i]);
                }
                if(search_bi_dirs(first, second, src, step, 6)){
                    fstpiece = getfield(first);
                    sndpiece = getfield(second);
                    if(PIECES_COLORS[fstpiece] != PIECES_COLORS[sndpiece]){
                        if(PIECES_COLORS[fstpiece] == color && kg_pos == first &&
                           (enmyfaces[0] == sndpiece || enmyfaces[1] == sndpiece)){
                            return dir_ary[i];
                        }
                        if(PIECES_COLORS[sndpiece] == color && kg_pos == second && 
                           (enmyfaces[0] == fstpiece || enmyfaces[1] == fstpiece)){
                            return REVERSE_DIRS[dir_ary[i]];
                        }
                    }
                }
            }
        }
        return DIRS["undef"];
    }

    int cBoard::eval_soft_pin_dir(int src){
        int piece = getfield(src);
        int color = PIECES_COLORS[piece];
        int first, second, fstpiece, sndpiece;
        int enmyfaces[2];
        int dir_ary[2];
        int step;
        for(int j = 0; j < 2; ++j){
            if(j == 0){
                dir_ary[0] = DIRS["nth"];
                dir_ary[1] = DIRS["est"];
                if(color == COLORS["white"]){
                    enmyfaces[0] = mBRK;
                    enmyfaces[1] = mBQU;
                }
                else{
                    enmyfaces[0] = mWRK;
                    enmyfaces[1] = mWQU;
                }
            }
            else{
                dir_ary[0] = DIRS["nth-est"];
                dir_ary[1] = DIRS["nth-wst"];
                if(color == COLORS["white"]){
                    enmyfaces[0] = mBBP;
                    enmyfaces[1] = mBQU;
                }
                else{
                    enmyfaces[0] = mWBP;
                    enmyfaces[1] = mWQU;
                }
            }
            for(int i = 0; i < 2; ++i){
                if(j == 0){
                    step = cPiece::step_for_dir(mWRK, dir_ary[i]);
                }
                else{
                    step = cPiece::step_for_dir(mWBP, dir_ary[i]);
                }
                if(search_bi_dirs(first, second, src, step, 6)){
                    fstpiece = getfield(first);
                    sndpiece = getfield(second);
                    if(PIECES_COLORS[fstpiece] != PIECES_COLORS[sndpiece]){
                        if(PIECES_COLORS[fstpiece] == color && 
                           PIECES_RANKS[fstpiece] > PIECES_RANKS[sndpiece] &&
                           PIECES_RANKS[fstpiece] > PIECES_RANKS[piece] &&
                           (enmyfaces[0] == sndpiece || enmyfaces[1] == sndpiece)){
                            return dir_ary[i];
                        }
                        if(PIECES_COLORS[sndpiece] == color && 
                           PIECES_RANKS[sndpiece] > PIECES_RANKS[fstpiece] &&
                           PIECES_RANKS[sndpiece] > PIECES_RANKS[piece] &&
                           (enmyfaces[0] == fstpiece || enmyfaces[1] == fstpiece)){
                            return REVERSE_DIRS[dir_ary[i]];
                        }
                    }
                }
            }
        }
        return DIRS["undef"];
    }

    bool cBoard::is_king_after_move_attacked(int src, int dst, list<cMove> &minutes){
        int srcpiece = getfield(src);
        int dstpiece;
        int pawnenmy = mBLK;
        bool flag;
        if(srcpiece == mWPW){
            cPiece cpiece(*this, src);
            if(is_ep_move_ok_for_whitepawn(&cpiece, dst, minutes)){
                pawnenmy = getfield(dst);
                setfield(dst, mBLK);
            }
        }
        if(srcpiece == mBPW){
        cPiece cpiece(*this, src);
            if(is_ep_move_ok_for_blackpawn(&cpiece, dst, minutes)){
                pawnenmy = getfield(dst);
                setfield(dst, mBLK);
            }
        }

        setfield(src, mBLK);
        dstpiece = getfield(dst);
        setfield(dst, srcpiece);
        if(PIECES_COLORS[srcpiece] == COLORS["white"]){
            flag = is_field_touched(*this, wKg, COLORS["black"], EVAL_MODES["ignore-pins"]);
        }
        else{
            flag = is_field_touched(*this, bKg, COLORS["white"], EVAL_MODES["ignore-pins"]);
        }
        setfield(dst, dstpiece);
        setfield(src, srcpiece);
        if(pawnenmy != mBLK){
            setfield(dst, pawnenmy);
        }
        return flag;
    }

    bool cBoard::is_move_valid(int src, int dst, int prompiece, list<cMove> &minutes){
        int piece = getfield(src);
        int direction = cPiece::dir_for_move(piece, src, dst);
        int step = cPiece::step_for_dir(piece, direction);
        if(is_inbounds(src, dst, step) == false){
            return false; // RETURN_CODES["out-of-bounds"]
        }
        if(minutes.size() % 2 == 0 && PIECES_COLORS[piece] != COLORS["white"]){
            return false; // RETURN_CODES["wrong-color"]
        }
        if(minutes.size() % 2 == 1 && PIECES_COLORS[piece] != COLORS["black"]){
            return false; // RETURN_CODES["wrong-color"]
        }
        if(piece != mWKG && piece != mBKG){
            if(is_king_after_move_attacked(src, dst, minutes)){
                return false; // RETURN_CODES["king-attacked-error"]
            }
        }
        cPiece cpiece(*this, src);
        if(cpiece.is_move_valid(dst, prompiece, minutes)){
            return true; // RETURN_CODES["ok"]
        }
        else{
            return false; // RETURN_CODES["piece-error"]
        }
    }

    bool cBoard::is_move_available(list<cMove> &minutes){
        int color;
        if(minutes.size() % 2 == 0){
            color = COLORS["white"];
        }
        else{
            color = COLORS["black"];
        }
        for(int idx = 0; idx < 64; ++idx){
            int piece = getfield(idx);
            if(color == PIECES_COLORS[piece]){
                cPiece cpiece(*this, idx);
                for(auto &step : cpiece.get_mv_steps()){
                    if(step == 0){ break; }
                    int count = 0;
                    int dst = cpiece.pos + step;
                    while(is_inbounds(cpiece.pos, dst, step) && count < cpiece.get_maxcnt()){
                        int dstpiece = getfield(dst);
                        if(PIECES_COLORS[dstpiece] == cpiece.color){ break; }
                        count += 1;
                        for(auto &prompiece : cpiece.get_prom_pieces()){
                            if(is_move_valid(cpiece.pos, dst, prompiece, minutes)){
                                return true;
                            }
                            if(prompiece == mBLK){ break; }
                        }
                        dst += step;
                    }
                }
            }
        }
        return false;
    }

    void cBoard::prnt(){
        string textcolor, backcolor, strpiece;
        for(int y = 7; y >=0; --y){
            for(int x = 0; x < 8; ++x){
                int piece = getfield(y * 8 + x);
                if(piece == mBLK){
                    strpiece = "   ";
                }
                else{
                    strpiece = reverse_lookup(PIECES, piece);
                }
                if(PIECES_COLORS[piece] == COLORS["white"]){
                    textcolor = WHITE_TEXT + BOLD_ON;
                }
                else{
                    textcolor = BLACK_TEXT + BOLD_ON;
                }
                if((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1)){
                    backcolor = BLUE_BACK;
                }
                else{
                    backcolor = GREEN_BACK;
                }
                // "\e#3" \e#4"
                cout << backcolor + textcolor + strpiece + RESET_ALL; 
            }
            cout << endl;
        }
    }
   
    bool cBoard::debug_compare(cBoard &board2){
        return fields == board2.fields &&
               wKg == board2.wKg &&
               bKg == board2.bKg &&
               wKg_first_move_on == board2.wKg_first_move_on &&
               bKg_first_move_on == board2.bKg_first_move_on &&
               wRkA_first_move_on == board2.wRkA_first_move_on &&
               wRkH_first_move_on == board2.wRkH_first_move_on &&
               bRkA_first_move_on == board2.bRkA_first_move_on &&
               bRkH_first_move_on == board2.bRkH_first_move_on;
    }


/*
    def set_to_base(self){
        self.fields = self.BASE
        self.wKg = self.COLS["E"] + self.RANKS["1"] * 8
        self.bKg = self.COLS["E"] + self.RANKS["8"] * 8
        self.wKg_first_move_on = None
        self.bKg_first_move_on = None
        self.wRkA_first_move_on = None
        self.wRkH_first_move_on = None
        self.bRkA_first_move_on = None
        self.bRkH_first_move_on = None

    def clear(self){
        self.fields = 0x0
        self.wKg = None
        self.bKg = None
        self.wKg_first_move_on = None
        self.bKg_first_move_on = None
        self.wRkA_first_move_on = None
        self.wRkH_first_move_on = None
        self.bRkA_first_move_on = None
        self.bRkH_first_move_on = None

    

    @classmethod
    def erase_whites(cls, fields){
        mask = fields & cls.BITS1000
        mask = mask | mask >> 1 | mask >> 2 | mask >> 3
        return fields & mask

    @classmethod
    def erase_blacks(cls, fields){
        mask = fields & cls.BITS1000
        mask = (mask | mask >> 1 | mask >> 2 | mask >> 3)
        return fields & (mask ^ cls.FULL)

    @classmethod
    def mask_pieces(cls, fields, piece){
        negative = fields ^ PIECES_MASK[piece]
        negative = negative | ((negative & cls.BITS1110) >> 1)
        negative = negative | ((negative & cls.BITS1100) >> 2)
        negative = negative | ((negative & cls.BITS0111) << 1)
        negative = negative | ((negative & cls.BITS0011) << 2)
        return fields & (negative ^ cls.FULL)

# class end

*/
