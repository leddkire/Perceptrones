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
			//cout << "Pesos sin ajustar : " ;
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
		float entrenar(bool* entradas, float test, float eta, int numEntradas){
			float o;

			o = procesarEntrada(entradas, numEntradas);
			pesos[0] += eta*(test-o);
			for(int i = 0; i < numEntradas; i++){
				pesos[i+1] += eta*(test-o)*entradas[i];
			}
			return (test-o)*(test-o);
		}
			

};

class PerceptronRule{

	public:
		float *pesos;
		float *pesosOriginales;
		int nPesos;
		float *derivadaRegla;
		float *errores;

		PerceptronRule(int numPesos){
			pesos = new float[numPesos];
			derivadaRegla = new float[numPesos];
			pesosOriginales = new float[numPesos];
			errores = new float[numPesos];

			//cout << "Pesos sin ajustar : " << pesos[0] << ", ";
			for(int i = 0; i< numPesos; i++){
				float valor = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/0.1);
				pesosOriginales[i] =  valor;
				pesos[i] = valor;
				cout << pesos[i] << ", ";
				errores[i] = 0.00;
			};
			nPesos = numPesos;
		}

		int resetPesos(){
			for(int i = 0; i< nPesos; i++){
				pesos[i] = pesosOriginales[i];
				cout << pesos[i] << ", ";
			};
		}

		float procesarEntrada(bool* entradas, int numEntradas){
			float sum = 0;
			sum+= pesos[0];
			for(int i =0; i< numEntradas; i++){
				sum += pesos[i+1] * entradas[i];
			}
			return sum;
		}

		/*
		 *	test = valor objetivo, o = es la salida del perceptron, eta = taza de aprendizaje,
		 *	entradas = xi
		 */

		float entrenar(bool* entradas, int test, float eta, int numEntradas){
			float o = procesarEntrada(entradas, numEntradas);
			derivadaRegla[0]+=-(test-o);
			//cout << "Valor pesos: ";
			pesos[0] += eta*(test-o);
			for(int i = 0; i < numEntradas; i++){
				//if(entradas[i] == 0) pesos[i] += eta*(test-o)*(-1);
				pesos[i+1] += eta*(test-o)*entradas[i];
				//cout << pesos[i+1] << "\n";

			}
			//cout << "numEntradas: " << numEntradas << "\n";
			//cout << "Entradas: " << entradas[0] << "," << entradas[1] << "," << entradas[2] << "\n";
			//cout << "OutPut: " << o << "\n";

			return (test-o)*(test-o);
		}
			

};