
    #include "./analyze_move.hpp"
    #include "./analyze_helper.hpp"
    #include "./analyze_tactics.hpp"
    #include "../values.hpp"
    #include "../helper.hpp"


    bool find_excluded(list<cExclude> excludes, int pos, int touch_pos, int tactic_domain){
        for(list<cExclude>::iterator it = excludes.begin(); it != excludes.end(); ++it){
            if(it->pos == pos && it->touch_pos == touch_pos && it->tactic_domain == tactic_domain){
                return true;
        }
        return false;
    }


    void add_tactics(cPrioMove &priomove, cMatch &match, cMove *candidate, cMove *dbggmove, list<cExclude> &excludes){
        int rook = mBLK;
        list<cTouch> from_castl_rk_supported, from_castl_rk_attacked;

        int piece = match.board.getfield(priomove.src);
        int dstpiece = match.board.getfield(priomove.dst);

        list<cTouch>from_dstfield_supported, from_dstfield_attacked;
        find_touches_after_move(match, priomove, from_dstfield_supported, from_dstfield_attacked);

        list<cTouch> discl_supported, discl_attacked;
        find_disclosures(match, piece, priomove, discl_supported, discl_attacked);

        int weight = weight_for_standard(match, piece, priomove);


        if(candidate != NULL){
            if(candidate->src == priomove.src && candidate->dst == priomove.dst && candidate->prompiece == priomove.prompiece){
                priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["prev-candidate"], cTactic::WEIGHTS["good-deal"], PIECES_RANKS[piece])));
            }
        }


        if(defends_check(match)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["defends-check"], weight, PIECES_RANKS[piece])));
        }


        if(castles(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["castles"], weight, PIECES_RANKS[piece])));
            find_rook_touches_after_castling(match, priomove, rook, from_castl_rk_supported, from_castl_rk_attacked);
        }


        if(is_tactical_draw(match, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["is-tactical-draw"], cTactic::WEIGHTS["good-deal"], PIECES_RANKS[piece])));
        }


        if(promotes(priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["promotes"], weight, PIECES_RANKS[piece])));
        }

        
        if(captures(match, priomove)){
            int capture_weight = weight_for_capture(match, piece, dstpiece, priomove);
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["captures"], capture_weight, PIECES_RANKS[piece])));
        }
        if(captures_enpassant(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["captures"], cTactic::WEIGHTS["good-deal"], PIECES_RANKS[piece])));
        }
            

        if(does_unpin(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["unpins"], weight, PIECES_RANKS[piece])));
        }


        if(forks(piece, from_dstfield_attacked)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["forks"], weight, PIECES_RANKS[piece])));
        }


        if(defends_fork(match, piece, dstpiece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["defends-fork"], weight, PIECES_RANKS[piece])));
        }


        if(threatens_fork(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["threatens-fork"], weight, PIECES_RANKS[piece])));
        }


        if(flees(match, piece, priomove)){
            int flee_weight = weight_for_flee(match, piece, priomove, weight);
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["flees"], flee_weight, PIECES_RANKS[piece])));
            if(find_excluded(excludes, priomove.src, cBoard::SNOK, cTactic::DOMAINS["flees"])){
                priomove.downgrade(cTactic::DOMAINS["flees"]);
            }
            else{
                excludes.push_back(*(new cExclude(priomove.src, cBoard::SNOK, cTactic::DOMAINS["flees"])));
            }
        }


        if(from_dstfield_supported.size() > 0){
            fill_supported(match, piece, priomove, from_dstfield_supported, weight, excludes);
        }
        if(rook != mBLK){
            fill_supported(match, rook, priomove, from_castl_rk_supported, weight, excludes);
        }


        if(from_dstfield_attacked.size() > 0){
            fill_attacked(match, piece, priomove, from_dstfield_attacked, weight, excludes);
        }
        if(rook != mBLK){
            fill_attacked(match, rook, priomove, from_castl_rk_attacked, weight, excludes);
        }


        if(discl_attacked.size() > 0){
            int weight_for_discl_attacking_piece = cTactic::WEIGHTS["undef"];
            int touchpos_for_piece;
            for(list<cTouch>::iterator it = discl_attacked.begin(); it != discl_attacked.end(); ++it){
                int new_weight_for_discl_attacking_piece = weight_for_discl_attacking(*it, weight);
                if(new_weight_for_discl_attacking_piece < weight_for_discl_attacking_piece){
                    weight_for_discl_attacking_piece = new_weight_for_discl_attacking_piece;
                    touchpos_for_piece = it->pos;
                }
            }
            if(weight_for_discl_attacking_piece < cTactic::WEIGHTS["undef"]){
                priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["attacks"], weight_for_discl_attacking_piece, PIECES_RANKS[piece])));
                if(find_excluded(excludes, priomove.src, touchpos_for_piece, cTactic::DOMAINS["attacks"])){
                    priomove.downgrade(cTactic::DOMAINS["attacks"]);
                }
                else{
                    excludes.push_back(*(new cExclude(priomove.src, touchpos_for_piece, cTactic::DOMAINS["attacks"])));
                }
            }
        }


        if(discl_supported.size() > 0){
            int weight_for_discl_supporting_piece = cTactic::WEIGHTS["undef"];
            int touchpos_for_piece;
            for(list<cTouch>::iterator it = discl_supported.begin(); it != discl_supported.end(); ++it){
                int new_weight_for_discl_supporting_piece = weight_for_discl_supporting(*it, weight);
                if(new_weight_for_discl_supporting_piece < weight_for_discl_supporting_piece){
                    weight_for_discl_supporting_piece = new_weight_for_discl_supporting_piece;
                    touchpos_for_piece = it->pos;
                }
            }
            if(weight_for_discl_supporting_piece < cTactic::WEIGHTS["undef"]){
                priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["supports"], weight_for_discl_supporting_piece, PIECES_RANKS[piece])));
                if(find_excluded(excludes, priomove.src, touchpos_for_piece, cTactic::DOMAINS["supports"])){
                    priomove.downgrade(cTactic::DOMAINS["supports"]);
                }
                else{
                    excludes.push_back(*(new cExclude(priomove.src, touchpos_for_piece, cTactic::DOMAINS["supports"])));
                }
            }
        }


        if(blocks(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["blocks"], weight, PIECES_RANKS[piece])));
        }


        if(running_pawn(match, priomove)){
            int running_weight = weight_for_running_pawn(match, piece, priomove, weight);
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["is-running-pawn"], running_weight, PIECES_RANKS[piece])));
        }

        /*
        if(controles_file(match, move)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["controles-file"], weight))

        if(is_progress(match, move)):
            priomove.tactics.append(cTactic(cTactic.DOMAINS["is-progress"], weight))
        */


        if(match.is_endgame() && is_approach_to_opp_king(match, piece, priomove)){
            priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["approach-opp-king"], weight, PIECES_RANKS[piece])));
        }


        if(dbggmove != NULL && dbggmove->src == priomove.src && dbggmove->dst == priomove.dst){
            priomove.prio = 1;
        }
        else{
            priomove.evaluate_priority();
        }
    }
