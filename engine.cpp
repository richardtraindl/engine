
#include <iostream>
#include "./match.hpp"
#include "./values.hpp"


    int main(void){
        cMatch match;
        // cout << "\nmatch.status " << match.calcStatus();
        cout << "\nF " << match.STATUS["active"];
        cout << "\nD " << match.board.wKg_x;
        cout << "\nC " << PIECES["wKg"];
        
        // cout << "match.board->fields " << match.board.getFields();
        
        // struct session kant;
        // neu(&kant);
        // init_words();
        // forth(&kant);

        // perform_respond(&kant);

        return 0;
    }

