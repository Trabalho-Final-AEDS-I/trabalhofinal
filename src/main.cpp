#include "treinamento.hpp"
#include <iostream>
#include <chrono>

string file_input = "dataset/poker-hand-training.data";
//string file_input = "dataset/teste2.data";
string file_output = "dataset/poker-hand-testing.data";



int main() {
    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
    vector<vector<int>> classes;
    map<tuple<int,int>,vector<int>> features;
    vector<vector<tuple<int,int>>> data;

    Treinamento treinamento;
    treinamento.processando(file_output, &data, &classes, &features);
    //treinamento.printarValores(classes, features);


    chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Tempo de execução: " << elapsed_seconds.count() << "s" << endl;
    return 0;
}