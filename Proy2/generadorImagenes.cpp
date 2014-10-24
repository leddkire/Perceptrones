 #include <mgl2/mgl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;
int sample(mglGraph *gr)
{
  gr->Rotate(60,40);
  gr->Box();
  return 0;
}


int main(int ,char **)
{
	ifstream entrada;
	
	  
	string strang;
	int lineas =0;
	int i =0;
	int j =0;
	int top = 11;
	int numConjuntos = 6;
	double* arreglo;
	arreglo = new double[100];
	double valor = 0;
	int n[3] = {250,500,1000};

	string base_error = "/errores_conjunto_";
	string base_dir = "./ejercicio2_";
	string base_res = "/resultados_conjunto_";

	for(int i =2; i < top; i++){
		for(int j =0; j <numConjuntos; j++){
			mglGraph gr;
			stringstream nombreArch;
			nombreArch << base_dir << i << base_error << j;
			string nombre = nombreArch.str();
			mglData dat(nombre.data());
			mglData x = dat.SubData(0);
			gr.Alpha(true);   gr.Light(true);
			gr.Box();
			gr.SetSize(1000,1000);
			gr.SetOrigin(0,0); gr.SetRange('x',0,10000); gr.SetRange('y',0,n[j%3]);
			gr.Plot(x);
		  	gr.Axis(); gr.Label('x',"x",0); gr.Axis();gr.Label('y',"y",0);
		  	nombreArch << "_imagen";
			nombre = nombreArch.str();
			gr.WritePNG(nombre.data());
		}

		for(int j=0; j < numConjuntos; j++){
			mglGraph gr;
			stringstream nombreArch;
			nombreArch << base_dir << i << base_res << j;
			string nombre = nombreArch.str();
			mglData dat(nombre.data());
			mglData x = dat.SubData(0);
			mglData y = dat.SubData(1);
			stringstream nombreArchAfuera;
			nombreArchAfuera << base_dir << i << base_res << j << "Afuera";
			string nombreAfuera = nombreArchAfuera.str();
			mglData dat(nombreAfuera.data());
			mglData xA = dat.SubData(0);
			mglData yA = dat.SubData(1);
			gr.Alpha(true);   gr.Light(true);
			gr.Box();
			gr.SetSize(1000,1000);
			gr.SetOrigin(NAN,NAN); gr.SetRange('x',0,100); gr.SetRange('y',0,100);
			gr.Plot(x,y," .");
			gr.Plot(xA,yA,"R.");
		  	gr.Axis(); gr.Label('x',"x",0); gr.Axis();gr.Label('y',"y",0);
		  	nombreArch << "_imagen";
			nombre = nombreArch.str();
			gr.WritePNG(nombre.data());
		}

	}

	base_error = "/errores_binario_porcentaje";
	base_dir = "./ejercicio3_";
	base_res = "/resultados_conjunto_";
	numConjuntos = 5;
	for(int i =4; i < top; i++){
		for(int j =0; j <numConjuntos; j++){
			mglGraph gr;
			stringstream nombreArch;
			nombreArch << base_dir << i << base_error << 50+(10*j);
			string nombre = nombreArch.str();
			mglData dat(nombre.data());
			mglData x = dat.SubData(0);
			gr.Alpha(true);   gr.Light(true);
			gr.Box();
			gr.SetSize(1000,1000);
			gr.SetOrigin(NAN,NAN);
			gr.Plot(x);
		  	gr.Axis(); gr.Label('x',"x",0); gr.Axis();gr.Label('y',"y",0);
		  	nombreArch << "_imagen";
			nombre = nombreArch.str();
			gr.WritePNG(nombre.data());
		}
		cout << i << "\n";
	}
	/*mglData dat("./ejercicio2_10/resultados_conjunto_5");
	mglData x = dat.SubData(0);
	mglData y = dat.SubData(1);
	*/

	/*while(getline(entrada,strang)){
		stringstream temp(strang);
		temp >> x;
		temp >> y;
		temp >> valor;
		if(valor <0.5){
			arreglo[int(y*100)]=x*100;
			cout << x*100 << ", " << y*100 << "\n";
		}
		
		if(lineas == 100){
			datX.Set(arreglo,100);
			gr.Plot(datX);
			lineas = 0;
		}
		lineas++;
		
	}*/
		
	
	return 0;
}
