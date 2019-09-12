
    #include "./touch.hpp"

    cTouch::cTouch(int _piece, int _pos){
        piece = _piece;
        pos = _pos;
    }

    cTouch::cTouch(){
    }

    cExclude::cExclude(int newpos, int newtouched_pos, int newmvdir, int newdomain){
        pos = newpos;
        touched_pos = newtouched_pos;
        mvdir = newmvdir;
        domain = newdomain;
    }

