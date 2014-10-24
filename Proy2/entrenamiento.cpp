#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <sys/stat.h>
#include <sys/types.h>
#include "UnidadSigmoidal.cpp"

using namespace std;

//La generacion de datos acorde al enunciado
//Se tiene una region cuadrada del (0,0) al (20,20)
//Hay una circunferencia dentro de la region
//Para un arreglo N de puntos, la mitad tiene que estar adentro de la circunferencia
//La otra mitad fuera de ella y dentro de la region cuadrada.

void prueba_2(Capa* red, int numEntradas, int numCapas, string nombreArchivo){
	int tamPrueba = 100;
	double entradas[2];
	double test[1];
	double* resultado;
	int aciertos =0;
	ofstream resultadosPrueba;
	ofstream resultadosPruebaAfuera;
	resultadosPrueba.open(nombreArchivo);	
	resultadosPruebaAfuera.open(nombreArchivo+"Afuera");	
	for(int i = 0; i<tamPrueba; i++){
		for(int j =0; j < tamPrueba; j++){
			entradas[0] = double(i) / 5.0;
			entradas[1] = double(j) / 5.0;
			double comprobacion = pow(entradas[0]-10,2) + pow(entradas[1]-10,2);
    		if(comprobacion <= 49){
    			
    			test[0] = -1;
    		}else if(comprobacion >49){
    			
    			test[0] = 1;
    		}
    		entradas[0] = entradas[0]/20.0;
    		entradas[1] = entradas[1]/20.0;
    		resultado = red -> calcularSalida(entradas, numEntradas,numCapas,red);
    		if(resultado[0] > 0.5 && test[0] == 1){
    			aciertos++;
    		}else if(resultado[0] <= 0.5 && test[0]==-1){
    			aciertos++;
    		}
    		if(resultado[0] < 0.5  && test[0] == 1){
    			for(int k =0; k < red[numCapas-1].numNeuronas; k++){

    				resultadosPrueba << entradas[0]*100 << " " << entradas[1]*100 << "\n";
    			}
    		} else if(resultado[0] >= 0.5 && test[0]==-1) {
    				resultadosPruebaAfuera << entradas[0]*100 << " " << entradas[1]*100 << "\n";
    		}
		}
	}
	cout << aciertos<< "/" << tamPrueba*tamPrueba << "\n";
	resultadosPrueba.close();
	resultadosPruebaAfuera.close();
}

int main(){
	//Se inicializa la capa intermedia con el valor provisto.
	int numEntradas = 2;
	int numCapas = 2;
	int numSalidas = 1;
	double eta = 0.1;
	ifstream entradas [6];
	double entradasGeneradas500 [500][3];
	double entradasGeneradas1000 [1000][3];
	double entradasGeneradas2000 [2000][3];
	entradas[0].open("datos_P1_2_SD2014_n500.txt");
	entradas[1].open("datos_P1_2_SD2014_n1000.txt");
	entradas[2].open("datos_P1_2_SD2014_n2000.txt");
	entradas[3].open("datos_P1_2_generados_500");
	entradas[4].open("datos_P1_2_generados_1000");
	entradas[5].open("datos_P1_2_generados_2000");
	int numArchivos = 6;
	ofstream errores;
	UnidadSigmoidal* salida = new UnidadSigmoidal[numSalidas];
	double* coordenadas = new double[numEntradas];
	double* test = new double[numSalidas];
	//Pidiendo numero de neuronas que habra en la capa intermedia.
	

	for(int numeroNeuronasInter = 2; numeroNeuronasInter <=10; numeroNeuronasInter++){
		
		UnidadSigmoidal* intermedia = new UnidadSigmoidal[numeroNeuronasInter];
		stringstream nombreDir;
		nombreDir << "ejercicio2_" << numeroNeuronasInter;
		string temp = nombreDir.str();
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
		//Abriendo archivos de entrada
		
		
		
		
		cout << "Entrenando con " <<  numeroNeuronasInter << " neuronas en la capa intermedia \n";

		//Ahora se generan los datos del segundo conjunto de entrenamiento y se entrena
		
		for(int i =0; i < 6; i++){
			
			string linea;
			double error;
			//Reiniciamos los pesos a sus valores originales antes de cada entrenamiento
			for(int j = 0; j < numCapas; j++){
				for(int k = 0; k < red[j].numNeuronas; k++){
					red[j].neuronas[k].resetPesos();
				}
			}

			stringstream nombreArchivo;
			nombreArchivo << "./"<< "ejercicio2_" << numeroNeuronasInter << "/" << "errores_conjunto_" << i;
			ofstream errores;
			errores.open(nombreArchivo.str());
			

			
			double error_global=0;
			int iteraciones_maximas=10000;
			int iteracion = 0;
			int lineas = 0;
			do{
			iteracion++;
			error_global =0;
				while(getline(entradas[i],linea)){
					stringstream entrada(linea);
					for(int j = 0 ; j < numEntradas; j++){
						entrada >> coordenadas[j];
						coordenadas[j] = coordenadas[j]/20.0;
					}
					for(int j = 0; j <numSalidas; j++){
						entrada >> test[j];
						if(test[j] == -1){
							test[j] == 0;
						}
					}
					error_global += backpropagation(coordenadas, numEntradas, numCapas, red,test, eta);
					lineas++;
				}
			entradas[i].clear();
			entradas[i].seekg(0,ios::beg);	
			errores << error_global << "\n";
			
			}while(error_global >= 0 && iteracion < iteraciones_maximas );
			errores.close();
			stringstream nombrePrueba;
			nombrePrueba << "./ejercicio2_"<< numeroNeuronasInter << "/" << "resultados_conjunto_" << i;
			prueba_2(red,numEntradas,numCapas,nombrePrueba.str());
		}
		
		cout << "Se Termino de entrenar \n";

		
	}

	for(int i = 0; i < 3; i++){
			entradas[i].close();
		}

}