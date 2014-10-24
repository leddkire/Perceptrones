#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <sys/stat.h>
#include <sys/types.h>
#include <list>
#include <typeinfo>
#include "UnidadSigmoidal.cpp"
using namespace std;

void entrenar(double entradas[][3], int tam, int numEntradas, int numSalidas, int numCapas, Capa* red, double eta, double* coordenadas, double* test){
	
	double error_global=1;
	int iteraciones_maximas=1000;
	int iteracion = 0;
	stringstream nombreArchivo;
	nombreArchivo << "./" << red[0].numNeuronas << "/" << "errores_generados_" << tam;
	ofstream errores;
	errores.open(nombreArchivo.str());

	do{
		iteracion++;
		error_global =0;
		for(int i =0; i < tam; i++){
			for(int j = 0 ; j < numEntradas; j++){
				entradas[i][j] = coordenadas[j];
			}
			for(int j = 0; j <numSalidas; j++){
				entradas[i][numEntradas + j] = test[j];
			}
			error_global += backpropagation(coordenadas, numEntradas, numCapas, red,test, eta);

		}	
		errores << error_global << "\n";
	}while(error_global!=0 && iteracion < iteraciones_maximas );
	for(int j = 0; j < numCapas; j++){
		for(int k = 0; k < red[j].numNeuronas; k++){
			red[j].neuronas[k].resetPesos();
		}
	}
	errores.close();

}

int main(){
	int numEntradas = 4;
	int numCapas = 2;
	int numSalidas = 1;
	double eta = 0.1;
	list<double*> valores;
	list<int*> class_esperada;
	list<double> setosa_or_not;
	string datos[] = {"iris50.txt", "iris60.txt", "iris70.txt", "iris80.txt", "iris90.txt"};

	for(int h=0;h<5;h++){
		valores.clear();
		class_esperada.clear();
		setosa_or_not.clear();
		cout <<"*********************Vamos con " << datos[h] << "********************************\n";
		stringstream nombreDir;
		//nombreDir << numeroNeuronasInter;
		string temp = datos[h]+"-File";
		const char* nDir = temp.c_str();
		mkdir(nDir,S_IRWXU);

		ifstream infile(datos[h]);
		UnidadSigmoidal* salida = new UnidadSigmoidal[numSalidas];
		int numLineas = 0;
		for(string line; getline(infile, line);)   
		{
		    istringstream in(line);
		    double x, y, z, t;
		    string type;
		    char ch;
		    //int esp2[3];
		    in >> x  >> ch >> y >> ch >> z >> ch >> t >> ch >> type;

		    //cout << "Los valores" << x << ", " << y << ", " << z << ", " << t << ", " << type;
		    //in >> type;
		    double esp[] = {x,y,z,t};
		    valores.push_back(esp);
		    
		    //cout << (*valores.begin())[0];

		    if(type == "Iris-setosa"){
		    	int esp2[] = {1,0,0};
		    	class_esperada.push_back(esp2);
		    	setosa_or_not.push_back(1.00);
		    } else if(type == "Iris-versicolor") {
		    	int esp2[] = {0,1,0};
		    	setosa_or_not.push_back(0.00);
		    	class_esperada.push_back(esp2);
		    } else {
		    	int esp2[] = {0,0,1};
		    	class_esperada.push_back(esp2);
		    	setosa_or_not.push_back(0.00);
		    }
		    numLineas = numLineas+1;
		}

		double* test = new double[numSalidas];


		for(int numeroNeuronasInter = 4; numeroNeuronasInter <=10; numeroNeuronasInter++){
		    	cout <<"Vamos con " << numeroNeuronasInter << "\n";
		    	UnidadSigmoidal* intermedia = new UnidadSigmoidal[numeroNeuronasInter];
		    	stringstream nombreDir;
		    	//datos[h]+"-File"
				nombreDir << "./" << datos[h] << "-File/" << numeroNeuronasInter;
				string temp = nombreDir.str()+"-Iris";
				const char* nDir = temp.c_str();
				mkdir(nDir,S_IRWXU);
				//Inicializacion de los pesos de cada capa.
				for(int i = 0; i < numeroNeuronasInter; i++){
					intermedia[i] =  UnidadSigmoidal(numEntradas+1);
				}
				for(int i = 0; i < numSalidas; i++){
					salida[i] =  UnidadSigmoidal(numeroNeuronasInter+1);

				}

				//Inicializacion de la red. Ya se sabe que la salida solo tendra una neurona y que solo habra una capa intermedia.
				Capa* red = new Capa[2];
				red[0] = Capa(intermedia,numeroNeuronasInter);
				red[1] = Capa(salida,1);

				cout << "Entrenando con " << numeroNeuronasInter << " neuronas en la capa intermedia \n";
				//for(int i =0; i < 3; i++){
					
					string linea;
					double error;
					//Reiniciamos los pesos a sus valores originales antes de cada entrenamiento
					for(int j = 0; j < numCapas; j++){
						for(int k = 0; k < red[j].numNeuronas; k++){
							red[j].neuronas[k].resetPesos();
						}
					}

					stringstream nombreArchivo;
					nombreArchivo << "./" << datos[h] << "-File/" << numeroNeuronasInter << "-Iris/" << "errores_conjunto_" << 1;
					ofstream errores;
					errores.open(nombreArchivo.str());
					

					
					double error_global=1;
					double erro = 0;
					int iteraciones_maximas=1000;
					int iteracion = 0;
					int m=0;
					do{
					iteracion++;
					error_global =0;
						/*while(getline(entradas[0],linea)){
							stringstream entrada(linea);
							for(int j = 0 ; j < numEntradas; j++){
								entrada >> coordenadas[j];
							}
							for(int j = 0; j <numSalidas; j++){
								entrada >> test[j];
							}*/

						list<double>::iterator itest = setosa_or_not.begin();
						for (list<double*>::iterator it = valores.begin(); it != valores.end(); it++){
							//test[0] = *itest;
							double testi[] = {*itest};
							//cout << *itest;
							erro = backpropagation(*it, numEntradas, numCapas, red, testi, eta);
							//cout <<"errorsito " << erro << "\n";
							error_global += erro;
							itest++;
						}

						//}
					//entradas[i].clear();
					//entradas[i].seekg(0,ios::beg);	
					errores << error_global << "\n";
					}while(error_global!=0 && iteracion < iteraciones_maximas );
					errores.close();
			    //}

		}

	}

}