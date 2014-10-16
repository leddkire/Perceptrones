#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

class UnidadSigmoidal{
public:
	float *pesosOriginales;
	float* pesos;
	//La primera entrada siempre es 1. Este es el peso interno de la neurona.
	float* entradas;
	float salida = 0;
	int numPesos;
	float derivada = 0;


	UnidadSigmoidal(int nPesos){
		pesosOriginales= new float[nPesos];
		pesos = new float[nPesos];
		numPesos = nPesos;
		for(int i = 0; i< numPesos; i++){
				float valor = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.1);
				pesosOriginales[i] =  valor;
				pesos[i] = valor;
				cout << pesos[i] << ", ";

		};
	}

	int resetPesos(){
			for(int i = 0; i< numPesos; i++){
				pesos[i] = pesosOriginales[i];
				cout << pesos[i] << ", ";
			};
		}

	void calcularSalida(float* entrada){
		//Primero se calcula la suma de todos los wi*xi (Pesos por su entrada).
		float sum = 0;
		sum += pesos[0];
		for(int i = 0; i < numPesos -1; i++){
			sum += pesos[i+1]*entrada[i];
		}
		//Luego se aplica este resultado sobre la funcion sigmoidal, y este sera la salida.
		salida = 1/(1 + exp(-sum));
		entradas = entrada;
	
	}
	

};

class Capa{
public:
	UnidadSigmoidal* neuronas;
	int numNeuronas;
};

float backpropagation(float* ejemplos, int numEjemplos, int numCapas, int numNeuronasInter, Capa* red, float* test, float eta){
	UnidadSigmoidal* capa;
	float* entradas = ejemplos;
	float* nuevasEntradas;
	for(int i =0; i< numCapas; i++){
		nuevasEntradas = new float[red[i].numNeuronas];
		capa = red[i].neuronas;
		for(int j = 0; j < numNeuronasInter; j++){
			capa[j].calcularSalida(entradas);
			nuevasEntradas[j] = capa[j].salida;
		}
		entradas = nuevasEntradas;
		
	}
	//Nota: al final de la iteracion la variable entradas tendra el arreglo de salidas de la capa de output
	capa = red[numCapas-1].neuronas;
	for(int j=0; j<red[numCapas-1].numNeuronas;j++){
		capa[j].derivada = capa[j].salida * (1 - capa[j].salida)*(test[j]-capa[j].salida);
	}
	
	for(int j=numCapas-2; j>=0; j--){
		float sum = 0;
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
			for(int k = 0; k < capa[j].numPesos; k++){
				capa[j].pesos[k+1] += eta*(capa[j].derivada)*capa[j].entradas[k];
			}
			
		}
	}
	
	return 0;
}

