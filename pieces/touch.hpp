

#ifndef TOUCH_HPP
    #define TOUCH_HPP

    #include <list>

    using namespace std;

    class cTouch{
        public:
            int piece;
            int pos;
            list<cTouch> supporter_beyond;
            list<cTouch> attacker_beyond;
            
            cTouch(int piece, int pos);
            cTouch();
    };

    class cExclude{
        public:
            int pos;
            int touch_pos;
            int tactic_domain;
         
            cExclude(int newpos, int newtouch_pos, int newtactic_domain);
    };

#endif