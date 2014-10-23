#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <random>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
int main(){
    random_device rd;
    mt19937 eng(rd());
    uniform_real_distribution<> distr(0, 20);
    ofstream archivo [3];
    stringstream a500;
    string base = "datos_P1_2_generados_";

    int n[3] = {500,1000,2000};
    double xCirc;
    double yCirc;
    int turno =0;

    for(int j = 0 ; j < 3;j++){
        stringstream a;
        a << base << n[j];
        archivo[j].open(a.str());
        for(int i = 0; i < n[j]; i++){
            if(turno == 0){
                while(1){
                    xCirc = distr(eng);
                    yCirc = distr(eng);
                    double comprobacion = pow(xCirc-10,2) + pow(yCirc-10,2);
                    if(comprobacion <= 49){
                        archivo[j] << xCirc << " " << yCirc << " " << -1 << "\n";
                        turno =1;

                        break;
                    }
                }
            }else if(turno ==1 ){
                while(1){
                    xCirc = distr(eng);
                    yCirc = distr(eng);
                    double comprobacion = pow(xCirc-10,2) + pow(yCirc-10,2);
                    if(comprobacion > 49){
                        archivo[j] << xCirc << " " << yCirc << " " << 1 << "\n";
                        turno =0;

                        break;
                    }

                }

            }

        }
        archivo[j].close();


    }

}
