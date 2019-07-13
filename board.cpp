
#include "./board.hpp"
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

    map<string, unsigned> cBoard::RANKS = {
        {"1", 0},
        {"2", 1},
        {"3", 2},
        {"4", 3},
        {"5", 4},
        {"6", 5},
        {"7", 6},
        {"8", 7}
    };

    map<string, unsigned> cBoard::COLS = {
        {"A", 0},
        {"B", 1},
        {"C", 2},
        {"D", 3},
        {"E", 4},
        {"F", 5},
        {"G", 6},
        {"H", 7}
    };


    unsigned cBoard::getfield(unsigned idx){
        return (unsigned)(fields >> ((63 - idx) * 4)) & 0xF;
    }
        // return (fields >> ((63 - idx) * 4)); // & 0xF;

    
    void cBoard::setfield(unsigned idx, unsigned value){
        uint256_t _value = value;
        uint256_t tmpfields = SINGLE >> (idx * 4);
        tmpfields = tmpfields ^ FULL;
        tmpfields = tmpfields & fields;
        _value = (_value << ((63 - idx) * 4));
        fields = tmpfields | _value;
    }

    void cBoard::copyfields(uint256_t *_fields){
        *_fields = fields;
    }

    bool cBoard::comparefields(uint256_t _fields){
        return _fields == fields;
    }

    bool cBoard::verify(){
        int wKg_cnt = 0;
        int bKg_cnt = 0;
        int wPw_cnt = 0;
        int bPw_cnt = 0;
        int wOfficer_cnt = 0;
        int bOfficer_cnt = 0;
        for(int idx = 0; idx < 64; ++idx){
            unsigned piece = getfield(idx);
            if(piece == PIECES["wKg"]){
                wKg_cnt += 1; 
                continue;
            }
            if(piece == PIECES["bKg"]){
                bKg_cnt += 1;
                continue;
            }
            if(piece == PIECES["wPw"]){
                wPw_cnt += 1;
                continue;
            }
            if(piece == PIECES["bPw"]){
                bPw_cnt += 1;
                continue; 
            }
            if(piece == PIECES["wRk"] || piece == PIECES["wBp"] || 
               piece == PIECES["wKn"] || piece == PIECES["wQu"]){
                wOfficer_cnt += 1;
                continue;
            }
            if(piece == PIECES["bRk"] || piece == PIECES["bBp"] || 
               piece == PIECES["bKn"] || piece == PIECES["bQu"]){
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

    bool cBoard::is_inbounds_core(unsigned src, unsigned dst){
        if(src < 0 || src > 63 || dst < 0 || dst > 63){
            return false;
        }
        else{
            return true;
        }
    }

    bool cBoard::is_inbounds(unsigned src, unsigned dst, int step){
        if(src < 0 || src > 63 || dst < 0 || dst > 63){
            return false;
        }
        if(step == 0){
            return true;
        }
        unsigned dir = DIR_FOR_STEP[step];
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

    unsigned cBoard::search(unsigned src, unsigned step, int maxcnt){
        int cnt = 0;
        int dst = src + step;
        while(is_inbounds(src, dst, step) && cnt < maxcnt){
            unsigned piece = getfield(dst);
            if(piece != PIECES["blk"]){
                return dst;
            }
            cnt += 1;
            dst += step;
        }
        return PIECES["blk"];
    }

    bool cBoard::search_bi_dirs(unsigned *first, unsigned *second, unsigned src, int step, int maxcnt){
        int cnt = 0;
        *first = 65;
        int bisteps[2] = {step, (step * -1)};
        for(const int bistep : bisteps){
            int dst = src + bistep;
            while(is_inbounds(src, dst, bistep) || cnt < maxcnt){
                unsigned piece = getfield(dst);
                if(piece != PIECES["blk"]){
                    if(*first == 65){
                        *first = dst;
                        break;
                    }
                    else{
                        *second = dst;
                        return true;
                    }
                }
                cnt += 1;
                dst += bistep;
            }
            if(*first == 65){
                break;
            }
        }
        return false;
    }

    bool cBoard::is_nth(unsigned src, unsigned dst){
        if(src < dst && abs((int)src - (int)dst) % 8 == 0){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_sth(unsigned src, unsigned dst){
        if(src > dst && abs((int)src - (int)dst) % 8 == 0){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_est(unsigned src, unsigned dst){
        if(src % 8 < dst % 8 && src / 8 == dst / 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_wst(unsigned src, unsigned dst){
        if(src % 8 > dst % 8 && src / 8 == dst / 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_nth_est(unsigned src, unsigned dst){
        if(abs((int)src - (int)dst) % 9 == 0 && src < dst && src % 8 < dst % 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_sth_wst(unsigned src, unsigned dst){
        if(abs((int)src - (int)dst) % 9 == 0 && src > dst && src % 8 > dst % 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_nth_wst(unsigned src, unsigned dst){
        if(abs((int)src - (int)dst) % 7 == 0 && src < dst && src % 8 > dst % 8){
            return true;
        }
        else{
            return false;
        }
    }

    bool cBoard::is_sth_est(unsigned src, unsigned dst){
        if(abs((int)src - (int)dst) % 7 == 0 && src > dst && src % 8 < dst % 8){
            return true;
        }
        else{
            return false;
        }
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
