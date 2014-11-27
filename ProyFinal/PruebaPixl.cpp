#include <Magick++.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
using namespace std;
using namespace Magick;

int main(){

	Image my_image;
	//Pixels pix;
	int intense;
	vector<string> names;
int width = 25;
int height = 19;
	try { 
		//Read a file into image object 
		vector<int> red;
		vector<int> green;
		vector<int> blue;
		vector<double> shade;
		double shades[300][width*height];
		double sh=0.0;
		ofstream pixel_input, pixel_test;
		pixel_input.open("pixel_input.txt");
		pixel_test.open("pixel_test.txt");
		ifstream file;
		int numPrueba = 0;
		string clasificaciones[4] = {"happy", "angry", "surprised", "sad"};
		for(int fnum=0; fnum < 4; fnum++){
			stringstream ss;
			ss << "scaledFaces/" << clasificaciones[fnum] << "_face.txt";
			const char* s = ss.str().c_str();
			file.open(s);
			string line;
			numPrueba = 0;

			while(getline(file, line)){
				my_image.read("scaledFaces/"+line);
				//cout << "OH SHIT WHILE\n";
				//pixel_input << "face ";
				for(int j=0;j<width;j++)
		        {
					for(int i=0;i<height;i++)
			        {

			            ColorGray gray(my_image.pixelColor(i,j));
			            sh = gray.shade();
			            if(numPrueba < 15){
				            pixel_input << gray.shade() << " ";
				        } else {
				        	pixel_test << gray.shade() << " ";
				        }
			        }
			    }
			    if(numPrueba < 15){
				    pixel_input << clasificaciones[fnum] <<"\n";
				} else {
					pixel_test << clasificaciones[fnum] <<"\n";
				}
			    numPrueba++;
			}
		    
		    file.close();
		}
		pixel_test.close();
		pixel_input.close();

	} 
	catch( Exception &error_ ) 
    { 
    	cout << "Excepcion fea: " << error_.what() << endl; 
    	return 1; 
    } 
}
