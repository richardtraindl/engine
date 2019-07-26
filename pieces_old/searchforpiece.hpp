
#ifndef SEARCHFORPIECE_HPP
    #define SEARCHFORPIECE_HPP

    #include <list>
    #include "./touch.hpp"
    #include "../board.hpp"
    #include "../values.hpp"

    using namespace std;

    class cSearchforRook{
        public:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];

            static bool _is_field_touched(cBoard *board, int src, int color, int mode);
            static void _collect_touches_for_both_colors(cBoard *board, int src, int friendlycolor, list<cTouch> *frdlytouches, list<cTouch> *enmytouches);
            static void _collect_touches_for_color(cBoard *board, int src, int color, list<cTouch> *touches);
    };


    class cSearchforBishop : public cSearchforRook{
        public:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
    };


    class cSearchforKing : public cSearchforRook{
        public:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];

            static bool _is_field_touched(cBoard *board, int src, int color);
    };


    class cSearchforKnight : public cSearchforRook{
        public:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];
    };


    class cSearchforWhitePawn : public cSearchforRook{
        public:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
    };


    class cSearchforBlackPawn : public cSearchforRook{
        public:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
    };


    bool is_field_touched(cBoard *board, int src, int color, int mode);

    void collect_touches_for_both_colors(cBoard *board, int src, int color, list<cTouch> *friends, list<cTouch> *enmies);

    void collect_touches_for_color(cBoard *board, int src, int color, list<cTouch> *touches);

    void add_field_touches_beyond(cBoard *board, int color, cTouch *ctouch);

#endif