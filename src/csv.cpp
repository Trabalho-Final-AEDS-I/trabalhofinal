#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;



void readCSV(const string& endereco_arquivo) {
    vector<int> classes;
    vector<int> features;

    ifstream arquivo(endereco_arquivo);
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo do endereço: " << endereco_arquivo << endl;
        exit(1);
    }

    string linha;
    while (getline(arquivo, linha)) {
        vector<Elemento> numeros;

        stringstream ss(linha);
        string valor;
        unsigned short int numero_coluna = 1;

        while (getline(ss, valor, ',')) {
            numeros.emplace_back(stoi(valor), numero_coluna);
            numero_coluna++;
        }

        for(int i = 0; i < numeros.size() - 1 ; i ++){
            if(features.)
            features.emplace_back();
        }



    }


}

int main() {
    readCSV("../dataset/teste.data");
    return 0;
}
