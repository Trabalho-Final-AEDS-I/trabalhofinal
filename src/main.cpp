#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <functional>
#include <unordered_map>

using namespace std;

namespace std {
    template <>
    struct hash<tuple<int, int>> {
        size_t operator()(const tuple<int, int>& t) const {
            auto h1 = hash<int>{}(get<0>(t));
            auto h2 = hash<int>{}(get<1>(t));
            return h1 ^ (h2 << 1); // Combina os hashes
        }
    };
}
vector<tuple<int, int>> verificarFeatures(const unordered_map<tuple<int, int>, vector<tuple<int, int>>> &features_map, const tuple<int, int> &elemento) {
    auto aux = features_map.find(elemento);
    if (aux != features_map.end()) {
        return aux->second;
    }
    return {}; 
}

void print(const tuple<int,int> elementos){
    cout<<"{"<< to_string(get<0>(elementos))<<","<<to_string(get<1>(elementos))<<"},";
}

void classe( vector<vector<tuple<int,int>>> *list_classes, const vector<vector<tuple<int,int>>> &list_elementos){

    int idx_classe;

    for( auto i : list_elementos){
        idx_classe = get<1>(i.back());
        i.pop_back();

        list_classes[idx_classe].push_back(i);
    }

        for( auto i : list_elementos){
        idx_classe = get<1>(i.back());
        i.pop_back();

        list_classes[idx_classe].push_back(i);
    }


    int linha = 0;
    for( auto i: *list_classes->begin()){
        cout<<"linha"<<to_string(linha)<<endl;
        for( auto &j: i->begin()){
            for( auto &k : j){

            }
        }
    }
 
}



//tuple (coluna, valor)
vector<vector<tuple<int,int>>> readCSV(const string& endereco_arquivo) {
    vector<tuple<int,int>> elementos;
    vector<vector<tuple<int,int>>> list_elementos;

    vector<tuple<int, int>> features;

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
            tuple<int,int> elemento(numero_coluna++,stoi(valor));
            elementos.push_back(elemento);
        }
        elementos.clear();
        tamanho++;
    }
    arquivo.close();


    vector<vector<tuple<int,int>>> list_classes(tamanho);
    unordered_map<tuple<int, int>, vector<vector<tuple<int,int>>>> features_map;


    classe(&list_classes, list_elementos);

    return {{{}}}; 
}





int main() {
    readCSV("dataset/teste.data");
    return 0;
}