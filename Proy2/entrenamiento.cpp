#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include "UnidadSigmoidal.cpp"
using namespace std;

//La generacion de datos acorde al enunciado
//Se tiene una region cuadrada del (0,0) al (20,20)
//Hay una circunferencia dentro de la region
//Para un arreglo N de puntos, la mitad tiene que estar adentro de la circunferencia
//La otra mitad fuera de ella y dentro de la region cuadrada.
void generarDatos(float arreglo[][3] , int tam){
	int numDatosFueraCirc = tam / 2;
	int numDatosDentroCirc = tam / 2;
	float x, y;
	int i = 0;
	random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<> distr(0, 20);
    while(numDatosDentroCirc > 0 && numDatosFueraCirc > 0){
    	x = distr(eng);
    	y = distr(eng);
    	float comprobacion = pow(x-10,2) + pow(y-10,2);
    	if(comprobacion <= 49 && numDatosDentroCirc > 0){
    		numDatosDentroCirc--;
    		arreglo[i][2] = -1;
    	}else if(comprobacion >49 && numDatosFueraCirc >0){
    		numDatosFueraCirc--;
    		arreglo[i][2] = 1;
    	}
    	arreglo[i][0] = x;
    	arreglo[i][1] = y;
    	i++;
    }
	
}

void entrenar(float entradas[][3], int tam, int numEntradas, int numSalidas, int numCapas, Capa* red, float eta){
	
	float error_global=1;
	int iteraciones_maximas=1000;
	int iteracion = 0;
	float* coordenadas = new float[numEntradas];
	float* test = new float[numSalidas];
	stringstream nombreArchivo;
	nombreArchivo << "errores_" << tam;
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
		errores << error_global << "<<< ERROR GLOBAL\n";
	}while(error_global!=0 && iteracion <= iteraciones_maximas );

	for(int j = 0; j < numCapas; j++){
		for(int k = 0; k < red[j].numNeuronas; k++){
			red[j].neuronas[k].resetPesos();
		}
	}
	errores.close();
}

int main(){
	//Se inicializa la capa intermedia con el valor provisto.
	int numeroNeuronasInter;
	int numEntradas = 2;
	int numCapas = 2;
	int numSalidas = 1;
	float eta = 0.1;
	ifstream entradas [3];
	float entradasGeneradas500 [500][3];
	float entradasGeneradas1000 [1000][3];
	float entradasGeneradas2000 [2000][3];
	ofstream errores;
	//Pidiendo numero de neuronas que habra en la capa intermedia.
	cout << "Ingrese un valor entero del 2 al 10\n";
	cin >> numeroNeuronasInter;
	UnidadSigmoidal* intermedia = new UnidadSigmoidal[numeroNeuronasInter];
	UnidadSigmoidal* salida = new UnidadSigmoidal[numSalidas];
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
	errores.open("errores_2");
	
	if(numeroNeuronasInter < 2 || numeroNeuronasInter > 10){
		cout << "Error, no se ingreso un valor correcto. Corra el programa nuevamente.\n";
	}
	
	entradas[0].open("datos_P1_2_SD2014_n500.txt");
	entradas[1].open("datos_P1_2_SD2014_n1000.txt");
	entradas[2].open("datos_P1_2_SD2014_n2000.txt");

	for(int i =0; i < 3; i++){
		float* coordenadas = new float[numEntradas];
		float* test = new float[numSalidas];
		string linea;
		float error;
		//Reiniciamos los pesos a sus valores originales antes de cada entrenamiento
		for(int j = 0; j < numCapas; j++){
			for(int k = 0; k < red[j].numNeuronas; k++){
				red[j].neuronas[k].resetPesos();
			}
		}


		errores << "Conjunto de prueba numero: " << i << "\n";

		cout << "Entrenando \n";
		float error_global=1;
		int iteraciones_maximas=1000;
		int iteracion = 0;

		do{
		iteracion++;
		error_global =0;
			while(getline(entradas[i],linea)){
				stringstream entrada(linea);
				for(int j = 0 ; j < numEntradas; j++){
					entrada >> coordenadas[j];
				}
				for(int j = 0; j <numSalidas; j++){
					entrada >> test[j];
				}
				error_global += backpropagation(coordenadas, numEntradas, numCapas, red,test, eta);

			}
		entradas[i].clear();
		entradas[i].seekg(0,ios::beg);	
		errores << error_global << "<<< ERROR GLOBAL\n";
		}while(error_global!=0 && iteracion <= iteraciones_maximas );
		
	}
	
	for(int i = 0; i < 3; i++){
		entradas[i].close();
	}

	//Ahora se generan los datos del segundo conjunto de entrenamiento y se entrena
	generarDatos(entradasGeneradas500,500);
	generarDatos(entradasGeneradas1000,1000);
	generarDatos(entradasGeneradas2000,2000);
	entrenar(entradasGeneradas500,500, numEntradas, numSalidas, numCapas, red, eta);
	entrenar(entradasGeneradas1000,1000,numEntradas, numSalidas,numCapas, red, eta);
	entrenar(entradasGeneradas2000,2000,numEntradas, numSalidas,numCapas, red, eta);
	errores.close();


}