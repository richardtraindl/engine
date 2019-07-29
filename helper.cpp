
    #include <sstream>
    #include "./helper.hpp"

    using namespace std;


    int coord_to_index(string coord){
        int x = int(coord[0]);
        int y = int(coord[1]);
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


    string reverse_lookup(const map<string, int>& dict, int value){
        for(auto &x: dict){
            if(x.second == value){
                return x.first;
            }
        }
        return "";
  }

