

#ifndef TOUCH_HPP
    #define TOUCH_HPP

    #include <list>

    using namespace std;

    class cTouch{
        public:
            int piece;
            int pos;
            list<cTouch> attacker_beyond;
            list<cTouch> supporter_beyond;
            
            cTouch(int piece, int pos);
            cTouch();
    };

#endif