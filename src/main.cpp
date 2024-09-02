#include "treinamento.hpp"
#include "teste.hpp"
#include <iostream>
#include <chrono>

string file_input_train = "dataset/poker-hand-training.data";
string file_input_test = "dataset/poker-hand-testing.data";
string file_output = "dataset/output.txt";

int main() {
    cout<<"Iniciando execução..."<<endl;

    vector<vector<int>> classes;
    map<tuple<int,int>,vector<int>> features;

    cout<<"Processando arquivo de treinamento..."<<endl;
    Treinamento treinamento;
    treinamento.processando(file_input_train, &classes, &features);

    Teste teste;
    cout<<"Processando arquivo de teste..."<<endl;
    chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();

    teste.testando(file_input_test, file_output, &classes, &features);

    chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Tempo de execução: " << elapsed_seconds.count() << "s" << endl;
    return 0;
}