

    #include <iostream>
    #include <iomanip> 
    #include "./debug.hpp"


    uint32_t g_log_traces[30][10];


    void init_tracer(){
        
        for(uint8_t depth = 0; depth < 30; depth++){

            for(uint8_t i = 0; i < 10; i++){

                  g_log_traces[depth][i] = 0;
            }

        }

    }


    void prnt_tracer(){
      
        for(uint8_t depth = 0; depth < 30; depth++){

            for(uint8_t i = 0; i < 10; ++i){

                  cout << to_string(g_log_traces[depth][i]) << setw(8) << setfill(' ');

            }

            cout << endl;

        }

        cout << "Längssummen" << endl;

        for(uint8_t i = 0; i < 10; ++i){

            uint32_t sum = 0;

            for(uint8_t depth = 0; depth < 30; depth++){

                sum += g_log_traces[depth][i];

            }

            cout << to_string(sum) << setw(8) << setfill(' ') << endl;

        }

      
    }


    void incr_tracer(const uint8_t depth, const uint8_t count){
      
        if((depth - 1) < 0 || (depth - 1) > 29){
            return;
        }
      
        if((count - 1) < 0 || (count - 1) > 9){
            return;
        }
      
        g_log_traces[(depth - 1)][(count - 1)]++;

    }

