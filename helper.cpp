
#include <sstream>

using namespace std;

    int *coord_to_index(string coord){
        int index[2];
        int x = stoi(coord.substr(0, 1), nullptr, 10);
        int y = stoi(coord.substr(1, 1), nullptr, 10);
        if(x > (int)'H'){
            index[0] = x - (int)'a';
        }
        else{
            index[0] = x - (int)'A';
        }
        index[1] = y - (int)'1';
        return index;
    };

    string index_to_coord(int x, int y){
        string coord;
        stringstream out;
        out << (x + (int)'a') << (y + (int)'1');
        coord = out.str();
        return coord;
    };

