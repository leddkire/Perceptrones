#include <cmath>
#include <fstream>
#include "UnidadSigmoidal.cpp"
using namespace std;

int main(){

	ifstream infile("berzek.data");
	for(string line; getline(source, line);)   
	{
	    istringstream in(line);

	    if(type == "")
	    {
	        float x, y, z, t;
	        in >> x >> y >> z >> t;
	    }

	    string type;
	    in >> type;

	}

}