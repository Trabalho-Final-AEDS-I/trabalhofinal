/*
Pensei em dois tipos de algoritmos
 1 - sem um vector geral fixo
 2 - com vector geral fixo, onde as classes podem acessar 
*/


#include "CSV.hpp"
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

CSV::CSV(){
}

void CSV::readCSV(const string &nome_endereco){
    vector<vector<tuple<int,int>>> dados_geral;

    vector<vector<int>> classes;

    tuple<tuple<int,int>,vector<int>> features;
    //(elemento, )

    ifstream file(nome_endereco);
    if(!file){
        cerr<<"Erro ao abrir o arquivo"<<endl;
        exit(1);
    }

    string linha;
    int numero_linha = 1;
    while(getline(file,linha)){
        vector<tuple<int,int>> lista_elementos;

        stringstream aux_linha(linha);
        string valor;
        int numero_coluna = 1;

        while(getline(aux_linha, valor, ',')){
            tuple<int,int> elemento(numero_coluna, stoi(valor));
            lista_elementos.push_back(elemento);
            numero_coluna++;
        }

        int numero_classe = get<1>(lista_elementos.back());
        lista_elementos.pop_back();

        dados_geral.push_back(lista_elementos);

        if (numero_classe >= static_cast<int>(classes.size())) {
            classes.resize(numero_classe + 1);
        }

        classes[numero_classe].push_back(numero_linha);


        lista_elementos.clear();
        numero_linha ++;
    }
    int m = 0;
    for(auto &i : classes){
        cout<<"Linha"<< to_string(m)<<": ";
        for(auto &j: i){
            cout<< to_string(j)<<", ";
        }
        cout<<endl;
        m++;
    }


}

