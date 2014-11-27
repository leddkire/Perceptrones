#include <Magick++.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
using namespace Magick;

int main(){

	Image my_image;
	//Pixels pix;
	int intense;
	try { 
		//Read a file into image object 
	    my_image.read("glickman_left_angry_open_2.pgm");
	    //pix = my_image[0][0];
	   // PixelPacket *pixel_cache = my_image.getPixels(20,30,10,10);
	    //cout << pixel_cache->i;
	    /*unsigned char *pixels = new unsigned char[64*60];
	    int *pixelsint = new int[64*60];
	    //int val;
		my_image.write( 0, 0, 64, 60, "R", IntegerPixel, pixelsint);
		cout << "R: ";
		for(int i=0; i < 64*60; i++){
			cout << pixelsint[i] << ",";
		}*/
		vector<int> red;
		vector<int> green;
		vector<int> blue;
		vector<double> shade;

		for(int i=0;i<60;i++)
        {
            ColorGray gray(my_image.pixelColor(0,i));
            shade.push_back(gray.shade());
            //green.push_back(greyS.green());
            //blue.push_back(greyS.blue());
        }
		cout << "R: ";
		for(int i=0; i < 60; i++){
			cout << shade[i] << ",";
		}

	    //intense = pixel_cache.intensity();

	    // Crop the image to specified size (width, height, xOffset, yOffset)
	    //my_image.crop( Geometry(100,100, 100, 100) );

	    // Write the image to a file 
	    //my_image.write( "x.pgm" ); 
	} 
	catch( Exception &error_ ) 
    { 
    	cout << "Excepcion fea: " << error_.what() << endl; 
    	return 1; 
    } 
}
