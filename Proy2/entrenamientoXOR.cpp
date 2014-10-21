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
	double ent[4][2] = { {0,0}, {0,1}, {1,0}, {1,1}};
	double entradas[2];
	double test[4] = {-1,1,1,-1};
	double* resultado;
	ofstream resultadosPrueba;
	resultadosPrueba.open(nombreArchivo);	
	for(int i = 0; i<4; i++){
		entradas[0] = ent[i][0];
		entradas[1] = ent[i][1];

    	resultado = red -> calcularSalida(entradas, numEntradas,numCapas,red);
    		cout << entradas[0] << ", " << entradas[1] << "  " << 
    		red[numCapas-1].neuronas[0].salida << ", " << test[i] << "\n";

	}
	resultadosPrueba.close();
	
}
void generarDatos(double arreglo[][3] , int tam){
	int numDatosFueraCirc = tam / 2;
	int numDatosDentroCirc = tam / 2;
	double x, y;
	int i = 0;
	random_device rd;
    mt19937 eng(rd());
    uniform_real_distribution<> distr(0, 20);
    while(numDatosDentroCirc > 0 && numDatosFueraCirc > 0){
    	x = distr(eng);
    	y = distr(eng);
    	double comprobacion = pow(x-10,2) + pow(y-10,2);
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
/*
void entrenar(double entradas[][3], int tam, int numEntradas, int numSalidas, int numCapas, Capa* red, double eta, double* coordenadas, double* test){
	
	double error_global=0;
	int iteraciones_maximas=1000;
	int iteracion = 0;
	stringstream nombreArchivo;
	nombreArchivo << "./" << "ejercicio2_" << red[0].numNeuronas << "/" << "errores_generados_" << tam;
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

	stringstream nombreArchivoR;
	nombreArchivoR<< "./" << "ejercicio2_" << red[0].numNeuronas << "/" << "resultados_" << tam;

	//prueba_2(red, numEntradas,numCapas,nombreArchivoR.str());

}
*/
int main(){
	//Se inicializa la capa intermedia con el valor provisto.
	int numEntradas = 2;
	int numCapas = 2;
	int numSalidas = 1;
	double eta = 0.5;

	ofstream errores;
	UnidadSigmoidal* salida = new UnidadSigmoidal[numSalidas];
	double entradas[4][2] = { {0,0}, {0,1}, {1,0}, {1,1}};
	double* coordenadas = new double[numEntradas];
	double res[4] = {-1,1,1,-1};
	double* test = new double[1];
	//Pidiendo numero de neuronas que habra en la capa intermedia.
	for(int numeroNeuronasInter = 2; numeroNeuronasInter <=2; numeroNeuronasInter++){
		
		UnidadSigmoidal* intermedia = new UnidadSigmoidal[numeroNeuronasInter];
		stringstream nombreDir;
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
		for(int i =0; i < 3; i++){
			
			string linea;
			double error;
			//Reiniciamos los pesos a sus valores originales antes de cada entrenamiento
			for(int j = 0; j < numCapas; j++){
				for(int k = 0; k < red[j].numNeuronas; k++){
					red[j].neuronas[k].resetPesos();
				}
			}

			stringstream nombreArchivo;
			nombreArchivo << "./"<< "XOR_errores" << numeroNeuronasInter;
			ofstream errores;
			errores.open(nombreArchivo.str());
			

			
			double error_global=0;
			int iteraciones_maximas=100000;
			int iteracion = 0;

			do{
			iteracion++;
			error_global =0;
				
				for(int j = 0; j < 4; j++){
					coordenadas[0] = entradas[j][0];
					coordenadas[1] = entradas[j][1];
					if(res[j] == -1){
						test[0] = 0;
					} else{
						test[0] = res[j];
					}
					
					
					error_global += backpropagation(coordenadas, numEntradas, numCapas, red,test, eta);
				}
				errores << error_global << "\n";
			}while(error_global!=0 && iteracion < iteraciones_maximas );
			
		}
		stringstream nombreArchivoR;
		nombreArchivoR << "./resultadosXOR_"<< numeroNeuronasInter;
		prueba_2(red,numEntradas,numCapas,nombreArchivoR.str());
		
		cout << "Se Termino de entrenar \n";


		
	}


}