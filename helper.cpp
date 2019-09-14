
    #include <sstream>
    #include <iostream>
    #include <iomanip>
    #include "./helper.hpp"


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


    string reverse_lookup(const map<string, int> &dict, int value){
        for(auto &x: dict){
            if(x.second == value){
                return x.first;
            }
        }
        return "";
  }

    void prnt_fmttime(string msg, int seconds){
        int sec, minute, hour;
        hour = seconds / (60 * 60);
        int remaining = seconds % (60 * 60);
        minute = remaining / 60;
        sec = remaining % 60;
        cout << msg << setw(2) << setfill('0') << hour << ":";
        cout << setw(2) << setfill('0') <<  minute << ":";
        cout << setw(2) << setfill('0') << sec << endl;
    }

