#ifndef ANALYZE_TACTICS_HPP
    #define ANALYZE_TACTICS_HPP

    #include <list>
    #include "../match.hpp"
    #include "../move.hpp"

    using namespace std;


    class cDirTouch{
        public:
            int piece;
            int pos;
            int dir;
            cDirTouch(int piece, int pos, int dir);
    };

    //list<cDirTouches> *search_dir(match, int pos, int dir, int excl_dir);

    //int[] search_lines_of_pin(cMatch &match, int color, int pos, int excl_dir);

    //bool is_supported_running_pawn(cMatch &match, cTouch &supported);

    bool castles(cMatch &match, int piece, cMove &move);

    bool promotes(cMove &move);

    bool captures(cMatch &match, int piece, cMove &move);

    //bool forks(int piece, list<cTouch> &from_dstfield_attacked);

    //bool defends_fork(cMatch &match, int piece, cMove &move, int dstpiece);
 
    //bool threatens_fork(cMatch &match, int piece, cMove &move);

    //bool flees(cMatch &match, int piece, cMove &move);

    //void find_touches_after_move(cMatch &match, cMove move, list<cTouch> &supported, list<cTouch> &attacked);

    //void find_rook_touches_after_castling(cMatch &match, cMove move, ist<cTouch> &supported, list<cTouch> &attacked);

    //bool does_unpin(cMatch &match, int piece, cMove &move);

    bool defends_check(cMatch &match);

    //void find_disclosures(cMatch &match, cMove &move, list<cTouch> &discl_supported, list<cTouch> &discl_attacked);

    //bool blocks(cMatch &match, int piece, cMove &move);

    //bool running_pawn(cMatch &match, int piece, cMove &move);

    //bool defends_invasion(cMatch &match, cMove &move);

    //bool controles_file(cMatch &match, cMove &move):

    //bool is_tactical_draw(cMatch &match, cMove &move);

    //bool is_progress(cMatch &match, cMove &move);

    //bool is_approach_of_opp_king(cMatch &match, int piece, cMove &move);

#endif
