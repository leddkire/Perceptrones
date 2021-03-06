#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <ga/GASimpleGA.h>
#include <ga/GA1DBinStrGenome.h>
#include <ga/GAIncGA.h>
#include <ga/std_stream.h>
#include <ga/GASStateGA.h>

#define NUMERO_MAX_REGLAS 8
#define NUMERO_MIN_REGLAS 4
#define RULE_SIZE 1902
#define EXPERIMENTOS 1

using namespace std;

class dosVec{
public:
	vector<string> ant;
	vector<string> cons;
};

//Declaracion de funciones
string codificar2(string);

void Initializer(GAGenome&);
float Objective(GAGenome &);
float fitnessPorTam(GAGenome &);
int Crossover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
float test(GAGenome&, dosVec);
//Declaracion de variables globales


//Programa Principal
int main(int argc, char* argv[]){
	ifstream archivoEntren;
	ifstream archivoPrueba;
	ofstream promedioResultados;
	vector<string> conjuntoEntrenamientoAnt;
	vector<string> conjuntoEntrenamientoCons;
	dosVec conjuntoEntrenamiento;
	vector<string> conjuntoPruebaAnt;
	vector<string> conjuntoPruebaCons;
	dosVec conjuntoPruebas;
	string lineaE;
	int popsize  = 2000;
  int ngen     = 1000;
  float pmut   = 0.00000000001;
  float pcross = 0.8;

	//Definicion de parametros para el algoritmo
	if(argc == 7){
		archivoEntren.open(argv[1]);
		archivoPrueba.open(argv[2]);
		popsize  = atoi(argv[3]);
		ngen     = atoi(argv[4]);
		pmut   = atof(argv[5]);
		pcross = atof(argv[6]);


	}else if(argc == 3){
		archivoEntren.open(argv[1]);
		archivoPrueba.open(argv[2]);


	}else if(argc == 1){
		archivoEntren.open("pixel_input.txt");
		archivoPrueba.open("pixel_test.txt");

	}
	//Codificacion de conjunto de entrenamiento y prueba
	while(getline(archivoEntren,lineaE)){
		stringstream streamLineaE(lineaE);
		string dato;
		stringstream datoEntrenAnt;
		stringstream datoEntrenCons;
		for(int i = 0; i < (RULE_SIZE -2)/4; i++){
			streamLineaE >> dato;
			int datoNumerico = stof(dato);
			if(datoNumerico >= 0.0 && datoNumerico < 0.25){
				datoEntrenAnt << "0001";
			}else if (datoNumerico >= 0.25 && datoNumerico < 0.5){
				datoEntrenAnt << "0010";
			}else if(datoNumerico >= 0.5 && datoNumerico < 0.75){
				datoEntrenAnt << "0100";
			}else if(datoNumerico >= 0.75 && datoNumerico <= 1.0){
				datoEntrenAnt << "1000";
			}

		}
		conjuntoEntrenamientoAnt.push_back(datoEntrenAnt.str());
		streamLineaE >> dato;
		datoEntrenCons << codificar2(dato);
		conjuntoEntrenamientoCons.push_back(datoEntrenCons.str());

	}
	while(getline(archivoPrueba,lineaE)){
		stringstream streamLineaE(lineaE);
		string dato;
		stringstream datoPruebaAnt;
		stringstream datoPruebaCons;
		for(int i = 0; i < (RULE_SIZE -2)/3; i++){
			streamLineaE >> dato;
			int datoNumerico = stoi(dato);
			if(datoNumerico == 0){
				datoPruebaAnt << "001";
			}else if (datoNumerico == 5){
				datoPruebaAnt << "010";
			}else if(datoNumerico == 1){
				datoPruebaAnt << "100";
			}

		}

		conjuntoPruebaAnt.push_back(datoPruebaAnt.str());
		streamLineaE >> dato;
		datoPruebaCons << codificar2(dato);
		conjuntoPruebaCons.push_back(datoPruebaCons.str());

	}

	conjuntoEntrenamiento.ant = conjuntoEntrenamientoAnt;
	conjuntoEntrenamiento.cons = conjuntoEntrenamientoCons;
	conjuntoPruebas.ant = conjuntoPruebaAnt;
	conjuntoPruebas.cons = conjuntoPruebaCons;
	//Definiendo el genoma
	GA1DBinaryStringGenome genoma(0,fitnessPorTam,&conjuntoEntrenamiento);

	promedioResultados.open("resultados");
	genoma.initializer(Initializer);
	genoma.crossover(Crossover);
	float porcentajeTotal =0.0;
	float numReglasTotal =0.0;
	GA1DBinaryStringGenome mejor(0,fitnessPorTam,&conjuntoPruebas);
		GASteadyStateGA ga(genoma);
		ga.pReplacement(0.5);
		ga.scaling(GASigmaTruncationScaling());
		ga.populationSize(popsize);
	  	ga.nGenerations(ngen);
	  	ga.pMutation(pmut);
	  	ga.pCrossover(pcross);
	  	ga.flushFrequency(25);
	  	ga.scoreFrequency(25);
	  	for(int i = 0; i < EXPERIMENTOS; i++){
	  		ga.evolve();
	  		//cout << "The GA found:\n" << ga.statistics().bestIndividual() << "\n";
	  		GA1DBinaryStringGenome lol(0,fitnessPorTam,&conjuntoPruebas);
	  		lol.copy(ga.statistics().bestIndividual());
	  		float prueba = test(lol,conjuntoPruebas)/conjuntoPruebaAnt.size();
		  	porcentajeTotal += prueba;
	  		numReglasTotal += lol.length()/RULE_SIZE;
  		}

 	cout << ga.statistics() << "\n";

	  	promedioResultados << "    PORCENTAJE :" << porcentajeTotal/EXPERIMENTOS << "\n";
	  	promedioResultados << "    NUMERO DE REGLAS : " <<  numReglasTotal/EXPERIMENTOS << "\n";
	  	cout << "    PORCENTAJE :" << porcentajeTotal/EXPERIMENTOS<< "\n";
	  	cout << "    NUMERO DE REGLAS : " <<  numReglasTotal/EXPERIMENTOS << "\n";

	archivoPrueba.close();
	archivoEntren.close();
	promedioResultados.close();
	return 0;
}

void Initializer(GAGenome& g){
	GA1DBinaryStringGenome & genoma = (GA1DBinaryStringGenome &)g;
	int reglas = GARandomInt(1,NUMERO_MAX_REGLAS);
	int tamGenoma = RULE_SIZE*reglas;
	genoma.resize(tamGenoma);
	genoma.randomize();

}

float fitnessPorTam(GAGenome& g){
	GA1DBinaryStringGenome & genoma = (GA1DBinaryStringGenome &)g;
	dosVec* cjtoEntren = (dosVec*)g.userData();
	vector<string> cjtoEntrenAnt = cjtoEntren -> ant;
	vector<string> cjtoEntrenCons = cjtoEntren -> cons;
	stringstream streamG;
	string datosGenoma;
	double numCorrectos = 0.0;
	//Derivar el numero de reglas en el genoma
	unsigned int longitudAnt = cjtoEntrenAnt[0].size();
	unsigned int longitudCon = cjtoEntrenCons[0].size();
	int longitudTotal = (longitudAnt +longitudCon);
	int numReglas = genoma.length()/longitudTotal;


	//Extrayendo las reglas individuales
	bool match = true;
	for(unsigned int i =0; i < cjtoEntrenAnt.size(); i++){
		
		for(int k = 0; k < numReglas; k++){
			match = true;
			for(unsigned int j =0; j < cjtoEntrenAnt[i].length();j++){
				
				if(cjtoEntrenAnt[i].at(j) == '1'){
					if(genoma.gene(j+(longitudTotal*k)) == 0){

						match = false;
						break;
					}
				}
			}
			//Si encontro una regla que aplica, evaluar el resultado para ver si es correcto
			
			if(match){
				cout << "MATCH\n";
				bool matchRes = true;

				for(unsigned int j = 0; j < cjtoEntrenCons[i].length();j++){

					if((cjtoEntrenCons[i].at(j) - '0') != genoma.gene(cjtoEntrenAnt[i].length()+j +(longitudTotal*k)) ){
						matchRes = false;
						break;
					}
				}
				if(matchRes){
					numCorrectos += 1.0;
					break;
				}
			}
		}
	}
	double porcentajeCorrectos = numCorrectos/cjtoEntrenAnt.size();
	double fitness = ((porcentajeCorrectos * porcentajeCorrectos));
	
	return fitness;
}

int Crossover(const GAGenome& padre1, const GAGenome& padre2, GAGenome* hijo1, GAGenome* hijo2){
	GA1DBinaryStringGenome& padre = (GA1DBinaryStringGenome&) padre1;
	GA1DBinaryStringGenome& madre = (GA1DBinaryStringGenome&) padre2;

	
	unsigned int punto1p = GARandomInt(0, padre.length());

	unsigned int punto2p = GARandomInt(0, padre.length());
	int minPPadre = min(punto1p,punto2p);
	int maxPPadre = max(punto1p,punto2p);
	
	unsigned int punto2m;
	
	int temp = maxPPadre - minPPadre;
	temp = temp % RULE_SIZE;
	unsigned int punto1m = GARandomInt(0, madre.length() -((maxPPadre - minPPadre) % RULE_SIZE));
	int distancia = temp;

	for(int i = 0; i < madre.length(); i++){
		punto2m = i;
		temp = punto1m - punto2m;
		if(abs(temp) == distancia){
			break;
		}

	}
	int minPMadre = min(punto1m,punto2m);
	int maxPMadre = max(punto1m,punto2m);
	int numHijos = 0;
	if(hijo1){
		GA1DBinaryStringGenome &hermana = (GA1DBinaryStringGenome&) *hijo1;
		hermana.resize(minPPadre+distancia+padre.length()-maxPPadre);
		hermana.copy(padre,0,0,minPPadre);
		hermana.copy(madre,minPPadre,minPMadre, distancia);
		hermana.copy(padre,minPPadre+distancia,maxPPadre,padre.length()-maxPPadre);
		if(hermana.length() > RULE_SIZE*NUMERO_MAX_REGLAS){
			hermana.resize(RULE_SIZE*NUMERO_MAX_REGLAS);
		}
		numHijos++;
	}
	
	int distancia2 = maxPPadre - minPPadre;
	if(hijo2){
		GA1DBinaryStringGenome &hermano = (GA1DBinaryStringGenome&)*hijo2;
		hermano.resize(minPMadre + abs(distancia2)+madre.length()-maxPMadre);
		hermano.copy(madre,0,0,minPMadre);
		hermano.copy(padre,minPMadre,minPPadre,abs(distancia2));
		hermano.copy(madre,minPMadre + abs(distancia2),maxPMadre,madre.length()-maxPMadre);
		if(hermano.length() > RULE_SIZE*NUMERO_MAX_REGLAS){
			hermano.resize(RULE_SIZE*NUMERO_MAX_REGLAS);
		}
		numHijos++;

	}
	
	return numHijos;
}

string codificar2(string dato){
	if(dato.compare("happy") == 0){
		return "00";
	}else if(dato.compare("angry") ==0){
		return "01";
	}else if(dato.compare("sad") == 0){
		return "10";
	}else{
		return "11";
	}
}

float test(GAGenome& g, dosVec pruebas){
	
	GA1DBinaryStringGenome & genoma = (GA1DBinaryStringGenome &)g;
	dosVec* cjtoEntren = &pruebas;
	vector<string> cjtoEntrenAnt = cjtoEntren -> ant;
	vector<string> cjtoEntrenCons = cjtoEntren -> cons;
	stringstream streamG;
	string datosGenoma;
	double numCorrectos = 0.0;
	//Derivar el numero de reglas en el genoma
	unsigned int longitudAnt = cjtoEntrenAnt[0].size();
	unsigned int longitudCon = cjtoEntrenCons[0].size();
	int longitudTotal = (longitudAnt +longitudCon);
	int numReglas = genoma.length()/longitudTotal;


	//Extrayendo las reglas individuales
	bool match = true;
	for(unsigned int i =0; i < cjtoEntrenAnt.size(); i++){
		
		for(int k = 0; k < numReglas; k++){
			match = true;
			for(unsigned int j =0; j < cjtoEntrenAnt[i].length();j++){
			
				if(cjtoEntrenAnt[i].at(j) == '1'){
					if(genoma.gene(j+(longitudTotal*k)) == 0){
						match = false;
						break;
					}
				}
			}
			//Si encontro una regla que aplica, evaluar el resultado para ver si es correcto
			
			if(match){

				bool matchRes = true;

				for(unsigned int j = 0; j < cjtoEntrenCons[i].length();j++){

					if((cjtoEntrenCons[i].at(j) - '0') != genoma.gene(cjtoEntrenAnt[i].length()+j +(longitudTotal*k)) ){
						matchRes = false;
						break;
					}
				}
				if(matchRes){
					numCorrectos += 1.0;
					break;
				}
			}
		}
	}
	return numCorrectos;
}