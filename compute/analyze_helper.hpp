#ifndef ANALYZE_HELPER_HPP
    #define ANALYZE_HELPER_HPP

    #include <list> 
    #include <string>
    #include "../match.hpp"
    #include "../board.hpp"
    #include "../move.hpp"
    #include "../pieces/touch.hpp"

    using namespace std;
    
    class cAnalyzeField{
        private:
            cBoard *board;
            int pos;
            int piece;
            list<cTouch> friends_on_field;
            list<cTouch> enmies_on_field;

            void _is_field_ok();
            void _is_clean();
            void _is_supported();
            void _is_attacked();
            void _is_suppsize_eq_attsize();
            void _is_suppsize_lt_attsize();
            void _is_suppsize_gt_attsize();
            void _is_attacker_lt_piece();
            void _is_soft_pinned();
        public:
            bool is_field_ok;
            bool is_clean;
            bool is_supported;
            bool is_attacked;
            bool is_suppsize_eq_attsize;
            bool is_suppsize_lt_attsize;
            bool is_suppsize_gt_attsize;
            bool is_soft_pinned;
            int lowest_attacker;
            cAnalyzeField(cBoard &newboard, int newpos);
            static int lowest_piece(list<cTouch> touches);
    };
    

    bool are_fairy_equal(int piece1, int piece2);

    bool is_move_out_of_soft_pin(cMatch &match, int piece, cPrioMove &priomove);

    bool is_supply(cMatch &match, int piece, cPrioMove &priomove);

    bool are_move_dirs_equal(int piece1, int src1, int dst1, int piece2, int src2, int dst2);

    int search_for_checkmate(cMatch &match);

    int _search_for_checkmate(cMatch &match, int count, int maxcnt);

    bool find_excluded(list<cExclude*> &excludes, int pos, int touch_pos, int mvdir, int domain);

    int weight_for_standard(cMatch &match, int piece, cPrioMove &priomove, cAnalyzeField &analyzedst);

    int weight_for_capture(cMatch &match, int piece, int dstpiece, cPrioMove &priomove, cAnalyzeField &analyzedst);

    int weight_for_flee(cMatch &match, int piece, cPrioMove &priomove, cAnalyzeField &analyzesrc, cAnalyzeField &analyzedst);

    int weight_for_discl_support(cMatch &match, int piece, cPrioMove &priomove, cTouch &discl_supported, cAnalyzeField &analyzedst);

    int weight_for_discl_attack(cMatch &match, int piece, cPrioMove &priomove, cTouch &discl_attacked, cAnalyzeField &analyzedst);

    int weight_for_support(cMatch &match, int piece, cPrioMove &priomove, bool iscastlrook, cTouch &supported, cAnalyzeField &analyzedst);

    int weight_for_attack(cMatch &match, int piece, cPrioMove &priomove, bool iscastlrook, cTouch &attacked, cAnalyzeField &analyzedst);

    int weight_for_attack_on_king(cMatch &match, int piece, cPrioMove &priomove, cTouch &attacked, cAnalyzeField &analyzedst);
#endif
