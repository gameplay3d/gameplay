#include "IBehaviour.h"

#include <fstream>
#include <iostream>
using namespace std;

class Behaviour : public IBehaviour {
    public:
        void Update(float elapsedTime){
            
            static long count = 0;
            std::ofstream outfile;

            outfile.open("test.txt", std::ios_base::app);
            outfile << "Called: " << count++ << "\n"; 
            cout << "Called\n";
        }
};
