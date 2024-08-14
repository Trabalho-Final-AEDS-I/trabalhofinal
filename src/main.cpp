#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <sstream>

using namespace std;

//tuple {coluna, numero}

void print(const tuple<int,int> &elemento){
    cout<<"{"<<to_string(get<0>(elemento))<<","<<to_string(get<1>(elemento))<<"},";
}



void readCSV(const string &nome_endereco){
    vector<vector<vector<tuple<int,int>>>> classes;
    

    ifstream file(nome_endereco);
    if(!file){
        cerr<<"Erro ao abrir o arquivo"<<endl;
        exit(1);
    }

    string linha;
    while(getline(file,linha)){
        vector<tuple<int,int>> lista_elementos;

        stringstream aux_linha(linha);
        string valor;
        int coluna = 1;

        while(getline(aux_linha, valor, ',')){
            tuple<int,int> elemento(coluna++, stoi(valor));
            lista_elementos.push_back(elemento);
        }

        int numero_classe = get<1>(lista_elementos.back());
        lista_elementos.pop_back();

        classes[numero_classe].push_back(lista_elementos);




        lista_elementos.clear();
    }

}



int main(){
    readCSV("dataset/teste2.data");
    return 0;
}