
    #include "./helper.hpp"


    uint64_t coord_to_pos(string coord){
        int x = int(coord[0]);
        int y = int(coord[1]);
        if(x > (int)'H'){
            x -= (int)'a';
        }
        else{
            x -= (int)'A';
        }
        y -= (int)'1';
        return (0x8000000000000000 >> (x + (y * 8)));
    };


    string pos_to_coord(uint64_t pos){
        ostringstream outstream;
        uint64_t xmask = 0x8080808080808080;
        uint64_t ymask = 0xFF00000000000000;
        uint8_t x = (int)'a';
        uint8_t y = (int)'1';

        while((pos & xmask) == 0){
            x += 1;
            xmask = (xmask >> 1);
        }

        while((pos & ymask) == 0){
            y += 1;
            ymask = (ymask >> 8);
        }

        outstream << (char)x << (char)y;
        return outstream.str();
    };


    string reverse_lookup(const map<string, uint8_t> &dict, uint8_t value){
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


    void prnt_16hex(uint64_t pos){
        cout << "0x" << hex << setfill('0') << setw(16);
        cout << pos << endl;
    }
