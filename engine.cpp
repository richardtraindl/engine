
#include <iostream>
#include "./match.hpp"
#include "./values.hpp"

    using namespace std;

    int main(void){
        cMatch match;
        cout << "\nF " << match.STATUS["active"];
        cout << "\nD " << match.board.wKg;
        cout << "\nC " << PIECES["wKg"];
        
        // cout << "match.board->fields " << match.board.getFields();
        
        // struct session kant;
        // neu(&kant);
        // init_words();
        // forth(&kant);

        // perform_respond(&kant);

        return 0;
    }

