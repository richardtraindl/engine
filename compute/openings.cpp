
    #include "./openings.hpp"
    #include "../helper.hpp"
    #include <iostream>
    #include <algorithm>
    #include <iterator>


    cNode::cNode(uint64_t newfields[4], vector<string> &newcandidates){
        for(int idx = 0; idx < 4; ++idx){
            fields[idx] = newfields[idx];
        }
        for(int i = 0; i < newcandidates.size(); i++){
            candidates.push_back(newcandidates[i]); 
        }
    }

    cOpenings::cOpenings(){
    }


    void cOpenings::add_node(cNode &node, int depth){
        if(depth >= 0 && depth < cOpenings::MAXDEPTH){
            stages[depth].push_back(node);
        }
    }


    cMove *retrieve_move(cMatch &match){
        vector<string> candidates;
        if(match.minutes.size() >= cOpenings::MAXDEPTH){
            cout << "### openings: depth not supported ###" << endl;
            return NULL;
        }

        cOpenings *openings = populate_openings();
        vector<cNode> nodes = openings->stages[match.minutes.size()];
        uint64_t currfields[4];
        for(int idx = 0; idx < 4; ++idx){
            match.board.cpyfields_to_bigint((idx * 16), 16, currfields[idx]);
        }
        for(cNode node : nodes){                    
            if(node.fields[0] == currfields[0] &&
               node.fields[1] == currfields[1] &&
               node.fields[2] == currfields[2] &&
               node.fields[3] == currfields[3]){
                for(int i = 0; i < node.candidates.size(); i++){
                    candidates.push_back(node.candidates[i]); 
                }
                break;
            }
        }
        if(candidates.size() == 0){
            cout << "### openings: no opening move found ###" << endl;
            return NULL;
        }
        srand(time(0));
        int idx = (rand() % candidates.size());
        int src = coord_to_index(candidates[idx].substr(0,2));
        int dst = coord_to_index(candidates[idx].substr(3,2));
        int prev_dstpiece = match.board.getfield(dst);
        return new cMove(src, dst, prev_dstpiece, mBLK);
    }


    cOpenings *populate_openings(){
        cOpenings *openings = new cOpenings();

        //#################
        uint64_t base1[] = {{cBoard::BASE1}, {cBoard::BASE2}, {cBoard::BASE3}, {cBoard::BASE4}};
        vector<string> candidates1 = {{"e2-e4"}, {"d2-d4"}, {"c2-c4"}, {"g1-f3"}, {"g2-g3"}, {"d2-d3"}};
        cNode node1(base1, candidates1);
        openings->add_node(node1, 0);

        //e2-e4
        uint64_t base2[] = {{0X4235632411110111}, {0x0000000000001000}, {0x0000000000000000}, {0x99999999CABDEBAC}};
        vector<string> candidates2 = {{"e7-e5"}, {"c7-c5"}, {"e7-e6"}, {"g8-f6"}, {"g7-g6"}};
        cNode node2(base2, candidates2);
        openings->add_node(node2, 1);

        //d2-d4
        uint64_t base3[] = {{0x4235632411101111}, {0x0000000000010000}, {0x0000000000000000}, {0x99999999CABDEBAC}};
        vector<string> candidates3 = {{"d7-d5"}, {"f7-f5"}, {"g8-f6"}, {"g7-g6"}, {"e7-e6"}};
        cNode node3(base3, candidates3);
        openings->add_node(node3, 1);

        //c2-c4
        uint64_t base4[] = {{0x4235632411011111}, {0x0000000000100000}, {0x0000000000000000}, {0x99999999CABDEBAC}};
        vector<string> candidates4 = {{"e7-e5"}, {"c7-c5"}, {"b8-c6"}, {"g8-f6"}, {"g7-g6"}};
        cNode node4(base4, candidates4);
        openings->add_node(node4, 1);

        //g1-f3
        uint64_t base5[] = {{0x4235630411111111}, {0x0000020000000000}, {0x0000000000000000}, {0x99999999CABDEBAC}};
        vector<string> candidates5 = {{"d7-d5"}, {"g8-f6"}, {"g7-g6"}, {"c7-c5"}, {"b8-c6"}};
        cNode node5(base5, candidates5);
        openings->add_node(node5, 1);

        //g2-g3
        uint64_t base6[] = {{0x4235632411111101}, {0x0000001000000000}, {0x0000000000000000}, {0x99999999CABDEBAC}};
        vector<string> candidates6 = {{"e7-e5"}, {"d7-d5"}, {"c7-c5"}, {"g8-f6"}, {"g7-g6"}};
        cNode node6(base6, candidates6);
        openings->add_node(node6, 1);

        //d2-d3
        uint64_t base7[] = {{0x4235632411101111}, {0x0001000000000000}, {0x0000000000000000}, {0x99999999CABDEBAC}};
        vector<string> candidates7 = {{"e7-e5"}, {"d7-d5"}, {"c7-c5"}, {"g8-f6"}, {"g7-g6"}};
        cNode node7(base7, candidates7);
        openings->add_node(node7, 1);
        //#################

        //#################
        //e2-e4, e7-e5
        uint64_t base8[] = {{0x4235632411110111}, {0x0000000000001000}, {0x0000900000000000}, {0x99990999CABDEBAC}};
        vector<string> candidates8 = {{"g1-f3"}, {"b1-c3"}, {"f1-c4"}};
        cNode node8(base8, candidates8);
        openings->add_node(node8, 2);

        //e2-e4, c7-c5
        uint64_t base9[] = {{0x4235632411110111}, {0x0000000000001000}, {0x0090000000000000}, {0x99099999CABDEBAC}};
        vector<string> candidates9 = {{"g1-f3"}, {"d2-d4"}, {"f1-c4"}, {"b1-c3"}};
        cNode node9(base9, candidates9);
        openings->add_node(node9, 2);
        
        //e2-e4, e7-e6
        uint64_t base10[] = {{0x4235632411110111}, {0x0000000000001000}, {0x0000000000009000}, {0x99990999CABDEBAC}};
        vector<string> candidates10 = {{"d2-d4"}, {"g1-f3"}, {"b1-c3"}};
        cNode node10(base10, candidates10);
        openings->add_node(node10, 2);
     
        //e2-e4, d7-d6
        uint64_t base11[] = {{0x4235632411110111}, {0x0000000000001000}, {0x0000000000090000}, {0x99909999CABDEBAC}};
        vector<string> candidates11 = {{"d2-d4"}, {"g1-f3"}, {"f1-c4"}, {"b1-c3"}};
        cNode node11(base11, candidates11);
        openings->add_node(node11, 2);
        
        //d2-d4, d7-d5
        uint64_t base12[] = {{0x4235632411101111}, {0x0000000000010000}, {0x0009000000000000}, {0x99909999CABDEBAC}};
        vector<string> candidates12 = {{"c2-c4"}, {"g1-f3"}, {"c1-f4"}};
        cNode node12(base12, candidates12);
        openings->add_node(node12, 2);

        //d2-d4, f7-f5
        uint64_t base13[] = {{0x4235632411101111}, {0x0000000000010000}, {0x0000090000000000}, {0x99999099CABDEBAC}};
        vector<string> candidates13 = {{"c2-c4"}, {"g1-f3"}, {"g2-g3"}, {"e2-e4"}};
        cNode node13(base13, candidates13);
        openings->add_node(node13, 2);

        //d2-d4, d7-d6
        uint64_t base14[] = {{0x4235632411101111}, {0x0000000000010000}, {0x0000000000090000}, {0x99909999CABDEBAC}};
        vector<string> candidates14 = {{"e2-e4"}, {"c2-c4"}, {"g1-f3"}, {"c1-f4"}};
        cNode node14(base14, candidates14);
        openings->add_node(node14, 2);

        //d2-d4, e7-e6
        uint64_t base15[] = {{0x4235632411101111}, {0x0000000000010000}, {0x0000000000009000}, {0x99990999CABDEBAC}};
        vector<string> candidates15 = {{"e2-e4"}, {"c2-c4"}, {"g1-f3"}, {"c1-f4"}};
        cNode node15(base15, candidates15);
        openings->add_node(node15, 2);

        //d2-d4, g8-f6
        uint64_t base16[] = {{0x4235632411101111}, {0x0000000000010000}, {0x0000000000000A00}, {0x99999999CABDEB0C}};
        vector<string> candidates16 = {{"c2-c4"}, {"g1-f3"}, {"c1-f4"}};
        cNode node16(base16, candidates16);
        openings->add_node(node16, 2);

    /*  
        //c2-c4, e7-e5
        node = cNode(0X42356324110111110000000000100000000090000000000099990999CABDEBAC, \
                     ["b1-c3", "d2-d3", "g2-g3"])
        openings.add_node(node, 2)

        #c2-c4, c7-c5
        node = cNode(0X42356324110111110000000000100000009000000000000099099999CABDEBAC, \
                     ["b1-c3", "g1-f3", "g2-g3", "e2-e3"])
        openings.add_node(node, 2)

        //c2-c4, g8-f6
        node = cNode(0X423563241101111100000000001000000000000000000A0099999999CABDEB0C, \
                     ["d2-d4", "b1-c3", "g1-f3", "g2-g3"])
        openings.add_node(node, 2)

        //c2-c4, d7-d6
        node = cNode(0X42356324110111110000000000100000000000000009000099909999CABDEBAC, \
                     ["d2-d4", "b1-c3", "g1-f3", "g2-g3"])
        openings.add_node(node, 2)

        #c2-c4, g7-g6
        node = cNode(0X42356324110111110000000000100000000000000000009099999909CABDEBAC, \
                     ["d2-d4", "g1-f3", "b1-c3", "g2-g3"])
        openings.add_node(node, 2)

        //g1-f3, d7-d5
        node = cNode(0X42356304111111110000020000000000000900000000000099909999CABDEBAC, \
                     ["d2-d4", "d2-d3", "g2-g3"])
        openings.add_node(node, 2)

        //g1-f3, c7-c5
        node = cNode(0X42356304111111110000020000000000009000000000000099099999CABDEBAC, \
                     ["e2-e4", "c2-c4", "g2-g3", "d2-d4"])
        openings.add_node(node, 2)

        #g1-f3, g8-f6
        node = cNode(0X423563041111111100000200000000000000000000000A0099999999CABDEB0C, \
                     ["d2-d4", "c2-c4", "g2-g3"])
        openings.add_node(node, 2)

        //g1-f3, d7-d6
        node = cNode(0X42356304111111110000020000000000000000000009000099909999CABDEBAC, \
                     ["e2-e4", "d2-d4", "c2-c4", "g2-g3"])
        openings.add_node(node, 2)

        //g1-f3, e7-e6
        node = cNode(0X42356304111111110000020000000000000000000000900099990999CABDEBAC, \
                     ["e2-e4", "d2-d4", "c2-c4", "g2-g3"])
        openings.add_node(node, 2)

        //g2-g3, g7-g6
        node = cNode(0X42356324111111010000001000000000000000000000009099999909CABDEBAC, ["f1-g2"])
        openings.add_node(node, 2)

        //g2-g3, e7-e5
        node = cNode(0X42356324111111010000001000000000000090000000000099990999CABDEBAC, ["f1-g2"])
        openings.add_node(node, 2)

        //g2-g3, d7-d5
        node = cNode(0X42356324111111010000001000000000000900000000000099909999CABDEBAC, ["f1-g2"])
        openings.add_node(node, 2)

        //g2-g3, c7-c5
        node = cNode(0X42356324111111010000001000000000009000000000000099099999CABDEBAC, ["f1-g2"])
        openings.add_node(node, 2)

        //g2-g3, f8-g6
        node = cNode(0X423563241111110100000010000000000000000000000A0099999999CABDEB0C, ["f1-g2"])
        openings.add_node(node, 2)

        //d2-d3, e7-e5
        node = cNode(0X42356324111011110001000000000000000090000000000099990999CABDEBAC, \
                     ["e2-e4, c2-c4", "g1-f3"])
        openings.add_node(node, 2)

        //d2-d3, d7-d5
        node = cNode(0X42356324111011110001000000000000000900000000000099909999CABDEBAC, \
                     ["g1-f3, g2-g3", "e2-e3"])
        openings.add_node(node, 2)

        //d2-d3, c7-c5
        node = cNode(0X42356324111011110001000000000000009000000000000099099999CABDEBAC, \
                     ["e2-e4", "g1-f3"])
        openings.add_node(node, 2)

        //d2-d3, g8-f6
        node = cNode(0X423563241110111100010000000000000000000000000A0099999999CABDEB0C, \
                     ["e2-e4", "c2-c4", "g1-f3"])
        openings.add_node(node, 2)

        //d2-d3, g7-g6
        node = cNode(0X42356324111011110001000000000000000000000000009099999909CABDEBAC, \
                     ["c2-c4", "e2-e4", "g1-f3"])
        openings.add_node(node, 2)
        #################

        #################
        #d2-d4, f7-f5, e2-e4
        node = cNode(0X42356324111001110000000000011000000009000000000099999099CABDEBAC, \
                     ["g8-f6"])
        openings.add_node(node, 3)
        */

        return openings;
    }

