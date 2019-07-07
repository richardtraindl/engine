
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

            bool is_field_touched(cMatch *match, int src, int color, int mode);

            void list_all_field_touches(cMatch *match, int src, int color, list<cTouch> *frdlytouches, list<cTouch> *enmytouches);

            void list_field_touches_for_color(cMatch *match, int src, int color, list<cTouch> *touches);
    };


    class SearchforRook : public cSearchForPiece{
         private:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
        public:
            SearchforRook();
    };


    class SearchforBishop : public cSearchForPiece{
        private:
            static int STEPS[4];
            static int MAXCNT;
            static int TARGETS[4];
        public:
            SearchforBishop();
    };


    class SearchforKing : public cSearchForPiece{
        private:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];
        public:
            SearchforKing();
            bool is_field_touched(cMatch *match, int src, int color);
    };


    class SearchforKnight : public cSearchForPiece{
        private:
            static int STEPS[8];
            static int MAXCNT;
            static int TARGETS[2];
        public:
            SearchforKnight();
    };

    class SearchforWhitePawn : public cSearchForPiece{
        private:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
        public:
            SearchforWhitePawn();
    };


    class SearchforBlackPawn : public cSearchForPiece{
        private:
            static int STEPS[2];
            static int MAXCNT;
            static int TARGETS[1];
        public:
            SearchforBlackPawn();
    };


    bool is_field_touched(cMatch *match, int src, int color, int mode);

    void list_all_field_touches(cMatch *match, int src, int color, list<cTouch> *friends, list<cTouch> * enmies);

    void list_field_touches_beyond(cMatch *match, int color, cTouch *ctouch);

    list<cTouch> *list_field_touches(cMatch *match, int src, int color);

#endif