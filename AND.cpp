
#include <cmath>
#include <fstream>
#include "perceptron.cpp"
using namespace std;

int main(){
	int numBits;

	ofstream archivo;
	archivo.open("Error_And");
	ofstream archivo2;
	archivo2.open("Error_And_Delta");
	cout << "Especifique el numero de entradas que tendra el perceptron: ";
	cin >> numBits;
	cout << "\n";
	//Inicializar datos de entrenamiento
	float eta = 0.1;
	int numEjemplos = pow(2,numBits);

	
	bool **ejemplosBit = new bool*[numEjemplos];
	bool * ejemploBit;
	int * testAND = new int[numEjemplos];
	int *testOR= new int[numEjemplos];
	int *testXOR= new int[numEjemplos];
	for(int i = 0; i < numEjemplos; i++){
		ejemploBit = new bool[numBits];
		int valorACopiar = i;
		for(int j = 0; j< numBits; j++){
			int ultimoBit = valorACopiar & 1;
			ejemploBit[numBits-j-1] = ultimoBit;

			valorACopiar >>=1;
		}


		ejemplosBit[i] = ejemploBit;
	}

//Generacion de resultados para el AND
	for(int i = 0 ; i < numEjemplos; i++){
		ejemploBit = ejemplosBit[i];
		testAND[i] = 1;
		for(int j = 0; j < numBits; j++){
			if(!ejemploBit[j]){
				testAND[i] = -1;
				break;
			}
		}
		
	}


	Perceptron percy = Perceptron(numBits+1);

//EJERCICIO 1
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 1.a)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------------PERCEPTRON AND ----------------\\
	cout << "Entrenando \n";
	bool ** entradas = ejemplosBit;
	int numEntradas = numBits;
	bool* entrada;
	float error_local;
	float error_global=1;
	int iteraciones_maximas=1000;
	int iteracion = 0;
	do{
		iteracion++;
		error_global =0;
		for(int j = 0 ; j < numEjemplos; j++){

			entrada = entradas[j];
			error_global+= percy.entrenar(entrada,testAND[j],eta,numEntradas);
			
		}
		archivo << error_global << "\n";
	}while(error_global!=0 && iteracion <= iteraciones_maximas );

	

	cout << "Termino de entrenar \n";

	cout << "Prueba del AND \n";

	for (int i = 0 ; i < numEjemplos ; i++){
		entrada = entradas[i];
		for(int j = 0; j < numEntradas; j++){
			if(j == numEntradas-1){
				cout << entrada[j] << "\n";
			}else{
				cout << entrada[j] << ", ";
			}
		}
    	int resultado = percy.procesarEntrada(entrada, numEntradas);
		cout << "Resultado: " << resultado << "\n\n*********************\n";
	}

//--------------FIN DE  PERCEPTRON AND ---------------\\
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 1.b)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------ETA PARA AND--------\\

	

	cout << "******************\nEntrenando AND con distintas constantes eta\n";
	float arreta [5] = {0.01,0.1,0.2,0.5,0.99};
	for(int i =0;i < 5; i++){
		percy.resetPesos();


		cout << "Entrenando \n";
		float error_global=1;
		int iteraciones_maximas=1000;
		int iteracion = 0;

		do{
		iteracion++;
		error_global =0;
		for(int j = 0 ; j < numEjemplos; j++){

			entrada = entradas[j];
			error_global+= percy.entrenar(entrada,testAND[j],arreta[i],numEntradas);
			
		}
		cout << error_global << "<<< ERROR GLOBAL\n";
		}while(error_global!=0 && iteracion <= iteraciones_maximas );

		cout << "Termino de entrenar \n";

		cout << "Prueba del AND \n";

		for (int i = 0 ; i < numEjemplos ; i++){
			entrada = entradas[i];
			for(int j = 0; j < numEntradas; j++){
				if(j == numEntradas-1){
					cout << entrada[j] << "\n";
				}else{
					cout << entrada[j] << ", ";
				}
			}
	    	int resultado = percy.procesarEntrada(entrada, numEntradas);
			cout << "Resultado: " << resultado << "\n\n*********************\n";
		}

		cout << "Pesos ajustados: ";
		for (int i = 0; i < percy.nPesos; i++){
			cout << percy.pesos[i] << ", ";
		}
		cout << "\n";
	}
	archivo.close();

//EJERCICIO 2
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 2.a)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------------REGLA DE ENTRENAMIENTO AND-----------------------\\
	error_global = 1;
	iteracion = 0;
	iteraciones_maximas=1000;

	cout << "\n\n/**********************/\nEjercicio 2 con regla de entrenamiento \n/**********************/\n";
	PerceptronRule jackson = PerceptronRule(numBits+1);
	do{
		iteracion++;
		error_global =0;
		for(int j = 0 ; j < numEjemplos; j++){
			entrada = entradas[j];
			if(testAND[j] == -1) error_global+= jackson.entrenar(entrada,0,eta,numEntradas);
			else error_global+= jackson.entrenar(entrada,testAND[j],eta,numEntradas);
			
		}
		archivo2 << error_global << "\n";
	}while(error_global!=0 && iteracion <= iteraciones_maximas );

	

	cout << "Termino de entrenar \n";

	cout << "Prueba del AND \n";

	for (int i = 0 ; i < numEjemplos ; i++){
		entrada = entradas[i];
		for(int j = 0; j < numEntradas; j++){
			if(j == numEntradas-1){
				cout << entrada[j] << "\n";
			}else{
				cout << entrada[j] << ", ";
			}
		}
    	float resultado = jackson.procesarEntrada(entrada, numEntradas);
		cout << "Resultado: " << resultado << "\n\n*********************\n";
	}
	archivo2.close();

}