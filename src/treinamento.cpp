#include "treinamento.hpp"
#include <fstream>
#include <iostream>
#include <sstream>


Treinamento::Treinamento() {}

void Treinamento::printarValores(vector<vector<int>> classes, map<tuple<int,int>,vector<int>> features) const {
    cout << "Classes:" << endl;
    for (size_t i = 0; i < classes.size(); ++i) {
        cout << "Classe " << i << ": ";
        for (int valor : classes[i]) {
            cout << valor << " ";
        }
        cout << endl;
    }

    cout << "Features:" << endl;
    for (const auto &feature : features) {
        cout << "Feature (";
        cout << get<0>(feature.first) << ", " << get<1>(feature.first) << "): ";
        //for (int valor : feature.second) {
        //   cout << valor << " ";
        //}
        cout << endl;
    }
}

void Treinamento::processando(const string &filename, vector<vector<tuple<int,int>>>* data,  vector<vector<int>> *classes, map<tuple<int,int>,vector<int>> *features){

    ifstream file(filename);
    if(!file){
        cerr<<"Erro ao abrir o arquivo"<<endl;
        exit(1);
    }

    string line;
    int row = 1;
    while(getline(file,line)){
        vector<tuple<int,int>> list_line;
        stringstream ss(line);
        string valor;
        int chave = 1;
    
        while(getline(ss, valor, ',')){
            tuple<int, int> elemento(chave++, stoi(valor));
            list_line.push_back(elemento);

            if(!ss.eof()){
                (*features)[elemento].push_back(row);
            }
        }

        int numero_classe = get<1>(list_line.back());
        list_line.pop_back();
        data->push_back(list_line);


        if (numero_classe >= static_cast<int>(classes->size())) {
            classes->resize(numero_classe + 1);
        }
        (*classes)[numero_classe].push_back(row);

        list_line.clear();
        row ++;
    }

    file.close();
}

    //printarValores(vector<vector<int>> classes, map<tuple<int,int>,vector<int>> features);




