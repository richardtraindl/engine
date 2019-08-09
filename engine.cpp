
    #include <iostream>
    #include <string>
    #include "./match.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"
    #include "./compute/calc.hpp"
    #include "./ui/play.hpp"

    using namespace std;

    int main(void){
        cMatch match;
       
        /*cout << "fields " << hex << match.board.fields << "\n";

        cout << "movecnt() " << match.movecnt() << "\n";

        cout << "next_color() " << match.next_color() << "\n";

        cout << "is_opening() " << match.is_opening() << "\n";

        cout << "is_endgame() " << match.is_endgame() << "\n";

        cout << "is_fifty_moves_rule() " << match.is_fifty_moves_rule() << "\n";
        
        
        cout << "before do_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";

        cMove *move;
        move = match.do_move(12, 28, mBLK);
        cout << "1st do_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";
        cout << "move " << hex << move->format() << "\n";
        
        move = match.do_move(48, 40, mBLK);
        cout << "2nd do_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";
        cout << "move " << hex << move->format() << "\n";

        move = match.do_move(11, 27, mBLK);
        cout << "3rd do_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";
        cout << "move " << hex << move->format() << "\n"; 

        move = match.do_move(48, 40, mBLK);
        cout << "4nd do_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";
        cout << "move " << hex << move->format() << "\n";

        match.undo_move();
        cout << "undo_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";

        move = match.do_move(57, 40, mBLK);
        cout << "5nd do_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";
        cout << "move " << hex << move->format() << "\n";

        match.undo_move();
        cout << "undo_move " << dec << match.score << " \n" << hex << match.board.fields << "\n";

        cout << "is_king_after_move_attacked() " << match.board.is_king_after_move_attacked(11, 12, match.minutes) << "\n";

        cout << "is_move_valid() " << match.board.is_move_valid(0, 9, mBLK, match.minutes) << "\n";

        cout << "is_move_available() " << match.board.is_move_available(match.minutes) << "\n";
        
        cout << "eval_status() " << match.eval_status() << "\n";

        cout << "eval_pin_dir() " << match.board.eval_pin_dir(40) << "\n";

        cout << "eval_soft_pin_dir() " << match.board.eval_soft_pin_dir(42) << "\n";
        
        match.board.prnt();
        cout << "minutes size " << match.minutes.size() << endl;

        list<cMove> moves;        
        generate_moves(match, moves);

        cout << "size " << moves.size() << endl;
        for(list<cMove>::iterator it = moves.begin(); it != moves.end(); ++it){
            cout << it->format() << endl; // << " prio: " << it->prio 
        }

        list<cPrioMove> priomoves;
        generate_priomoves(match, NULL, NULL, true, priomoves);

        cout << "size " << priomoves.size() << endl;
        for(list<cPrioMove>::iterator it = priomoves.begin(); it != priomoves.end(); ++it){
            cout << it->format() << " prio: " << dec << it->prio << endl;;
        }

        match.board.prnt();
        cout << dec << match.score << endl;
        list<cPrioMove> candidates;
        int score = calc_move(match, NULL, candidates);
        cout << "score: " << dec << score << " match.score: " << match.score << endl; */

        play(2);

        return 0;
    }
