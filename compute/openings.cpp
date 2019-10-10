
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

        //c2-c4, e7-e5
        uint64_t base17[] = {{0x4235632411011111}, {0x0000000000100000}, {0x0000900000000000}, {0x99990999CABDEBAC}};
        vector<string> candidates17 = {{"b1-c3"}, {"d2-d3"}, {"g2-g3"}};
        cNode node17(base17, candidates17);
        openings->add_node(node17, 2);

        //c2-c4, c7-c5
        uint64_t base18[] = {{0x4235632411011111}, {0x0000000000100000}, {0x0090000000000000}, {0x99099999CABDEBAC}};
        vector<string> candidates18 = {{"b1-c3"}, {"g1-f3"}, {"g2-g3"}, {"e2-e3"}};
        cNode node18(base18, candidates18);
        openings->add_node(node18, 2);

        //c2-c4, g8-f6
        uint64_t base19[] = {{0x4235632411011111}, {0x0000000000100000}, {0x0000000000000A00}, {0x99999999CABDEB0C}};
        vector<string> candidates19 = {{"d2-d4"}, {"b1-c3"}, {"g1-f3"}, {"g2-g3"}};
        cNode node19(base19, candidates19);
        openings->add_node(node19, 2);

        //c2-c4, d7-d6
        uint64_t base20[] = {{0x4235632411011111}, {0x0000000000100000}, {0x0000000000090000}, {0x99909999CABDEBAC}};
        vector<string> candidates20 = {{"d2-d4"}, {"b1-c3"}, {"g1-f3"}, {"g2-g3"}};
        cNode node20(base20, candidates20);
        openings->add_node(node20, 2);

        //c2-c4, g7-g6
        uint64_t base21[] = {{0x4235632411011111}, {0x0000000000100000}, {0x0000000000000090}, {0x99999909CABDEBAC}};
        vector<string> candidates21 = {{"d2-d4"}, {"g1-f3"}, {"b1-c3"}, {"g2-g3"}};
        cNode node21(base21, candidates21);
        openings->add_node(node21, 2);

        uint64_t base22[] = {{0x4235630411111111}, {0x0000020000000000}, {0x0009000000000000}, {0x99909999CABDEBAC}};
        vector<string> candidates22 = {{"d2-d4"}, {"d2-d3"}, {"g2-g3"}};
        cNode node22(base22, candidates22);
        openings->add_node(node22, 2);

        //g1-f3, c7-c5
        uint64_t base23[] = {{0x4235630411111111},{0x00000200000000000}, {0x0900000000000009}, {0x9099999CABDEBAC}};
        vector<string> candidates23 = {{"e2-e4"}, {"c2-c4"}, {"g2-g3"}, {"d2-d4"}};
        cNode node23(base23, candidates23);
        openings->add_node(node23, 2);

        //g1-f3, g8-f6
        uint64_t base24[] = {{0x4235630411111111}, {0x0000020000000000}, {0x0000000000000A00}, {0x99999999CABDEB0C}};
        vector<string> candidates24 = {{"d2-d4"}, {"c2-c4"}, {"g2-g3"}};
        cNode node24(base24, candidates24);
        openings->add_node(node24, 2);

        //g1-f3, d7-d6
        uint64_t base25[] = {{0x4235630411111111}, {0x0000020000000000}, {0x0000000000090000}, {0x99909999CABDEBAC}};
        vector<string> candidates25 = {{"e2-e4"}, {"d2-d4"}, {"c2-c4"}, {"g2-g3"}};
        cNode node25(base25, candidates25);
        openings->add_node(node25, 2);

        //g1-f3, e7-e6
        uint64_t base26[] = {{0x4235630411111111}, {0x0000020000000000}, {0x0000000000009000}, {0x99990999CABDEBAC}};
        vector<string> candidates26 = {{"e2-e4"}, {"d2-d4"}, {"c2-c4"}, {"g2-g3"}};
        cNode node26(base26, candidates26);
        openings->add_node(node26, 2);

        //g2-g3, g7-g6
        uint64_t base27[] = {{0x4235632411111101}, {0x0000001000000000}, {0x0000000000000090}, {0x99999909CABDEBAC}};
        vector<string> candidates27 = {{"f1-g2"}};
        cNode node27(base27, candidates27);
        openings->add_node(node27, 2);

        //g2-g3, e7-e5
        uint64_t base28[] = {{0x4235632411111101}, {0x0000001000000000}, {0x0000900000000000}, {0x99990999CABDEBAC}};
        vector<string> candidates28 = {{"f1-g2"}};
        cNode node28(base28, candidates28);
        openings->add_node(node28, 2);

        //g2-g3, d7-d5
        uint64_t base29[] = {{0x4235632411111101}, {0x00000010000000000}, {0x0090000000000009}, {0x9909999CABDEBAC}};
        vector<string> candidates29 = {{"f1-g2"}};
        cNode node29(base29, candidates29);
        openings->add_node(node29, 2);

                                
        //g2-g3, c7-c5
        uint64_t base30[] = {{0x4235632411111101}, {0x00000010000000000}, {0x0900000000000009}, {0x9099999CABDEBAC}};
        vector<string> candidates30 = {{"f1-g2"}};
        cNode node30(base30, candidates30);
        openings->add_node(node30, 2);

        //g2-g3, f8-g6
        uint64_t base31[] = {{0x4235632411111101}, {0x0000001000000000}, {0x0000000000000A00}, {0x99999999CABDEB0C}};
        vector<string> candidates31 = {{"f1-g2"}};
        cNode node31(base31, candidates31);
        openings->add_node(node31, 2);

        //d2-d3, e7-e5
        uint64_t base32[] = {{0x4235632411101111}, {0x0001000000000000}, {0x0000900000000000}, {0x99990999CABDEBAC}};
        vector<string> candidates32 = {{"e2-e4"}, {"c2-c4"}, {"g1-f3"}};
        cNode node32(base32, candidates32);
        openings->add_node(node32, 2);

        //d2-d3, d7-d5
        uint64_t base33[] = {{0x4235632411101111}, {0x0001000000000000}, {0x0009000000000000}, {0x99909999CABDEBAC}};
        vector<string> candidates33 = {{"g1-f3"}, {"g2-g3"}, {"e2-e3"}};
        cNode node33(base33, candidates33);
        openings->add_node(node33, 2);

        //d2-d3, c7-c5
        uint64_t base34[] = {{0x4235632411101111}, {0x0001000000000000}, {0x0090000000000000}, {0x99099999CABDEBAC}};
        vector<string> candidates34 = {{"e2-e4"}, {"g1-f3"}};
        cNode node34(base34, candidates34);
        openings->add_node(node34, 2);

        //d2-d3, g8-f6
        uint64_t base35[] = {{0x4235632411101111}, {0x0001000000000000}, {0x0000000000000A00}, {0x99999999CABDEB0C}};
        vector<string> candidates35 = {{"e2-e4"}, {"c2-c4"}, {"g1-f3"}};
        cNode node35(base35, candidates35);
        openings->add_node(node35, 2);

        //d2-d3, g7-g6
        uint64_t base36[] = {{0x4235632411101111}, {0x0001000000000000}, {0x0000000000000090}, {0x99999909CABDEBAC}};
        vector<string> candidates36 = {{"c2-c4"}, {"e2-e4"}, {"g1-f3"}};
        cNode node36(base36, candidates36);
        openings->add_node(node36, 2);
        //#################

        //#################
        //d2-d4, f7-f5, e2-e4
        uint64_t base37[] = {{0x4235632411100111}, {0x0000000000011000}, {0x0000090000000000}, {0x99999099CABDEBAC}};
        vector<string> candidates37 = {{"g8-f6"}};
        cNode node37(base37, candidates37);
        openings->add_node(node37, 2);
        //#################

        return openings;
    }

