
#include <iostream>
#include "./match.hpp"
#include "./values.hpp"

    using namespace std;

    int main(void){
        cMatch match;
       
        cout << "fields " << hex << match.board.fields << "\n";

        cout << "movecnt() " << match.movecnt() << "\n";

        cout << "next_color() " << match.next_color() << "\n";

        cout << "is_opening() " << match.is_opening() << "\n";

        cout << "is_endgame() " << match.is_endgame() << "\n";

        cout << "is_fifty_moves_rule() " << match.is_fifty_moves_rule() << "\n";
        
        match.board.setfield(8, mBLK);
        cMove *move = match.do_move(0, 16, mBLK);
        cout << "do_move " << hex << match.board.fields << "\n";
        cout << "move " << hex << move->format() << "\n";

        //move = match.do_move(52, 44, mBLK);
        move = match.do_move(9, 17, mBLK;
        cout << "do_move " << hex << match.board.fields << "\n";
        cout << "move " << hex << move->format() << "\n";

        match.undo_move();
        cout << "undo_move " << hex << match.board.fields << "\n";

        cout << "is_king_after_move_attacked() " << match.board.is_king_after_move_attacked(11, 12, &match.minutes) << "\n";

        cout << "is_move_valid() " << match.board.is_move_valid(0, 9, mBLK, &match.minutes) << "\n";

        cout << "is_move_available() " << match.board.is_move_available(&match.minutes) << "\n";
        
        cout << "eval_status() " << match.eval_status() << "\n";

        cout << "eval_pin_dir() " << match.board.eval_pin_dir(40) << "\n";

        cout << "eval_soft_pin_dir() " << match.board.eval_soft_pin_dir(42) << "\n";
        
        return 0;
    }
