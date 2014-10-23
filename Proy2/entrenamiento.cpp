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
	resultadosPrueba.open(nombreArchivo);	
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
    		if(resultado[0] >= 0.5 && test[0] == 1){
    			aciertos++;
    		}else if(resultado[0] < 0.5 && test[0]==-1){
    			aciertos++;
    		}
    		if(resultado[0] < 0.5){
    			for(int k =0; k < red[numCapas-1].numNeuronas; k++){

    				resultadosPrueba << entradas[0]*100 << " " << entradas[1]*100 << "\n";
    			}

    		}
    		
			
		}
	}
	cout << aciertos<< "/" << tamPrueba*tamPrueba << "\n";
	resultadosPrueba.close();
}
void generarDatos(double arreglo[][3] , int tam){
	
   /* while(numDatosDentroCirc > 0 || numDatosFueraCirc > 0 && i < tam){
    	x = distr(eng);
    	y = distr(eng);
    	double comprobacion = pow(x-10,2) + pow(y-10,2);
    	if(comprobacion <= 49 && numDatosDentroCirc > 0){
    		numDatosDentroCirc--;
    		arreglo[i][2] = 0;
    	}else if(comprobacion >49 && numDatosFueraCirc >0){
    		numDatosFueraCirc--;
    		arreglo[i][2] = 1;
    	}
    	arreglo[i][0] = x/20.0;
    	arreglo[i][1] = y/20.0;
    	i++;
    }*/
	
}

void entrenar(double entradas[][3], int tam, int numEntradas, int numSalidas, int numCapas, Capa* red, double eta, double* coordenadas, double* test){
	
	double error_global=0;
	int iteraciones_maximas=10000;
	int iteracion = 0;
	stringstream nombreArchivo;
	nombreArchivo << "./" << "ejercicio2_" << red[0].numNeuronas << "/" << "errores_generados_" << tam;
	ofstream errores;
	errores.open(nombreArchivo.str());

	int numDatosFueraCirc =0;
	int numDatosDentroCirc =0;
	

	double x, y;
	int i = 0;
	random_device rd;
    mt19937 eng(rd());
    uniform_real_distribution<> distr(0, 20);


    do{
    	iteracion++;
		error_global =0;
		int numDatosFueraCirc =0;
		int numDatosDentroCirc =0;
		while(numDatosDentroCirc < tam/2 || numDatosFueraCirc <tam/2){
			x = distr(eng);
    		y = distr(eng);
    		double comprobacion = pow(x-10,2) + pow(y-10,2);
    		if(comprobacion <= 49.0 && numDatosDentroCirc < tam/2 ){
    			test[0] = 0;
    			coordenadas[0] = x/20.0;
    			coordenadas[1] = y/20.0;
 				numDatosDentroCirc++;
 				error_global += backpropagation(coordenadas, numEntradas, numCapas, red,test, eta);
    		}else if(comprobacion >49.0 && numDatosFueraCirc < tam/2){
    			test[0] = 1;
    			coordenadas[0] = x/20.0;
    			coordenadas[1] = y/20.0;
 				numDatosFueraCirc++;
 				
 				error_global += backpropagation(coordenadas, numEntradas, numCapas, red,test, eta);
    		}
		}
    		
    	
    	errores << error_global << "\n";
	}while(error_global >= 0.01 && iteracion < iteraciones_maximas );


		
	for(int j = 0; j < numCapas; j++){
		for(int k = 0; k < red[j].numNeuronas; k++){
			red[j].neuronas[k].resetPesos();
		}
	}
	errores.close();

	stringstream nombreArchivoR;
	nombreArchivoR<< "./" << "ejercicio2_" << red[0].numNeuronas << "/" << "resultados_" << tam;

	prueba_2(red, numEntradas,numCapas,nombreArchivoR.str());

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
		
		/*
		generarDatos(entradasGeneradas500,500);
		entrenar(entradasGeneradas500,500, numEntradas, numSalidas, numCapas, red, eta, coordenadas, test);
		generarDatos(entradasGeneradas1000,1000);
		entrenar(entradasGeneradas1000,1000,numEntradas, numSalidas,numCapas, red, eta, coordenadas, test);
		generarDatos(entradasGeneradas2000,2000);
		entrenar(entradasGeneradas2000,2000,numEntradas, numSalidas,numCapas, red, eta, coordenadas, test);
		*/
		cout << "Se Termino de entrenar \n";

		
	}

	for(int i = 0; i < 3; i++){
			entradas[i].close();
		}

}