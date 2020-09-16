
#ifndef SEARCHFORPIECE_HPP
    #define SEARCHFORPIECE_HPP

    #include <list>
    #include "./touch.hpp"
    #include "../board.hpp"

    using namespace std;


    class cSearchforPiece{
        public:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
    };


    class cSearchforRook : public cSearchforPiece{
        public:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
    };


    class cSearchforBishop : public cSearchforPiece{
        public:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
    };


    class cSearchforKing : public cSearchforPiece{
        public:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];
    };


    class cSearchforKnight : public cSearchforPiece{
        public:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];
    };


    class cSearchforWhitePawn : public cSearchforPiece{
        public:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
    };


    class cSearchforBlackPawn : public cSearchforPiece{
        public:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
    };


    bool _is_field_touched(cBoard &board, int src, int color, int mode);
    
    bool _is_field_touched_by_king(cBoard &board, int src, int color);

    bool is_field_touched(cBoard &board, int src, int color, int mode);

    void _collect_touches_for_both_colors(cBoard &board, int src, int friendlycolor, list<cTouch> &frdlytouches, list<cTouch> &enmytouches);

    void collect_touches_for_both_colors(cBoard &board, int src, int color, list<cTouch> &friends, list<cTouch> &enmies);

    void _collect_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches);

    void collect_long_distance_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches);

    void collect_touches_for_color(cBoard &board, int src, int color, list<cTouch> &touches);

    void add_field_touches_beyond(cBoard &board, int color, cTouch &touch);

#endif
