#ifndef SEARCHHELPER_HPP
    #define SEARCHHELPER_HPP

    #include <list> 
    #include "../match.hpp"
    #include "../values.hpp"

    class cTouch{
        public:
            int piece;
            int pos;
            list<cTouch> attacker_beyond;
            list<cTouch> supporter_beyond;
            
            cTouch(int piece, int pos);
            cTouch();
    };

    class cSearchHelper{
        private:
            static int RKSTEPS[4];
            static int RKMAXCNT;
            static int RKTARGETS[4];
            static int BPSTEPS[4];
            static int BPMAXCNT;
            static int BPTARGETS[4];
            static int KGSTEPS[8];
            static int KGMAXCNT;
            static int KGTARGETS[2];
            static int KNSTEPS[8];
            static int KNMAXCNT;
            static int KNTARGETS[2];
            static int WPSTEPS[2];
            static int WPMAXCNT;
            static int WPTARGETS[1];
            static int BPSTEPS[2];
            static int BPMAXCNT;
            static int BPTARGETS[1];

            bool _is_field_touched(cMatch *match, int src, int color, int mode, int steps[], int maxcnt, int targets[]);
            bool _is_field_touched_by_king(cMatch *match, int src, int color, int mode, int steps[], int maxcnt, int targets[]);
            void _search_for_color_pieces(cMatch *match, int src, int color, int steps[], int maxcnt, int targets[], list<cTouch> *touches);
            void _search_for_both_colors_pieces(cMatch *match, int src, int color, int steps[], int maxcnt, int targets[], list<cTouch> *frdlytouches, list<cTouch> *enmytouches);
        public:
            bool is_field_touched(cMatch *match, int src, int color, int mode);
            void field_touches_for_both(cMatch *match, int src, int color, list<cTouch> *friends, list<cTouch> * enmies);
            list<cTouch> field_touches_for_color(cMatch *match, int src, int color);
            void add_field_touches_beyond(cMatch *match, int color, cTouch *ctouch);
    };

#endif

