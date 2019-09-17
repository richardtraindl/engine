
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
        match.board.prnt();
        match.do_move(coord_to_index("e2"), coord_to_index("e4"), mBLK);
        match.do_move(coord_to_index("e7"), coord_to_index("e5"), mBLK);
        match.do_move(coord_to_index("g1"), coord_to_index("f3"), mBLK);
        match.do_move(coord_to_index("d7"), coord_to_index("d6"), mBLK);
        match.do_move(coord_to_index("d2"), coord_to_index("d4"), mBLK);
        match.do_move(coord_to_index("c8"), coord_to_index("g4"), mBLK);
        match.do_move(coord_to_index("d4"), coord_to_index("e5"), mBLK);
        match.do_move(coord_to_index("g4"), coord_to_index("f3"), mBLK);
        match.do_move(coord_to_index("d1"), coord_to_index("f3"), mBLK);
        match.do_move(coord_to_index("d6"), coord_to_index("e5"), mBLK);
        match.do_move(coord_to_index("f1"), coord_to_index("c4"), mBLK);
        match.do_move(coord_to_index("g8"), coord_to_index("f6"), mBLK);
        match.do_move(coord_to_index("f3"), coord_to_index("b3"), mBLK);
        match.do_move(coord_to_index("d8"), coord_to_index("e7"), mBLK);
        match.do_move(coord_to_index("c1"), coord_to_index("g5"), mBLK);
        match.do_move(coord_to_index("c7"), coord_to_index("c6"), mBLK);
        match.do_move(coord_to_index("b1"), coord_to_index("c3"), mBLK);
        match.do_move(coord_to_index("b7"), coord_to_index("b5"), mBLK);
        match.do_move(coord_to_index("c3"), coord_to_index("b5"), mBLK);
        match.do_move(coord_to_index("c6"), coord_to_index("b5"), mBLK);
        match.do_move(coord_to_index("c4"), coord_to_index("b5"), mBLK);
        match.do_move(coord_to_index("b8"), coord_to_index("d7"), mBLK);

        match.do_move(coord_to_index("e1"), coord_to_index("c1"), mBLK);
        match.do_move(coord_to_index("a8"), coord_to_index("d8"), mBLK);
        match.do_move(coord_to_index("d1"), coord_to_index("d7"), mBLK);
        match.do_move(coord_to_index("d8"), coord_to_index("d7"), mBLK);
        //match.do_move(coord_to_index("h1"), coord_to_index("d1"), mBLK);
        //match.do_move(coord_to_index("e7"), coord_to_index("e6"), mBLK);

        /*match.do_move(coord_to_index("b3"), coord_to_index("e6"), mBLK);
        match.do_move(coord_to_index("f7"), coord_to_index("e6"), mBLK);
        match.do_move(coord_to_index("g5"), coord_to_index("f6"), mBLK);
        match.do_move(coord_to_index("g7"), coord_to_index("f6"), mBLK);
        match.do_move(coord_to_index("d1"), coord_to_index("d7"), mBLK);
        match.do_move(coord_to_index("f8"), coord_to_index("h6"), mBLK);
        match.do_move(coord_to_index("c1"), coord_to_index("b1"), mBLK);
        match.do_move(coord_to_index("e8"), coord_to_index("g8"), mBLK);*/
        
        //match.do_move(coord_to_index("d7"), coord_to_index("d6"), mBLK);


        //[c3xb5]  [c6xb5]  [c4-d5]  [f6xd5]  [g5xe7]  [d5xe7]  [b3xb5]
        // match.do_move(coord_to_index("b8"), coord_to_index("d7"), mBLK);
        /*
        match.do_move(coord_to_index("a2"), coord_to_index("a4"), mBLK);
        match.do_move(coord_to_index("b5"), coord_to_index("c4"), mBLK);
        match.do_move(coord_to_index("a4"), coord_to_index("a5"), mBLK);
        match.do_move(coord_to_index("c4"), coord_to_index("b3"), mBLK);
        match.do_move(coord_to_index("a5"), coord_to_index("a6"), mBLK);
        match.do_move(coord_to_index("b3"), coord_to_index("c2"), mBLK);
        match.do_move(coord_to_index("b2"), coord_to_index("b4"), mBLK);
        */

        bool maximizing;
        if(match.next_color() == COLORS["white"]){
            maximizing = true;
        }
        else{
            maximizing = false;
        }
        play(match, maximizing);

        return 0;
    }
