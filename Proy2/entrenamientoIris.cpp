#include <cmath>
#include <fstream>
#include "UnidadSigmoidal.cpp"
using namespace std;

int main(){
	int* esperados;
	ifstream infile("dat.data");
	for(string line; getline(infile, line);)   
	{
	    istringstream in(line);
	    float x, y, z, t;
	    in >> x >> y >> z >> t;

	    string type;
	    in >> type;

	    if(type == "Iris-setosa"){

	    } else if(type == "Iris-versicolor") {

	    } else {

	    }

	}

}