
#include <iostream>
#include "./match.hpp"
#include "./values.hpp"

    using namespace std;

    int main(void){
        cMatch match;
        cout << "\nF " << match.STATUS["active"];
        cout << "\nD " << match.board.wKg;
        cout << "\nC " << PIECES["wKg"];
        
        match.is_fifty_moves_rule();
        
        cout << "\nGGFG " << hex << match.board.getfield(63);
        cout << "\nGGFG " << hex << match.board.fields;
        
        match.board.setfield(7, PIECES["bRk"]);
        cout << "\nGGFG " << hex << match.board.fields;
        // cout << "match.board->fields " << match.board.getFields();
        
        // struct session kant;
        // neu(&kant);
        // init_words();
        // forth(&kant);

        // perform_respond(&kant);

        return 0;
    }

