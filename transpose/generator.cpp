#include <iostream>
#include <array>
#include <ctime>
#include <random>
#include <chrono>
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

class DimensionsHaveToBeNonZeroPositiveIntegersException{};

void Generate2DArray(int n){
    if(n <= 0){
        throw DimensionsHaveToBeNonZeroPositiveIntegersException{};
    }

    ofstream outFile ("inputMatrix.txt");
    outFile<<n<<" ";

    default_random_engine generator(chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0,n);
    distribution(generator);

    for(auto i = 0; i < n; i++){
        for(auto j = 0; j < n; j++)
        {
            outFile<<distribution(generator)<<" ";
        }
    }
    outFile.close();
}

int main(int argc, const char *argv[]){
	int size = atoi(argv[1]);
    	Generate2DArray(size);
   	return 0;
}
