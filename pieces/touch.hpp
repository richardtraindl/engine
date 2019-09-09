

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
            int touched_pos;
            int mvdir;
            int domain;

            cExclude(int newpos, int newtouched_pos, int newmvdir, int newdomain);
    };

#endif