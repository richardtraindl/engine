
    #include <sstream>
    #include "./helper.hpp"

    using namespace std;

    int coord_to_index(string coord){
        int x = stoi(coord.substr(0, 1), nullptr, 10);
        int y = stoi(coord.substr(1, 1), nullptr, 10);
        if(x > (int)'H'){
            x -= (int)'a';
        }
        else{
            x -= (int)'A';
        }
        y -= (int)'1';
        return x + y * 8;
    };

    string index_to_coord(int idx){
        ostringstream outstream;
        outstream << (char)(idx % 8 + (int)'a' ) << (char)(idx / 8 + (int)'1');
        return outstream.str();
    };

