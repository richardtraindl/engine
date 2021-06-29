

    #include "./analyze_position.hpp"


    cCandidate_new::cCandidate_new(uint8_t newmin_matches_cnt, 
                                   uint8_t newmax_matches_cnt, 
                                   uint64_t *newfield,
                                   list<uint64_t> &newblanks){ 

        min_matches_cnt = newmin_matches_cnt;

        max_matches_cnt = newmax_matches_cnt;

        for(uint8_t i = 0; i < 8; ++i){
            field[i] = *(newfield + i);
        }

        for(uint64_t newblank : newblanks){
            uint64_t *blank = new uint64_t(newblank);
            blanks.push_back(*blank);
        }
    }


    cCandidate_new::cCandidate_new(){
    }


    uint8_t cCandidate_new::calc_matches_cnt(uint64_t *newfield){
        uint8_t count = 0;

        for(uint8_t i = 0; i < 8; ++i){
            if((field[i] & *(newfield + i)) == *(newfield + i)){
                count++;
            }
        }

        for(uint64_t newblank : blanks){
            if((field[mIDX_WHITE] ^ newblank) == newblank &&
               (field[mIDX_BLACK] ^ newblank) == newblank){
                count++;
            }
        }

        return count;
    }


    cCandidate_ori::cCandidate_ori(list<uint64_t *> &newsrcpositions, list<uint64_t *> &newdstpositions){ 
        for(uint64_t *srcpos : newsrcpositions){
            uint64_t *srcposition = new uint64_t[9];

            for(int i = 0; i < 9; ++i){
                *(srcposition + i) = *(srcpos + i);
            }

            srcpositions.push_back(srcposition);
        }

        for(uint64_t *dstpos : newdstpositions){
            uint64_t *dstposition = new uint64_t[9];

            for(int i = 0; i < 9; ++i){
                *(dstposition + i) = *(dstpos + i);
            }

            dstpositions.push_back(dstposition);
        }
    }


    cCandidate_ori::cCandidate_ori(){
    }


    bool cCandidate_ori::find(cMatch &match, list<cGMove *> &rcandidates){
        bool found = false;

        for(uint64_t *srcpos : srcpositions){
            found = true;

            for(uint8_t i = 0; i < 8; ++i){
                if((match.board.field[i] & *(srcpos + i)) != *(srcpos + i)){
                    found = false;
                    break;
                }
            }

            if(found){
                if((match.board.field[mIDX_WHITE] & *(srcpos + 8)) == 0 &&
                   (match.board.field[mIDX_BLACK] & *(srcpos + 8)) == 0){
                    return _find(match, dstpositions, 1, rcandidates);
                }
            }
        }

        return false;
    }


    bool cCandidate_ori::_find(cMatch &match, list<uint64_t *> &dstpositions, uint8_t depth, list<cGMove *> &rcandidates){
        if(depth > 3){
            return false;
        }

        list<cGMove *> moves;
        gen_moves(match, moves);

        if(moves.size() == 0){
            return false;
        }

        bool found;

        for(cGMove *move : moves){
            match.do_move(move->src, move->dst, move->prompiece);

            for(uint64_t *dstpos : dstpositions){
                found = true;

                for(int i = 0; i < 8; ++i){
                    if((match.board.field[i] & *(dstpos + i)) != *(dstpos + i)){
                        found = false;
                        break;
                    }
                }
                if(found){
                    if((match.board.field[mIDX_WHITE] & *(dstpos + 8)) == 0 &&
                       (match.board.field[mIDX_BLACK] & *(dstpos + 8)) == 0){
                        rcandidates.push_back(new cGMove(move->src, move->dst, move->prompiece));
                        match.undo_move();
                        return true;
                    }
                }
            }

            found = _find(match, dstpositions, depth + 1, rcandidates);

            if(found){
                rcandidates.push_back(new cGMove(move->src, move->dst, move->prompiece));
                match.undo_move();
                return true;
            }
            else{
                match.undo_move();
            }
        }

        return false;
    }


    cSearchCandidates_new::cSearchCandidates_new(){
    }


    cSearchCandidates::cSearchCandidates(){
        list<uint64_t *> white_dstpositions;

        uint64_t dstpositions[][9] = 
            { { 0x0000000100000010, 
                0x0000000000000408, 
                0x0000000000000008, 
                0x0000000000000000, 
                0x0000000000000010, 
                0x0000000100000000, 
                0x0000000000000000, 
                0x0000000000000400,
                0x0000000002040800 },
              { 0x0000000002000010, 
                0x0000000000000408, 
                0x0000000000000008, 
                0x0000000000000000, 
                0x0000000000000010, 
                0x0000000002000000, 
                0x0000000000000000, 
                0x0000000000000400,
                0x0000000000040800 } };

        for(uint64_t *dstpos : dstpositions){
            white_dstpositions.push_back(dstpos);
        }


        list<uint64_t *> white_srcpositions;

        uint64_t srcpositions[][9] = 
            { { 0x1000000100000000, 
                0x0000000000000408, 
                0x0000000000000008, 
                0x0000000000000000, 
                0x1000000000000000, 
                0x0000000100000000, 
                0x0000000000000000, 
                0x0000000000000400,
                0x0000000002040800 },
              { 0x1000000002000000, 
                0x0000000000000408, 
                0x0000000000000008, 
                0x0000000000000000, 
                0x1000000000000000, 
                0x0000000002000000, 
                0x0000000000000000, 
                0x0000000000000400,
                0x0000000000040800 },
              { 0x1000000000040000, 
                0x0000000000000408, 
                0x0000000000000008, 
                0x0000000000000000, 
                0x1000000000000000, 
                0x0000000000040000, 
                0x0000000000000000, 
                0x0000000000000400,
                0x0000000000000800 } };

        for(uint64_t *srcpos : srcpositions){
            white_srcpositions.push_back(srcpos);
        }

        cCandidate_ori white_candidate(white_srcpositions, white_dstpositions);

        white_search_candidates.push_back(white_candidate);
    }


    bool cSearchCandidates::search(cMatch &match, uint8_t color, list<cGMove *> &rcandidates){
        if(color == mWHITE){
            for(cCandidate_ori candidate : white_search_candidates){
                return candidate.find(match, rcandidates);
            }
        }
        else{
            for(cCandidate_ori candidate : black_search_candidates){
                return candidate.find(match, rcandidates);
            }
        }

        return false;
    }

