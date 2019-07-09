#ifndef SEARCHFORPIECE_HPP
    #define SEARCHFORPIECE_HPP

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

    class cSearchForPiece{
        private:
            static int STEPS[1];
            static int MAXCNT;
            static int TARGETS[1];
        public:
            cSearchForPiece();

            static bool is_field_touched(cMatch *match, int src, int color, int mode);

            static void field_touches_for_both(cMatch *match, int src, int color, list<cTouch> *frdlytouches, list<cTouch> *enmytouches);

            static void field_touches_for_color(cMatch *match, int src, int color, list<cTouch> *touches);
    };


    class SearchforRook : public cSearchForPiece{
         private:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
    };


    class SearchforBishop : public cSearchForPiece{
        private:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
    };


    class SearchforKing : public cSearchForPiece{
        private:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];
        public:
            static bool is_field_touched(cMatch *match, int src, int color);
    };


    class SearchforKnight : public cSearchForPiece{
        private:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];
    };

    class SearchforWhitePawn : public cSearchForPiece{
        private:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
    };


    class SearchforBlackPawn : public cSearchForPiece{
        private:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
    };


    bool is_field_touched(cMatch *match, int src, int color, int mode);

    void field_touches_for_both(cMatch *match, int src, int color, list<cTouch> *friends, list<cTouch> * enmies);

    void add_field_touches_beyond(cMatch *match, int color, cTouch *ctouch);

    list<cTouch> field_touches_for_color(cMatch *match, int src, int color);

#endif
