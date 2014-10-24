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

void prueba_Clases(Capa* red, int numEntradas, int numSalidas, int numCapas, string nombreArchivo){
	double entradas[4];
	double test[3];
	double* resultado;
	int aciertos =0;
	int lineas = 0;
	double coordenadas[numEntradas];
	ofstream resultadosPrueba;
	resultadosPrueba.open(nombreArchivo);
	ifstream datosPrueba;
	string linea;
	double esp[3][3] = {{1,0,0},{0,1,0}, {0,0,1}};
	datosPrueba.open("./berzek.data");
	while(getline(datosPrueba,linea)){

		istringstream entrada(linea);
		char ch;
		for(int j = 0 ; j < numEntradas; j++){
			entrada >> coordenadas[j];
			entrada >> ch;
		}
		string tipo;
		entrada >> tipo;
		int tipoEsp =0;
		if(tipo == "Iris-setosa"){
			tipoEsp =0;
		}else if(tipo == "Iris-versicolor"){
			tipoEsp =1;
		}else if(tipo == "Iris-virginica"){
			tipoEsp =2;
		}
		for(int j = 0; j <numSalidas; j++){			
			test[j] = esp[tipoEsp][j];
		}
		resultado = red -> calcularSalida(coordenadas, numEntradas,numCapas,red);
		resultadosPrueba << resultado[0] << " " <<  resultado[1]<< " " <<  resultado[2] << "\n";
		if(resultado[0] >= 0.5 && test[0] == 1){
			if(resultado[1] < 0.5 && test[1] ==0){
				if(resultado[2]<0.5 && test[2] == 0){
					aciertos++;
				}
			}
    	}else if(resultado[0] < 0.5 && test[0]==0){
    		if(resultado[1] >= 0.5 && test[1] ==1){
				if(resultado[2]<0.5 && test[2] == 0){
					aciertos++;
				}
			}else if(resultado[1] < 0.5 && test[1] == 0){
				if(resultado[2]>=0.5 && test[2] == 1){
					aciertos++;
				}
			}

    	}
    	lineas++;
	}

	cout << aciertos<< "/" << lineas << "\n";
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



int main(){
	//Se inicializa la capa intermedia con el valor provisto.
	int numEntradas = 4;
	int numCapas = 2;
	int numSalidas = 3;
	double eta = 0.1;
	ifstream entradas [5];
	entradas[0].open("iris50.txt");
	entradas[1].open("iris60.txt");
	entradas[2].open("iris70.txt");
	entradas[3].open("iris80.txt");
	entradas[4].open("iris90.txt");
	int numArchivos = 5;
	ofstream errores;
	UnidadSigmoidal* salida = new UnidadSigmoidal[numSalidas];
	double* coordenadas = new double[numEntradas];
	double* test = new double[numSalidas];
	//Pidiendo numero de neuronas que habra en la capa intermedia.
	

	for(int numeroNeuronasInter = 4; numeroNeuronasInter <=10; numeroNeuronasInter++){
		
		UnidadSigmoidal* intermedia = new UnidadSigmoidal[numeroNeuronasInter];
		stringstream nombreDir;
		nombreDir << "ejercicio3_" << numeroNeuronasInter;
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
		Capa* red = new Capa[2];
		red[0] = Capa(intermedia,numeroNeuronasInter);
		red[1] = Capa(salida,numSalidas);
		
		
		
		
		cout << "Entrenando con " <<  numeroNeuronasInter << " neuronas en la capa intermedia \n";
		
		for(int i =0; i < numArchivos; i++){
			
			string linea;
			double error;
			//Reiniciamos los pesos a sus valores originales antes de cada entrenamiento
			for(int j = 0; j < numCapas; j++){
				for(int k = 0; k < red[j].numNeuronas; k++){
					red[j].neuronas[k].resetPesos();
				}
			}

			stringstream nombreArchivo;
			nombreArchivo << "./"<< "ejercicio3_" << numeroNeuronasInter << "/" << "errores_clases_porcentaje_" << 50+(10*i);
			ofstream errores;
			errores.open(nombreArchivo.str());
			

			
			double error_global=0;
			int iteraciones_maximas=10000;
			int iteracion = 0;
			int lineas = 0;
			double esp[3][3] = {{1,0,0},{0,1,0}, {0,0,1}};
			do{
			iteracion++;
			error_global =0;
				while(getline(entradas[i],linea)){
					istringstream entrada(linea);
				    string type;
				    char ch;

					for(int j = 0 ; j < numEntradas; j++){
						entrada >> coordenadas[j];
						entrada >> ch;
					}
					
					string tipo;
					entrada >> tipo;
					int tipoEsp =0;
					if(tipo == "Iris-setosa"){
						tipoEsp =0;
					}else if(tipo == "Iris-versicolor"){
						tipoEsp =1;
					}else if(tipo == "Iris-virginica"){
						tipoEsp =2;
					}
					for(int j = 0; j <numSalidas; j++){			
						test[j] = esp[tipoEsp][j];
					}
					error_global += backpropagation(coordenadas, numEntradas, numCapas, red,test, eta);
					lineas++;
				}
			entradas[i].clear();
			entradas[i].seekg(0,ios::beg);	
			errores << error_global << "\n";
			
			}while(error_global >= 0.0001 && iteracion < iteraciones_maximas );
			errores.close();
			stringstream nombrePrueba;
			nombrePrueba << "./ejercicio3_"<< numeroNeuronasInter << "/" << "resultados_conjunto_clases" << i;
			prueba_Clases(red,numEntradas,numSalidas,numCapas,nombrePrueba.str());
		}
		cout << "Se Termino de entrenar \n";

		
	}


}