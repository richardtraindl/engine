
    #include <iostream>
    #include <string>
    #include "./match.hpp"
    #include "./board.hpp"
    #include "./helper.hpp"
    #include "./values.hpp"
    //#include "./compute/calc.hpp"
    //#include "./ui/play.hpp"

    using namespace std;

    int main(void){
        cMatch match;

        match.do_move(coord_to_pos("e2"),coord_to_pos("e4"), mBLK);
        match.do_move(coord_to_pos("e7"), coord_to_pos("e5"), mBLK);
        match.do_move(coord_to_pos("g1"), coord_to_pos("f3"), mBLK);
        match.do_move(coord_to_pos("d7"), coord_to_pos("d6"), mBLK);
        match.do_move(coord_to_pos("d2"), coord_to_pos("d4"), mBLK);
        match.do_move(coord_to_pos("c8"), coord_to_pos("g4"), mBLK);
        match.do_move(coord_to_pos("d4"), coord_to_pos("e5"), mBLK);
        match.do_move(coord_to_pos("g4"), coord_to_pos("f3"), mBLK);
        match.do_move(coord_to_pos("d1"), coord_to_pos("f3"), mBLK);
        match.do_move(coord_to_pos("d6"), coord_to_pos("e5"), mBLK);
        match.do_move(coord_to_pos("f1"), coord_to_pos("c4"), mBLK);
        match.do_move(coord_to_pos("g8"), coord_to_pos("f6"), mBLK);
        match.do_move(coord_to_pos("f3"), coord_to_pos("b3"), mBLK);
        match.do_move(coord_to_pos("d8"), coord_to_pos("e7"), mBLK);
        match.do_move(coord_to_pos("c1"), coord_to_pos("g5"), mBLK);
        match.do_move(coord_to_pos("c7"), coord_to_pos("c6"), mBLK);
        match.do_move(coord_to_pos("b1"), coord_to_pos("c3"), mBLK);
        match.do_move(coord_to_pos("b7"), coord_to_pos("b5"), mBLK);
        match.do_move(coord_to_pos("c3"), coord_to_pos("b5"), mBLK);
        match.do_move(coord_to_pos("c6"), coord_to_pos("b5"), mBLK);
        match.do_move(coord_to_pos("c4"), coord_to_pos("b5"), mBLK);
        match.do_move(coord_to_pos("b8"), coord_to_pos("d7"), mBLK);


        /*
        bool maximizing;
        if(match.next_color() == COLORS["white"]){
            maximizing = true;
        }
        else{
            maximizing = false;
        }
        play(match, maximizing);
        */

        //cBoard board;
        //board.prnt();
        
        match.board.prnt();

        match.board.gen_moves(match.minutes);

        return 0;
    }
