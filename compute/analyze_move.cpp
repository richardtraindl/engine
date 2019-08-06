
    #include "./analyze_move.hpp"
    #include "./analyze_helper.hpp"
    #include "./analyze_tactics.hpp"
    #include "../values.hpp"
    #include "../helper.hpp"


    void add_tactics(cPrioMove &priomove, cMatch &match, cMove *candidate, cMove *dbggmove, bool search_for_mate, list<cPrioMove> &excludes){
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
            int capture_weight = weight_for_capture(match, piece, dstpiece, priomove, weight);
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
            cTactic tactic(cTactic::DOMAINS["flees"], flee_weight, PIECES_RANKS[piece]);
            priomove.tactics.push_back(tactic);
        }

        fill_attacked(match, piece, priomove, search_for_mate, from_dstfield_attacked, weight);
        if(rook != mBLK){
            fill_attacked(match, rook, priomove, search_for_mate, from_castl_rk_attacked, weight);
        }

        fill_supported(match, piece, priomove, from_dstfield_supported, weight);
        if(rook != mBLK){
            fill_supported(match, rook, priomove, from_castl_rk_supported, weight);
        }

        if(discl_attacked.size() > 0){
            int weight_for_attacking_piece = cTactic::WEIGHTS["undef"];
            for(list<cTouch>::iterator it = discl_attacked.begin(); it != discl_attacked.end(); ++it){
                weight_for_attacking_piece = min(weight_for_attacking_piece, weight_for_discl_attacking(*it, weight));
            }
            if(weight_for_attacking_piece < cTactic::WEIGHTS["undef"]){
                priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["attacks"], weight_for_attacking_piece, PIECES_RANKS[piece])));
            }
        }

        if(discl_supported.size() > 0){
            int weight_for_supporting_piece = cTactic::WEIGHTS["undef"];
            for(list<cTouch>::iterator it = discl_supported.begin(); it != discl_supported.end(); ++it){
                weight_for_supporting_piece = min(weight_for_supporting_piece, weight_for_discl_supporting(*it, weight));
            }
            if(weight_for_supporting_piece < cTactic::WEIGHTS["undef"]){
                priomove.tactics.push_back(*(new cTactic(cTactic::DOMAINS["supports"], weight_for_supporting_piece, PIECES_RANKS[piece])));
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

        priomove.evaluate_priority();
    }
