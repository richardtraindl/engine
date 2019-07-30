#ifndef ANALYZE_TACTICS_HPP
    #define ANALYZE_TACTICS_HPP

    #include <list>
    #include "../match.hpp"
    #include "../move.hpp"
    #include "../pieces/touch.hpp"

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

    bool is_supported_running_pawn(cMatch &match, cTouch &supported);

    bool castles(cMatch &match, int piece, cPrioMove &priomove);

    bool promotes(cPrioMove &priomove);

    bool captures(cMatch &match, int piece, cPrioMove &priomove);

    //bool forks(int piece, list<cTouch> &from_dstfield_attacked);

    //bool defends_fork(cMatch &match, int piece, cMove &move, int dstpiece);
 
    //bool threatens_fork(cMatch &match, int piece, cMove &move);

    bool flees(cMatch &match, int piece, cPrioMove &priomove);

    void find_touches_after_move(cMatch &match, cPrioMove &priomove, list<cTouch> &supported, list<cTouch> &attacked);

    void find_rook_touches_after_castling(cMatch &match, cPrioMove &priomove, int &rook, list<cTouch> &supported, list<cTouch> &attacked);

    void fill_attacked(cMatch &match, int piece, cPrioMove &priomove, bool search_for_mate, list<cTouch> &attacked);

    void fill_supported(cMatch &match, int piece, cPrioMove &priomove, list<cTouch> &supported);

    //bool does_unpin(cMatch &match, int piece, cMove &move);

    bool defends_check(cMatch &match);

    //void find_disclosures(cMatch &match, cMove &move, list<cTouch> &discl_supported, list<cTouch> &discl_attacked);

    //bool blocks(cMatch &match, int piece, cMove &move);

    bool running_pawn(cMatch &match, cPrioMove &priomove);

    //bool defends_invasion(cMatch &match, cMove &move);

    //bool controles_file(cMatch &match, cMove &move):

    bool is_tactical_draw(cMatch &match, cPrioMove &priomove);

    bool is_progress(cMatch &match, cPrioMove &priomove);

    bool is_approach_to_opp_king(cMatch &match, int piece, cPrioMove &priomove);

#endif
