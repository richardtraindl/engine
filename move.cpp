

    #include "./move.hpp"


    cMove::cMove(uint8_t newsrc_x, uint8_t newsrc_y, uint8_t newdst_x, uint8_t newdst_y, uint8_t newsrcpiece, uint8_t newdstpiece, uint8_t newprompiece){ 
        src_x = newsrc_x;
        src_y = newsrc_y;
        dst_x = newdst_x;
        dst_y = newdst_y;
        srcpiece = newsrcpiece;
        dstpiece = newdstpiece;
        prompiece = newprompiece;
    }


    cMove::cMove(){
    }

    cMove::cMove(const cMove &move){
        src_x = move.src_x;
        src_y = move.src_y;
        dst_x = move.dst_x;
        dst_y = move.dst_y;
        srcpiece = move.srcpiece;
        dstpiece = move.dstpiece;
        prompiece = move.prompiece;  
    }


    bool cMove::is_en_passant(){

        if(srcpiece == mWPW){
            if(dstpiece == mBLK && src_y == 4 && dst_y == 5){
                if((src_x + 1 <= 7 && src_x + 1 == dst_x) || (src_x - 1 >= 0 && src_x - 1 == dst_x)){
                    return true;
                }
            }
        }
        else if(srcpiece == mBPW){
            if(dstpiece == mBLK && src_y == 3 && dst_y == 2){
                if((src_x + 1 <= 7 && src_x + 1 == dst_x) || (src_x - 1 >= 0 && src_x - 1 == dst_x)){
                    return true;
                }
            }
        }

        return false;

    }


    bool cMove::is_promotion(){

        if(srcpiece == mWPW){
            if(prompiece != mBLK && src_y == 6 && dst_y == 7){
                return true;
            }
        }
        else if(srcpiece == mBPW){
            if(prompiece != mBLK && src_y == 1 && dst_y == 0){
                return true;
            }
        }

        return false;

    }


   bool cMove::is_short_castling(){

        if(srcpiece == mWKG){
            if(src_y == 0 && dst_y == 0 && src_x == 4 && dst_x == 6){
                return true;
            }
        }
        else if(srcpiece == mBKG){
            if(src_y == 7 && dst_y == 7 && src_x == 4 && dst_x == 6){
                return true;
            }
        }

        return false;

    }


    bool cMove::is_long_castling(){

        if(srcpiece == mWKG){
            if(src_y == 0 && dst_y == 0 && src_x == 4 && dst_x == 2){
                return true;
            }
        }
        else if(srcpiece == mBKG){
            if(src_y == 7 && dst_y == 7 && src_x == 4 && dst_x == 2){
                return true;
            }
        }

        return false;

    }


    string cMove::format(){

        string hyphen = "";
        string trailing = "";

        if(is_en_passant()){
            return indices_to_coord(src_x, src_y) + "x" + indices_to_coord(dst_x, dst_y) + ", e.p.";
        }

        (dstpiece == mBLK)? hyphen = "-" : hyphen = "x";

        if(prompiece != mBLK){ 
            trailing = ", " + reverse_lookup(PIECES, prompiece); 
        }

        return indices_to_coord(src_x, src_y) + hyphen + indices_to_coord(dst_x, dst_y) + trailing;

    }


    void cMove::coord_to_indices(uint8_t &x, uint8_t &y, string coord){

        //if(coord.length() < 4){
        //    return;
        //}

        uint8_t x1 = (uint8_t)(coord[0]);
        if(x1 > (uint8_t)'H'){
            x = x1 - (uint8_t)'a';
        }
        else{
            x = x1 - (uint8_t)'A';
        }
        uint8_t y1 = (uint8_t)(coord[1]);
        y = y1 - (uint8_t)'1';

    }


    string cMove::indices_to_coord(uint8_t x, uint8_t y){

        ostringstream outstream;
        outstream << (char)(x + (uint8_t)'a' ) << (char)(y + (uint8_t)'1');
        return outstream.str();

    }

