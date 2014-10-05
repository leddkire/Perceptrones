#include <stdlib.h>
#include <iostream>

using namespace std;

class Perceptron{

	public:
		float *pesosOriginales;
		float *pesos;
		int nPesos;

		Perceptron(int numPesos){
			pesosOriginales = new float[numPesos];
			pesos = new float[numPesos];
			cout << "Pesos sin ajustar : " << pesosOriginales[0] << ", ";
			for(int i = 0; i< numPesos; i++){
				float valor = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.1);
				pesosOriginales[i] =  valor;
				pesos[i] = valor;
				cout << pesos[i] << ", ";

			};
			nPesos = numPesos;
			

			

		}

		int resetPesos(){
			for(int i = 0; i< nPesos; i++){
				pesos[i] = pesosOriginales[i];
				cout << pesos[i] << ", ";
			};
		}

		int procesarEntrada(int entradas[][3], int numEntradas, int numEjemplo){
			float sum = 0;
			for(int i =0; i< numEntradas; i++){
				sum += pesos[i] * entradas[numEjemplo][i];
			}
			if(sum >0){
				return 1;
			}else{
				return -1;
			}
		}

		/*
		 *	test = valor objetivo, o = es la salida del perceptron, eta = taza de aprendizaje,
		 *	entradas = xi
		 */
		void entrenar(int entradas[][3], int test, float eta, int numEntradas, int numEjemplo){
			float o[4];
			for(int i = 0; i<nPesos; i++){
				o[i] = procesarEntrada(entradas, numEntradas, numEjemplo);
			}
			for(int i = 0; i < nPesos; i++){
				pesos[i] += eta*(test-o[i])*entradas[numEjemplo][i];
			}

		}
			

};

class PerceptronRule{

	public:
		float *pesos;
		int nPesos;
		float *derivadaRegla;
		float *errores;

		PerceptronRule(int numPesos){
			pesos = new float[numPesos];
			derivadaRegla = new float[numPesos];
			errores = new float[numPesos];

			cout << "Pesos sin ajustar : " << pesos[0] << ", ";
			for(int i = 0; i< numPesos; i++){
				pesos[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.1);
				cout << pesos[i] << ", ";
				derivadaRegla[i] = 0.00;
				errores[i] = 0.00;
			};
			nPesos = numPesos;
		}

		int resetPesos(){
			for(int i = 0; i< nPesos; i++){
				pesos[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.1);
				cout << pesos[i] << ", ";
				derivadaRegla[i] = 0.00;
				errores[i] = 0.00;
			};
		}

		int procesarEntrada(int entradas[][3], int numEntradas, int numEjemplo){
			float sum = 0;
			for(int i =0; i< numEntradas; i++){
				sum += pesos[i] * entradas[numEjemplo][i];
			}
			if(sum >0){
				return 1;
			}else{
				return -1;
			}
		}

		/*
		 *	test = valor objetivo, o = es la salida del perceptron, eta = taza de aprendizaje,
		 *	entradas = xi
		 */

		void entrenar(int entradas[][3], int test, float eta, int numEntradas, int numEjemplo){
			for(int i = 0; i < nPesos; i++){
				float o = procesarEntrada(entradas, numEntradas, numEjemplo);
				derivadaRegla[i] += -(test-o)*entradas[numEjemplo][i];
				//errores[i] += pow(test-o, 2);
			}

			for(int i = 0; i < nPesos; i++){
				pesos[i] += -eta*derivadaRegla[i];
			}
		}
			

};

int main(){

	//Inicializar datos de entrenamiento
	float eta = 0.1;
	float pesos [3];
	

	//Ejemplos para el AND
	// ej= ejemplo, F = False, T = True
	//El primer 1 es por w0, que siempre es 1
	int entradas [4][3] = {  {1,0,0},
							 {1,0,1}, 
							 {1,1,0}, 
							 {1,1,1}	};
	int numEntradas = 3;
	int ejFF [3] = {1,0,0};
	int ejTF [3] = {1,1,0};
	int ejFT [3] = {1,0,1};
	int ejTT [3] = {1,1,1};

	//Resultados esperados para cada ejemplo
	int test [4] = {-1,-1,-1,1};

	//Inicializando para el OR
	int orFF [3] = {1,0,0};
	int orTF [3] = {1,1,0};
	int orFT [3] = {1,0,1};
	int orTT [3] = {1,1,1};

	//Resultados esperados para cada ejemplo del OR
	int test_or [4] = {-1,1,1,1};

	//Resultados esperados para cada ejemplo del XOR
	int test_xor [4] = {-1,1,1,-1};

	Perceptron percy = Perceptron(3);

//EJERCICIO 1
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 1.a)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------------PERCEPTRON AND ----------------\\

	cout << "Entrenando \n";


	for(int j = 0 ; j < 1000; j++){
		int ejAUsar = j % 4;
		percy.entrenar(entradas,test[ejAUsar],eta,numEntradas, ejAUsar);
	}

	cout << "Termino de entrenar \n";

	cout << "Prueba del AND \n";

	for (int i = 0 ; i < 4 ; i++){
		int lol1 = entradas[i][1];
		int lol2 = entradas[i][2];
    	cout << lol1 << ", " << lol2 << "\n";
		int resultado = percy.procesarEntrada(entradas, numEntradas, i);
		cout << "Resultado: " << resultado << "\n";
	}

	cout << "Pesos ajustados: " << percy.pesos [0] << "," << percy.pesos[1] << ", " << percy.pesos[2] << "\n";

//--------------FIN DE  PERCEPTRON AND ---------------\\

//-------------PERCEPTRON OR -------------------------\\

	cout << "\n\nEntrenando OR \n";

	percy.resetPesos();
	for(int j = 0 ; j < 1000; j++){
		int ejAUsar = j % 4;
		percy.entrenar(entradas,test_or[ejAUsar],eta,numEntradas, ejAUsar);
	}

	cout << "Termino de entrenar \n";

	cout << "Prueba del OR \n";

	for (int i = 0 ; i < 4 ; i++){
		int lol1 = entradas[i][1];
		int lol2 = entradas[i][2];
    	cout << lol1 << ", " << lol2 << "\n";
		int resultado = percy.procesarEntrada(entradas, numEntradas, i);
		cout << "Resultado: " << resultado << "\n";
	}

	cout << "Pesos ajustados: " << percy.pesos [0] << "," << percy.pesos[1] << ", " << percy.pesos[2] << "\n";

//-------------FIN DEL PERCEPTRON OR ----------------\\

//--------------PERCEPTRON XOR----------------------\\


	cout << "\n\nEntrenando XOR \n";

	percy.resetPesos();
	for(int j = 0 ; j < 1000; j++){
		int ejAUsar = j % 4;
		percy.entrenar(entradas,test_xor[ejAUsar],eta,numEntradas, ejAUsar);
	}

	cout << "Termino de entrenar \n";

	cout << "Prueba del XOR \n";

	for (int i = 0 ; i < 4 ; i++){
		int lol1 = entradas[i][1];
		int lol2 = entradas[i][2];
    	cout << lol1 << ", " << lol2 << "\n";
		int resultado = percy.procesarEntrada(entradas, numEntradas, i);
		cout << "Resultado: " << resultado << "\n";
	}

	cout << "Pesos ajustados: " << percy.pesos [0] << "," << percy.pesos[1] << ", " << percy.pesos[2] << "\n";
//------------------------FIN DE PERCEPTRON XOR--------------------------------\\

//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 1.b)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------ETA PARA AND--------\\
	

	cout << "******************\nEntrenando AND con distintas constantes eta\n";
	float arreta [5] = {0.01,0.1,0.2,0.5,0.99};
	for(int i =0;i < 5; i++){

		cout << "Entrenando con eta: " << arreta[i] << "\n";
		percy.resetPesos();

		for(int j = 0 ; j < 10000; j++){
			int ejAUsar = j % 4;
			percy.entrenar(entradas,test[ejAUsar],arreta[i],numEntradas, ejAUsar);
		}

		cout << "Termino de entrenar \n";

		for (int i = 0 ; i < 4 ; i++){
			int lol1 = entradas[i][1];
			int lol2 = entradas[i][2];
	    	cout << lol1 << ", " << lol2 << "\n";
			int resultado = percy.procesarEntrada(entradas, numEntradas, i);
			cout << "Resultado: " << resultado << "\n";
		}

		cout << "Pesos ajustados: " << percy.pesos [0] << "," << percy.pesos[1] << ", " << percy.pesos[2] << "\n";

	}

//----------ETA PARA OR---------\\

	cout << "******************\nEntrenando OR con distintas constantes eta\n";
		for(int i =0;i < 5; i++){

			cout << "Entrenando con eta: " << arreta[i] << "\n";
			percy.resetPesos();

			for(int j = 0 ; j < 10000; j++){
				int ejAUsar = j % 4;
				percy.entrenar(entradas,test_or[ejAUsar],arreta[i],numEntradas, ejAUsar);
			}

			cout << "Termino de entrenar \n";

			for (int i = 0 ; i < 4 ; i++){
				int lol1 = entradas[i][1];
				int lol2 = entradas[i][2];
		    	cout << lol1 << ", " << lol2 << "\n";
				int resultado = percy.procesarEntrada(entradas, numEntradas, i);
				cout << "Resultado: " << resultado << "\n";
			}

			cout << "Pesos ajustados: " << percy.pesos [0] << "," << percy.pesos[1] << ", " << percy.pesos[2] << "\n";

		}

/*


//EJERCICIO 2
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 2.a)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------------REGLA DE ENTRENAMIENTO AND-----------------------\\


	//Parte de la regla de entrenamiento
	PerceptronRule jackson = PerceptronRule(3);
	cout << "\n\n\n\n*************\nAND: \n";
	cout << "Entrenando con la regla de entrenamiento \n";


	for(int j = 0 ; j < 1000; j++){
		int ejAUsar = j % 4;
		jackson.entrenar(entradas,test[ejAUsar],eta,numEntradas, ejAUsar);
	}

	//cout << "Termino de entrenar con regla de entrenamiento \n";

	cout << "Prueba del AND con regla de entrenamiento \n";

	for (int i = 0 ; i < 4 ; i++){
		int lol1 = entradas[i][1];
		int lol2 = entradas[i][2];
		cout << lol1 << ", " << lol2 << "\n";
		int resultado = jackson.procesarEntrada(entradas, numEntradas, i);
		cout << "Resultado con la regla de entrenamiento: " << resultado << "\n";
	}


	cout << "Pesos ajustados con la regla de entrenamiento: "<< percy.pesos [0] << "," << jackson.pesos[1] << ", " << jackson.pesos[2] << "\n";

//-------------------FIN DE REGLA DE ENTRENAMIENTO AND--------------------\\

//------------------REGLA DE ENTRENAMIENTO OR -------------------------\\
	cout << "\n\n*************\nOR: \n";
	cout << "Entrenando con la regla de entrenamiento \n";

	jackson.resetPesos();
	for(int j = 0 ; j < 1000; j++){
		int ejAUsar = j % 4;
		jackson.entrenar(entradas,test_or[ejAUsar],eta,numEntradas, ejAUsar);
	}

	//cout << "Termino de entrenar con regla de entrenamiento \n";

	cout << "Prueba del OR con regla de entrenamiento \n";

	for (int i = 0 ; i < 4 ; i++){
		int lol1 = entradas[i][1];
		int lol2 = entradas[i][2];
		cout << lol1 << ", " << lol2 << "\n";
		int resultado = jackson.procesarEntrada(entradas, numEntradas, i);
		cout << "Resultado con la regla de entrenamiento: " << resultado << "\n";
	}

	cout << "Pesos ajustados con la regla de entrenamiento: "<< percy.pesos [0] << "," << jackson.pesos[1] << ", " << jackson.pesos[2] << "\n";

//----------FIN DE REGLA DE ENTRENAMIENTO OR-----------\\

//-----------REGLA DE ENTRENAMIENTO XOR--------------------\\
	
	cout << "\n\n*************\nXOR con diferentes etas: \n";
	for(int t=0;t < 5; t++){
		float arreta [5] = {0.01, 0.1, 0.2, 0.5, 0.99};
		cout << "\n\nEntrenando con la regla de entrenamiento iteracion blah\n";

		jackson.resetPesos();
		for(int j = 0 ; j < 1000; j++){
			int ejAUsar = j % 4;
			jackson.entrenar(entradas,test_xor[ejAUsar],arreta[t],numEntradas, ejAUsar);
		}


		//cout << "Termino de entrenar con regla de entrenamiento \n";

		cout << "Prueba del XOR con regla de entrenamiento \n";

		for (int i = 0 ; i < 4 ; i++){
			int lol1 = entradas[i][1];
			int lol2 = entradas[i][2];
			cout << lol1 << ", " << lol2 << "\n";
			int resultado = jackson.procesarEntrada(entradas, numEntradas, i);
			cout << "Resultado con la regla de entrenamiento: " << resultado << "\n";
		}

		cout << "Pesos ajustados con la regla de entrenamiento: "<< percy.pesos [0] << "," << jackson.pesos[1] << ", " << jackson.pesos[2] << "\n";

	}

//----------------FIN DE REGLA DE ENTRENAMIENTO XOR-------------\\

	/*cout << "\n\n*************\nXOR con etas decrecientes: \n";
	//for(int t=0;t < 5; t++){
	
	cout << "\n\nEntrenando con la regla de entrenamiento iteracion blah\n";

	jackson.resetPesos();
	for(int j = 0 ; j < 100000; j++){
		int ejAUsar = j % 4;
		jackson.entrenar(entradas,test_xor[ejAUsar],1/j,numEntradas, ejAUsar);
	}


	//cout << "Termino de entrenar con regla de entrenamiento \n";

	cout << "Prueba del XOR con regla de entrenamiento \n";

	for (int i = 0 ; i < 4 ; i++){
		int lol1 = entradas[i][1];
		int lol2 = entradas[i][2];
		cout << lol1 << ", " << lol2 << "\n";
		int resultado = jackson.procesarEntrada(entradas, numEntradas, i);
		cout << "Resultado con la regla de entrenamiento: " << resultado << "\n";
	}

	cout << "Pesos ajustados con la regla de entrenamiento: " << jackson.pesos[1] << ", " << jackson.pesos[2] << "\n";

	//}*/


}