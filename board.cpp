
#include "./board.hpp"
#include "./values.hpp"
#include "./helper.hpp"

    using namespace std;

    cBoard::cBoard(){ 
        fields[0] = BASE0;
        fields[1] = BASE1;
        fields[2] = BASE2;
        fields[3] = BASE3;
        wKg = COLS["E"] + RANKS["1"] * 8;
        bKg = COLS["E"] + RANKS["8"] * 8;
        wKg_first_move_on = -1;
        bKg_first_move_on = -1;
        wRkA_first_move_on = -1;
        wRkH_first_move_on = -1;
        bRkA_first_move_on = -1;
        bRkH_first_move_on = -1;
    }

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
        return ((fields[0] >> (63 - idx)) & 0x1) | 
               ((fields[1] >> (62 - idx)) & 0x2) | 
               ((fields[2] >> (61 - idx)) & 0x4) | 
               ((fields[3] >> (60 - idx)) & 0x8);
    }

    void cBoard::setfield(int idx, int value){
        int eraseval, setval;
        for(int i = 0; i < 4; ++i){
            eraseval = (0x0000000000000001 << (63 - (i + idx))) ^ 0xFFFFFFFFFFFFFFFF;
            setval = value << (63 - (i + idx));
            fields[i] = (fields[i] & eraseval) | setval;
        }
    }

    void cBoard::copyfields(unsigned long long int _fields[]){
        for(int i = 0; i < 4; ++i){
            _fields[i] = fields[i];
        }
    }

    bool cBoard::comparefields(unsigned long long int _fields[]){
        for(int i = 0; i < 4; ++i){
            if(_fields[i] != fields[i]){
                return false;
            }
        }
        return true;    
    }

    bool cBoard::is_inbounds(int src, int dst, int step){
        if(src < 0 or src > 63 or dst < 0 or dst > 63){
            return false;
        }
        if(step == 0){
            return true;
        }
        int dir = DIR_FOR_STEP[step];
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
            if(piece != PIECES["blk"]){
                return dst;
            }
            cnt += 1;
            dst += step;
        }
        return -1;
    }

    bool cBoard::search_bi_dirs(int *first, int *second, int src, int step, int maxcnt){
        int cnt = 0;
        *first = -1;
        int bisteps[2] = {step, (step * -1)};
        for(const int bistep : bisteps){
            int dst = src + bistep;
            while(is_inbounds(src, dst, bistep) || cnt < maxcnt){
                int piece = getfield(dst);
                if(piece != PIECES["blk"]){
                    if(*first == -1){
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
            if(*first == -1){
                break;
            }
        }
        return false;
    }

/*

    def set_to_base(self):
        self.fields = self.BASE
        self.wKg = self.COLS["E"] + self.RANKS["1"] * 8
        self.bKg = self.COLS["E"] + self.RANKS["8"] * 8
        self.wKg_first_move_on = None
        self.bKg_first_move_on = None
        self.wRkA_first_move_on = None
        self.wRkH_first_move_on = None
        self.bRkA_first_move_on = None
        self.bRkH_first_move_on = None

    def clear(self):
        self.fields = 0x0
        self.wKg = None
        self.bKg = None
        self.wKg_first_move_on = None
        self.bKg_first_move_on = None
        self.wRkA_first_move_on = None
        self.wRkH_first_move_on = None
        self.bRkA_first_move_on = None
        self.bRkH_first_move_on = None

    def verify(self):
        wKg_cnt = 0
        bKg_cnt = 0
        wPw_cnt = 0
        bPw_cnt = 0
        wOfficer_cnt = 0
        bOfficer_cnt = 0
        for idx in range(64):
            piece = self.getfield(idx)
            if(piece == PIECES["wKg"]):
                wKg_cnt += 1
            elif(piece == PIECES["bKg"]):
                bKg_cnt += 1
            elif(piece == PIECES["wPw"]):
                wPw_cnt += 1
            elif(piece == PIECES["bPw"]):
                bPw_cnt += 1
            elif(piece == PIECES["wRk"] or piece == PIECES["wBp"] or 
                 piece == PIECES["wKn"] or piece == PIECES["wQu"]):
                wOfficer_cnt += 1
            elif(piece == PIECES["bRk"] or piece == PIECES["bBp"] or 
                 piece == PIECES["bKn"] or piece == PIECES["bQu"]):
                bOfficer_cnt += 1
            elif(piece == PIECES["blk"]):
                continue
            else:
                return False
        if(wKg_cnt != 1 or bKg_cnt != 1):
            return False
        if(wPw_cnt > 8 or bPw_cnt > 8):
            return False
        if(wPw_cnt + wOfficer_cnt > 15):
            return False
        if(bPw_cnt + bOfficer_cnt > 15):
            return False
        if(self.wKg is None or self.bKg is None):
            return False
        if(abs(self.wKg // 8 - self.bKg // 8) < 2 and abs(self.wKg % 8 - self.bKg % 8) < 2):
            return False
        return True

    @classmethod
    def is_inbounds_core(cls, src, dst):
        if(src < 0 or src > 63 or dst < 0 or dst > 63):
            return False
        else:
            return True

    @classmethod
    def is_nth(cls, src, dst):
        if(src < dst and abs(src - dst) % 8 == 0):
            return True
        else:
            return False

    @classmethod
    def is_sth(cls, src, dst):
        if(src > dst and abs(src - dst) % 8 == 0):
            return True
        else:
            return False

    @classmethod
    def is_est(cls, src, dst):
        if(src % 8 < dst % 8 and src // 8 == dst // 8):
            return True
        else:
            return False

    @classmethod
    def is_wst(cls, src, dst):
        if(src % 8 > dst % 8 and src // 8 == dst // 8):
            return True
        else:
            return False

    @classmethod
    def is_nth_est(cls, src, dst):
        if(abs(src - dst) % 9 == 0 and src < dst and src % 8 < dst % 8):
            return True
        else:
            return False

    @classmethod
    def is_sth_wst(cls, src, dst):
        if(abs(src - dst) % 9 == 0 and src > dst and src % 8 > dst % 8):
            return True
        else:
            return False

    @classmethod
    def is_nth_wst(cls, src, dst):
        if(abs(src - dst) % 7 == 0 and src < dst and src % 8 > dst % 8):
            return True
        else:
            return False

    @classmethod
    def is_sth_est(cls, src, dst):
        if(abs(src - dst) % 7 == 0 and src > dst and src % 8 < dst % 8):
            return True
        else:
            return False

    @classmethod
    def erase_whites(cls, fields):
        mask = fields & cls.BITS1000
        mask = mask | mask >> 1 | mask >> 2 | mask >> 3
        return fields & mask

    @classmethod
    def erase_blacks(cls, fields):
        mask = fields & cls.BITS1000
        mask = (mask | mask >> 1 | mask >> 2 | mask >> 3)
        return fields & (mask ^ cls.FULL)

    @classmethod
    def mask_pieces(cls, fields, piece):
        negative = fields ^ PIECES_MASK[piece]
        negative = negative | ((negative & cls.BITS1110) >> 1)
        negative = negative | ((negative & cls.BITS1100) >> 2)
        negative = negative | ((negative & cls.BITS0111) << 1)
        negative = negative | ((negative & cls.BITS0011) << 2)
        return fields & (negative ^ cls.FULL)

# class end

*/
