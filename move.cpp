
    #include "./move.hpp"


    cGMove::cGMove(uint64_t newsrc, uint64_t newdst, uint8_t newprompiece){ 
        src = newsrc;
        dst = newdst;
        prompiece = newprompiece;
        presort = PRESORT_LOW;
    }


    cGMove::cGMove(){
    }


    const uint8_t cGMove::PRESORT_STEP = 30;
    const uint8_t cGMove::PRESORT_STORMY = PRESORT_STEP;
    const uint8_t cGMove::PRESORT_HIGH = PRESORT_STORMY + PRESORT_STEP;
    const uint8_t cGMove::PRESORT_MEDIUM = PRESORT_HIGH + PRESORT_STEP;
    const uint8_t cGMove::PRESORT_LOW = PRESORT_MEDIUM + PRESORT_STEP;


    string cGMove::format(bool core){
        string trailing = "";

        if(prompiece != mBLK){
            trailing = ", " + reverse_lookup(PIECES, prompiece);
        }

        if(core){
            return pos_to_coord(src) + " " + pos_to_coord(dst) + trailing;
        }
        else{
            return pos_to_coord(src) + " " + pos_to_coord(dst) + " presort: " + to_string(presort) + trailing;
        }
    }


    cMove::cMove(uint8_t newtype, 
                 uint64_t newsrc, 
                 uint64_t newdst, 
                 uint8_t newprompiece, 
                 uint64_t *newprev_field){ 
        type = newtype;
        src = newsrc;
        dst = newdst;
        prompiece = newprompiece;
        for(int i = 0; i < 8; ++i){
            prev_field[i] = *(newprev_field + i);
        }
    }
    cMove::cMove(){
    }


    string cMove::format(){
        string trailing = "";
        if(prompiece != mBLK){
            trailing = ", " + reverse_lookup(PIECES, prompiece);
        }

        if(type == MOVE_TYPE["standard"]){
            return pos_to_coord(src) + "-" + pos_to_coord(dst) + trailing;
        }

        if(type == MOVE_TYPE["capture"]){
            return pos_to_coord(src) + "x" + pos_to_coord(dst) + trailing;
        }

        if(type == MOVE_TYPE["en-passant"]){
            return pos_to_coord(src) + "x" + pos_to_coord(dst) + ", e.p.";
        }

        if(type == MOVE_TYPE["short-castling"]){
            return string("0-0");
        }

        if(type == MOVE_TYPE["long-castling"]){
            return string("0-0-0");
        }
        
        return "";
    }

