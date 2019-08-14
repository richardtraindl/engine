
    #include "./touch.hpp"

    cTouch::cTouch(int _piece, int _pos){
        piece = _piece;
        pos = _pos;
    }

    cTouch::cTouch(){
    }

    cExclude::cExclude(int newpos, int newtouch_pos, int newtactic_domain){
        pos = newpos;
        touch_pos = newtouch_pos;
        tactic_domain = newtactic_domain;
    }
