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

#define NUMERO_MAX_REGLAS 10
#define NUMERO_MIN_REGLAS 3
#define RULE_SIZE 30
#define EXPERIMENTOS 100

using namespace std;

class dosVec{
public:
	vector<string> ant;
	vector<string> cons;
};

//Declaracion de funciones
string codificar1(string);
string codificar2(string);
string codificar3(string);
string codificar4(string);
string codificar5(string);

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
	int popsize  = 100;
  int ngen     = 20;
  float pmut   = 0.01;
  float pcross = 0.9;
  int tipoFit = 1;
	
	//Definicion de parametros para el algoritmo
	if(argc == 8){
		archivoEntren.open(argv[1]);
		archivoPrueba.open(argv[2]);
		popsize  = atoi(argv[3]);
		ngen     = atoi(argv[4]);
		pmut   = atof(argv[5]);
		pcross = atof(argv[6]);
		tipoFit = atoi(argv[7]);

	}else if(argc == 3){
		archivoEntren.open(argv[1]);
		archivoPrueba.open(argv[2]);


	}else if(argc == 1){
		archivoEntren.open("50E");
		archivoPrueba.open("50P");

	}
	//Codificacion de conjunto de entrenamiento y prueba
	while(getline(archivoEntren,lineaE)){
		stringstream streamLineaE(lineaE);
		string dato;
		stringstream datoEntrenAnt;
		stringstream datoEntrenCons;
		getline(streamLineaE, dato,',');
		datoEntrenAnt << codificar1(dato);
		getline(streamLineaE, dato,',');
		datoEntrenAnt << codificar2(dato);
		getline(streamLineaE, dato,',');
		datoEntrenAnt << codificar3(dato);
		getline(streamLineaE, dato,',');
		datoEntrenAnt << codificar4(dato);
		conjuntoEntrenamientoAnt.push_back(datoEntrenAnt.str());
		getline(streamLineaE, dato,',');
		datoEntrenCons << codificar5(dato);
		conjuntoEntrenamientoCons.push_back(datoEntrenCons.str());

	}
	while(getline(archivoPrueba,lineaE)){
		stringstream streamLineaE(lineaE);
		string dato;
		stringstream datoPruebaAnt;
		stringstream datoPruebaCons;
		getline(streamLineaE, dato,',');
		datoPruebaAnt << codificar1(dato);
		getline(streamLineaE, dato,',');
		datoPruebaAnt << codificar2(dato);
		getline(streamLineaE, dato,',');
		datoPruebaAnt << codificar3(dato);
		getline(streamLineaE, dato,',');
		datoPruebaAnt << codificar4(dato);
		conjuntoPruebaAnt.push_back(datoPruebaAnt.str());
		getline(streamLineaE, dato,',');
		datoPruebaCons << codificar5(dato);
		conjuntoPruebaCons.push_back(datoPruebaCons.str());

	}
	conjuntoEntrenamiento.ant = conjuntoEntrenamientoAnt;
	conjuntoEntrenamiento.cons = conjuntoEntrenamientoCons;
	conjuntoPruebas.ant = conjuntoPruebaAnt;
	conjuntoPruebas.cons = conjuntoPruebaCons;
	//Definiendo el genoma
	GA1DBinaryStringGenome genoma(0,Objective,&conjuntoEntrenamiento);

	if(tipoFit == 2){
		genoma = GA1DBinaryStringGenome(0,fitnessPorTam,&conjuntoEntrenamiento);
		promedioResultados.open("promedioResultadosConPenalizacion");
	}else{
		promedioResultados.open("promedioResultados");
	}
	genoma.initializer(Initializer);
	genoma.crossover(Crossover);
	//Definiendo el algoritmo y parametros de los experimentos 1 y 2
	GASteadyStateGA ga(genoma);
	ga.pReplacement(0.5);
	ga.scaling(GASigmaTruncationScaling());
	ga.populationSize(popsize);
  	ga.nGenerations(ngen);
  	ga.pMutation(pmut);
  	ga.pCrossover(pcross);
  	ga.flushFrequency(25);
  	ga.scoreFrequency(25);

  	
  	//Experimento 1
  	//Seleccion de padre: Rueda de Ruleta. Seleccion de sobrevivientes: reemplazo del 50 por ciento
  	float porcentajeTotal = 0.0;
  	float numReglasTotal = 0.0;
  	for(int i = 0; i < EXPERIMENTOS; i++){
  		ga.evolve();
  		//cout << "The GA found:\n" << ga.statistics().bestIndividual() << "\n";
  		GA1DBinaryStringGenome lol(0,fitnessPorTam,&conjuntoPruebas);
  		lol.copy(ga.statistics().bestIndividual());
  		porcentajeTotal += test(lol,conjuntoPruebas)/conjuntoPruebaAnt.size();
  		numReglasTotal += lol.length()/RULE_SIZE;
  	}
  	promedioResultados << "Seleccion de padre: Rueda de Ruleta. Seleccion de sobrevivientes: reemplazo del 50 por ciento \n";
  	promedioResultados << "    PORCENTAJE :" << porcentajeTotal/EXPERIMENTOS << "\n";
  	promedioResultados << "    NUMERO DE REGLAS : " <<  numReglasTotal/EXPERIMENTOS << "\n";
  	//Experimento 2
  	//Seleccion de padre: Rueda de Ruleta. Seleccion de sobrevivientes: reemplazo del 50 por ciento
  	porcentajeTotal = 0.0;
  	numReglasTotal = 0.0;
  	ga.selector(GATournamentSelector());
  	for(int i = 0; i < EXPERIMENTOS; i++){
  		ga.evolve();
  		//cout << "The GA found:\n" << ga.statistics().bestIndividual() << "\n";
  		GA1DBinaryStringGenome lol(0,fitnessPorTam,&conjuntoPruebas);
  		lol.copy(ga.statistics().bestIndividual());
  		porcentajeTotal += test(lol,conjuntoPruebas)/conjuntoPruebaAnt.size();
  		numReglasTotal += lol.length()/RULE_SIZE;
  	}
  	promedioResultados << "\nSeleccion de padre: Torneo. Seleccion de sobrevivientes: reemplazo del 50 por ciento \n";
  	promedioResultados << "    PORCENTAJE :" << porcentajeTotal/EXPERIMENTOS << "\n";
  	promedioResultados << "    NUMERO DE REGLAS : " <<  numReglasTotal/EXPERIMENTOS << "\n";
  	
  	//Definicion de algoritmo y parametros para los experimentos 3 y 4
  	GAIncrementalGA gaInc(genoma);
	gaInc.scaling(GASigmaTruncationScaling());
	gaInc.populationSize(popsize);
  	gaInc.nGenerations(ngen);
  	gaInc.pMutation(pmut);
  	gaInc.pCrossover(pcross);
  	gaInc.flushFrequency(25);
  	gaInc.scoreFrequency(25);

  	//Experimento 3
  	//Seleccion de padre: Rueda de Ruleta. Seleccion de sobrevivientes: reemplazo del 50 por ciento
  	porcentajeTotal = 0.0;
  	numReglasTotal = 0.0;


  	for(int i = 0; i < EXPERIMENTOS; i++){
  		gaInc.evolve();
  		//cout << "The GA found:\n" << gaInc.statistics().bestIndividual() << "\n";
  		GA1DBinaryStringGenome lol(0,fitnessPorTam,&conjuntoPruebas);
  		lol.copy(gaInc.statistics().bestIndividual());
  		porcentajeTotal += test(lol,conjuntoPruebas)/conjuntoPruebaAnt.size();
  		numReglasTotal += lol.length()/RULE_SIZE;
  	}

  	promedioResultados << "\nSeleccion de padre: Rueda de Ruleta. Seleccion de sobrevivientes: Sobreviven los dos mejores y reemplazan a los dos peores \n";
  	promedioResultados << "    PORCENTAJE :" << porcentajeTotal/EXPERIMENTOS << "\n";
  	promedioResultados << "    NUMERO DE REGLAS : " <<  numReglasTotal/EXPERIMENTOS << "\n";
  	//Experimento 4
  	//Seleccion de padre: Rueda de Ruleta. Seleccion de sobrevivientes: reemplazo del 50 por ciento

  	porcentajeTotal = 0.0;
  	numReglasTotal = 0.0;
  	gaInc.selector(GATournamentSelector());
  	for(int i = 0; i < EXPERIMENTOS; i++){
  		gaInc.evolve();
  		//cout << "The GA found:\n" << gaInc.statistics().bestIndividual() << "\n";
  		GA1DBinaryStringGenome lol(0,fitnessPorTam,&conjuntoPruebas);
  		lol.copy(gaInc.statistics().bestIndividual());
  		porcentajeTotal += test(lol,conjuntoPruebas)/conjuntoPruebaAnt.size();
  		numReglasTotal += lol.length()/RULE_SIZE;
  	}
  	promedioResultados << "\nSeleccion de padre: Torneo. Seleccion de sobrevivientes: Sobreviven los dos mejores y reemplazan a los dos peores \n";
  	promedioResultados << "    PORCENTAJE :" << porcentajeTotal/EXPERIMENTOS << "\n";
  	promedioResultados << "    NUMERO DE REGLAS : " <<  numReglasTotal/EXPERIMENTOS << "\n";
  	
  	cout << "PORCENTAJE :" << porcentajeTotal/EXPERIMENTOS << "\n";
  	cout << "NUMERO DE REGLAS : " <<  numReglasTotal/EXPERIMENTOS << "\n";

  
  

  archivoPrueba.close();
  archivoEntren.close();
  promedioResultados.close();
}

void Initializer(GAGenome& g){
	GA1DBinaryStringGenome & genoma = (GA1DBinaryStringGenome &)g;
	int reglas = GARandomInt(1,NUMERO_MAX_REGLAS);
	int tamGenoma = RULE_SIZE*reglas;
	genoma.resize(tamGenoma);
	genoma.randomize();

}

float Objective(GAGenome& g){
	
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


	//Revisando si alguna parte de la regla hay un consequente "11"
	for(int i =0; i < numReglas; i++){
		if(genoma.gene(cjtoEntrenAnt[0].length()+(i*longitudTotal))==1 && genoma.gene(cjtoEntrenAnt[0].length()+(i*longitudTotal)+1)==1){
			return 0.0;
		}
	}
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
	double porcentajeCorrectos = numCorrectos/cjtoEntrenAnt.size();
	return (porcentajeCorrectos) * porcentajeCorrectos;
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


	//Revisando si alguna parte de la regla hay un consequente "11"
	for(int i =0; i < numReglas; i++){
		if(genoma.gene(cjtoEntrenAnt[0].length()+(i*longitudTotal))==1 && genoma.gene(cjtoEntrenAnt[0].length()+(i*longitudTotal)+1)==1){
			return 0.0;
		}
	}

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
	double porcentajeCorrectos = numCorrectos/cjtoEntrenAnt.size();
	double fitness = ((porcentajeCorrectos * porcentajeCorrectos)) - (numReglas*0.001);
	
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

//6 bits
string codificar1(string dato){
	double datoNumerico = stoi(dato);
	if(4.0 <= datoNumerico && datoNumerico < 4.5){
		return "100000";
	}else if(4.5 <= datoNumerico && datoNumerico < 5.0){
		return "010000";
	}else if(5.0 <= datoNumerico && datoNumerico < 5.5){
		return "001000";
	}else if(5.5 <= datoNumerico && datoNumerico < 6.0){
		return "000100";
	}else if(6.0 <= datoNumerico && datoNumerico < 6.5){
		return "000010";
	}else{
		return "000001";
	}
}
//5 bits
string codificar2(string dato){
	double datoNumerico = stoi(dato);
	if(2.0 <= datoNumerico && datoNumerico < 2.5){
		return "10000";
	}else if(2.5 <= datoNumerico && datoNumerico < 3.0){
		return "01000";
	}else if(3.0 <= datoNumerico && datoNumerico < 3.5){
		return "00100";
	}else if(3.5 <= datoNumerico && datoNumerico < 4.0){
		return "00010";
	}else{
		return "00001";
	}
}
//12 bits
string codificar3(string dato){
	double datoNumerico = stoi(dato);
	if(1.0 <= datoNumerico && datoNumerico < 1.5){
		return "100000000000";
	}else if(1.5 <= datoNumerico && datoNumerico < 2.0){
		return "010000000000";
	}else if(2.0 <= datoNumerico && datoNumerico < 2.5){
		return "001000000000";
	}else if(2.5 <= datoNumerico && datoNumerico < 3.0){
		return "000100000000";
	}else if(3.0 <= datoNumerico && datoNumerico < 3.5){
		return "000010000000";
	}else if(3.5 <= datoNumerico && datoNumerico < 4.0){
		return "000001000000";
	}else if(4.0 <= datoNumerico && datoNumerico < 4.5){
		return "000000100000";
	}else if(4.5 <= datoNumerico && datoNumerico < 5.0){
		return "000000010000";
	}else if(5.0 <= datoNumerico && datoNumerico < 5.5){
		return "000000001000";
	}else if(5.5 <= datoNumerico && datoNumerico < 6.0){
		return "000000000100";
	}else if(6.0 <= datoNumerico && datoNumerico < 6.5){
		return "000000000010";
	}else{
		return "000000000001";
	}
}
//5 bits
string codificar4(string dato){
	double datoNumerico = stoi(dato);
	if(0 <= datoNumerico && datoNumerico < 0.5){
		return "10000";
	}else if(0.5 <= datoNumerico && datoNumerico < 1.0){
		return "01000";
	}else if(1 <= datoNumerico && datoNumerico < 1.5){
		return "00100";
	}else if(1.5 <= datoNumerico && datoNumerico < 2.0){
		return "00010";
	}else{
		return "00001";
	}
	return dato;
}
//2 bits
string codificar5(string dato){
	if(dato.compare("Iris-setosa") == 0){
		return "00";
	}else if(dato.compare("Iris-versicolor") ==0){
		return "01";
	}else if(dato.compare("Iris-virginica") == 0){
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


	//Revisando si alguna parte de la regla hay un consequente "11"
	for(int i =0; i < numReglas; i++){
		if(genoma.gene(cjtoEntrenAnt[0].length()+(i*longitudTotal))==1 && genoma.gene(cjtoEntrenAnt[0].length()+(i*longitudTotal)+1)==1){
			return 0;
		}
	}
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