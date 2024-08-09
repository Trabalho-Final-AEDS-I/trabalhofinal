#include <iostream>
#include <vector>
#include <tuple>
#include <variant>
#include <fstream>
#include <sstream>

using namespace std;
void print(const tuple<int,int> elementos){
    cout<<"{"<< to_string(get<0>(elementos))<<","<<to_string(get<1>(elementos))<<"},";
}

void readCSV(const string& endereco_arquivo) {
    vector<tuple<int,int>> elementos;
    vector<vector<tuple<int,int>>> list_elementos;
    vector<vector<tuple<int,int>>> classes;

    int tamanho = 0;

    ifstream arquivo(endereco_arquivo);
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo do endereço: " << endereco_arquivo << endl;
        exit(1);
    }

    string linha;
    while (getline(arquivo, linha)) {
        
        
        stringstream ss(linha);
        string valor;
        int numero_coluna = 1;

        while (getline(ss, valor, ',')) {
            tuple<int,int> elemento(stoi(valor),numero_coluna++);
            elementos.push_back(elemento);
        }

        int aux = get<0>(elementos.back());
        elementos.pop_back();

        classes[aux] = elementos;

        for(size_t idx = 0; idx < classes.size(); ++idx) {
                cout << idx << " - "; 
                for(const auto& j : classes[idx]) {
                    print(j);
                }
                cout << endl;
            }

    }

    arquivo.close();
}


int main() {
    readCSV("dataset/teste.data");
    return 0;
}