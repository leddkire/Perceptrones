#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

class UnidadSigmoidal{
public:
	double *pesosOriginales;
	double* pesos;
	//La primera entrada siempre es 1. Este es el peso interno de la neurona.
	double* entradas;
	double salida = 0;
	int numPesos;
	double derivada = 0;
	UnidadSigmoidal(){

	}

	UnidadSigmoidal(int nPesos){
		pesosOriginales= new double[nPesos];
		pesos = new double[nPesos];
		entradas = new double[nPesos -1];
		numPesos = nPesos;
		for(int i = 0; i< numPesos; i++){
				double valor = static_cast <double> (rand()) / static_cast <double> (RAND_MAX/0.1);
				pesosOriginales[i] =  valor;
				pesos[i] = valor;

		};
	}

	int resetPesos(){
			for(int i = 0; i< numPesos; i++){
				pesos[i] = pesosOriginales[i];
			};
		}

	void calcularSalida(double* entrada){
		//Primero se calcula la suma de todos los wi*xi (Pesos por su entrada).
		double sum = 0;
		sum += pesos[0];
		for(int i = 1; i < numPesos; i++){
			sum += pesos[i]*entrada[i-1];
		}
		//Luego se aplica este resultado sobre la funcion sigmoidal, y este sera la salida.
		salida = 1/(1 + exp(-sum));
		for(int i = 0; i< numPesos -1; i++){
			entradas[i] = entrada[i];
		}
		
	
	}
	

};

class Capa{
public:
	UnidadSigmoidal* neuronas;
	int numNeuronas;
	Capa(){

	}
	Capa(UnidadSigmoidal* neuronasDeCapa, int numeroNeuronas){
		neuronas = neuronasDeCapa;
		numNeuronas = numeroNeuronas;
	}
};

double backpropagation(double* ejemplos, int numEjemplos, int numCapas, Capa* red, double* test, double eta){
	UnidadSigmoidal* capa;
	double* entradas = new double[numEjemplos];
	for(int i =0; i < numEjemplos; i++){
		entradas[i] = ejemplos[i];
	}
	double* nuevasEntradas;
	double sumaErrorCuadrado = 0;
	for(int i =0; i< numCapas; i++){
		capa = red[i].neuronas;
		for(int j = 0; j < red[i].numNeuronas; j++){
			capa[j].calcularSalida(entradas);
			
		}
		delete entradas;
		entradas = new double[red[i].numNeuronas];
		for(int j = 0; j < red[i].numNeuronas; j++){
			entradas[j] = capa[j].salida;
			
		}

		
	}
	delete entradas;
	for (int j = 0; j< red[numCapas-1].numNeuronas;j++){
		sumaErrorCuadrado += (test[j] - capa[j].salida)*(test[j] - capa[j].salida);
	}
		
	//Nota: al final de la iteracion la variable entradas tendra el arreglo de salidas de la capa de output
	capa = red[numCapas-1].neuronas;
	for(int j=0; j<red[numCapas-1].numNeuronas;j++){
		capa[j].derivada = capa[j].salida * (1 - capa[j].salida)*(test[j]-capa[j].salida);
		
	}
	
	for(int j=numCapas-2; j>=0; j--){
		double sum = 0;
		UnidadSigmoidal* capaReceptora = red[j+1].neuronas;
		UnidadSigmoidal* capaIntermedia = red[j].neuronas;
		for(int k = 0; k < red[j].numNeuronas; k++){
			for(int i = 0; i < red[numCapas-1].numNeuronas;i++){
				sum+= capaReceptora[i].pesos[k+1] * capaReceptora[i].derivada;
			}
			capaIntermedia[k].derivada = sum;
		}
	}
	//Actualizar pesos
	for(int i=0; i < numCapas; i++){
		capa = red[i].neuronas;
		for(int j=0; j < red[i].numNeuronas; j++){
			capa[j].pesos[0] += eta*(capa[j].derivada);
			for(int k = 1; k < capa[j].numPesos; k++){
				capa[j].pesos[k] += eta*(capa[j].derivada)*capa[j].entradas[k-1];
			}
			
		}
	}
	
	return sumaErrorCuadrado;
}

