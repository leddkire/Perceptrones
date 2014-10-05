#include <stdlib.h>
#include <iostream>
#include <bitset>
#include <cmath>

using namespace std;

class Perceptron{

	public:
		float *pesosOriginales;
		float *pesos;
		int nPesos;

		Perceptron(int numPesos){
			pesosOriginales = new float[numPesos];
			pesos = new float[numPesos];
			cout << "Pesos sin ajustar : " ;
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

		int procesarEntrada(bool* entradas, int numEntradas){
			float sum = 0;
			sum += pesos[0];
			for(int i =0; i< numEntradas; i++){

				sum += pesos[i+1] * entradas[i];


			}
			
			if(sum >0){
				return 1;
			}else{
				return -1;
			}
		}

		/*
		 *	test = valor objetivo, o = es la salida del perceptron, eta = tasa de aprendizaje,
		 *	entradas = xi
		 */
		void entrenar(bool* entradas, float test, float eta, int numEntradas){
			float o;

			o = procesarEntrada(entradas, numEntradas);
			pesos[0] += eta*(test-o);
			for(int i = 0; i < numEntradas; i++){
				pesos[i+1] += eta*(test-o)*entradas[i];
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

		int procesarEntrada(bool* entradas, int numEntradas){
			float sum = 0;
			sum+= pesos[0];
			for(int i =0; i< numEntradas; i++){
				sum += pesos[i+1] * entradas[i];
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

		void entrenar(bool* entradas, int test, float eta, int numEntradas){
			float o = procesarEntrada(entradas, numEntradas);
			derivadaRegla[0]+=-(test-o); 
			for(int i = 0; i < numEntradas; i++){
				
				derivadaRegla[i+1] += -(test-o)*entradas[i];
				//errores[i] += pow(test-o, 2);
			}

			for(int i = 0; i < nPesos; i++){
				pesos[i] += -eta*derivadaRegla[i];
			}
		}
			

};

int main(){
	int numBits;


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
//Generacion de resultados para el OR
	for(int i = 0 ; i < numEjemplos; i++){
		ejemploBit = ejemplosBit[i];
		testOR[i] = -1;
		for(int j = 0; j < numBits; j++){
			if(ejemploBit[j]){
				testOR[i] = 1;
				break;
			}
		}

		
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
//Generacion de resultados para el XOR
	for(int i = 0 ; i < numEjemplos; i++){
		int contadorXOR = 0;
		ejemploBit = ejemplosBit[i];
		testXOR[i] = -1;
		for(int j = 0; j < numBits; j++){
			if(ejemploBit[j]){
				if(contadorXOR == 0){
					contadorXOR++;
					testXOR[i] = 1;
				}else if( contadorXOR >=1){
					testXOR[i] = -1;
					break;
				}
				
				
			}
		}
		
	}

	for(int i = 0 ; i < numEjemplos; i++){

		cout << testAND[i] << "\n";
		

	}
	cout << "OR\n";
	for(int i = 0 ; i < numEjemplos; i++){

		cout <<testOR[i] << "\n";
		

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

	


	for(int j = 0 ; j < numEjemplos*1000; j++){
		int ejAUsar = j % numEjemplos;
		entrada = entradas[ejAUsar];
		percy.entrenar(entrada,testAND[ejAUsar],eta,numEntradas);
	}

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

//-------------PERCEPTRON OR -------------------------\\


	cout << "\n\nEntrenando OR \n";

	cout << "Entrenando \n";


	for(int j = 0 ; j < numEjemplos*1000; j++){
		int ejAUsar = j % numEjemplos;
		percy.entrenar(entradas[ejAUsar],testOR[ejAUsar],eta,numEntradas);
	}

	cout << "Termino de entrenar \n";

	cout << "Prueba del OR \n";

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

//-------------FIN DEL PERCEPTRON OR ----------------\\
/*
//--------------PERCEPTRON XOR----------------------\\


	cout << "\n\nEntrenando XOR \n";


	cout << "Entrenando \n";


	for(int j = 0 ; j < numEjemplos*1000; j++){
		int ejAUsar = j % numEjemplos;
		percy.entrenar(entradas[ejAUsar],testXOR[ejAUsar],eta,numEntradas);
	}

	cout << "Termino de entrenar \n";

	cout << "Prueba del XOR \n";

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
//------------------------FIN DE PERCEPTRON XOR--------------------------------\\
*/

//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 1.b)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------ETA PARA AND--------\\

	

	cout << "******************\nEntrenando AND con distintas constantes eta\n";
	float arreta [5] = {0.01,0.1,0.2,0.5,0.99};
	for(int i =0;i < 5; i++){
		percy.resetPesos();


		cout << "Entrenando \n";


		for(int j = 0 ; j < numEjemplos*1000; j++){
		int ejAUsar = j % numEjemplos;
		entrada = entradas[ejAUsar];
		percy.entrenar(entrada,testAND[ejAUsar],arreta[i],numEntradas);
		}

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

//----------ETA PARA OR---------\\

	cout << "******************\nEntrenando OR con distintas constantes eta\n";
		for(int i =0;i < 5; i++){
		percy.resetPesos();


		cout << "Entrenando \n";


		for(int j = 0 ; j < numEjemplos*1000; j++){
		int ejAUsar = j % numEjemplos;
		entrada = entradas[ejAUsar];
		percy.entrenar(entrada,testOR[ejAUsar],arreta[i],numEntradas);
		}

		cout << "Termino de entrenar \n";

		cout << "Prueba del OR \n";

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




//EJERCICIO 2
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//PARTE 2.a)
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//----------------REGLA DE ENTRENAMIENTO AND-----------------------\\


	//Parte de la regla de entrenamiento
	PerceptronRule jackson = PerceptronRule(numBits+1);
	cout << "\n\n\n\n*************\nAND: \n";
	cout << "Entrenando con la regla de entrenamiento \n";


	for(int j = 0 ; j < numEjemplos*1000; j++){
		int ejAUsar = j % numEjemplos;
		jackson.entrenar(entradas[ejAUsar],testAND[ejAUsar],eta,numEntradas);
	}

	//cout << "Termino de entrenar con regla de entrenamiento \n";

	cout << "Prueba del AND con regla de entrenamiento \n";
	for (int i = 0 ; i < numEjemplos ; i++){
			entrada = entradas[i];
			for(int j = 0; j < numEntradas; j++){
				if(j == numEntradas-1){
					cout << entrada[j] << "\n";
				}else{
					cout << entrada[j] << ", ";
				}
			}
	    	int resultado = jackson.procesarEntrada(entrada, numEntradas);
			cout << "Resultado: " << resultado << "\n\n*********************\n";
		}

		cout << "Pesos ajustados: ";
		for (int i = 0; i < jackson.nPesos; i++){
			cout << jackson.pesos[i] << ", ";
		}
		cout << "\n";
	



//-------------------FIN DE REGLA DE ENTRENAMIENTO AND--------------------\\

//------------------REGLA DE ENTRENAMIENTO OR -------------------------\\
	cout << "\n\n*************\nOR: \n";
	cout << "Entrenando con la regla de entrenamiento \n";

	jackson.resetPesos();
	for(int j = 0 ; j < numEjemplos*1000; j++){
		int ejAUsar = j % numEjemplos;
		jackson.entrenar(entradas[ejAUsar],testOR[ejAUsar],eta,numEntradas);
	}

	//cout << "Termino de entrenar con regla de entrenamiento \n";

	cout << "Prueba del OR con regla de entrenamiento \n";
	for (int i = 0 ; i < numEjemplos ; i++){
			entrada = entradas[i];
			for(int j = 0; j < numEntradas; j++){
				if(j == numEntradas-1){
					cout << entrada[j] << "\n";
				}else{
					cout << entrada[j] << ", ";
				}
			}
	    	int resultado = jackson.procesarEntrada(entrada, numEntradas);
			cout << "Resultado: " << resultado << "\n\n*********************\n";
		}

		cout << "Pesos ajustados: ";
		for (int i = 0; i < jackson.nPesos; i++){
			cout << jackson.pesos[i] << ", ";
		}
		cout << "\n";
	

//----------FIN DE REGLA DE ENTRENAMIENTO OR-----------\\

//-----------REGLA DE ENTRENAMIENTO XOR--------------------\\
	
	

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