#include "treinamento.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>

mutex mtx;

Treinamento::Treinamento() {}

void processarLinhaTreinamento(const string& line, int row, vector<vector<int>>* classes, map<tuple<int, int>, vector<int>>* features) {
    stringstream ss(line);
    string valor;
    int chave = 1;
    int numero_classe;

    while (getline(ss, valor, ',')) {
        if (ss.peek() == EOF) {
            numero_classe = stoi(valor);
            lock_guard<mutex> lock(mtx);
            if (numero_classe >= static_cast<int>(classes->size())) {
                classes->resize(numero_classe + 1);
            }
            (*classes)[numero_classe].push_back(row);
        } else {
            tuple<int, int> elemento(chave++, stoi(valor));
            lock_guard<mutex> lock(mtx);
            (*features)[elemento].push_back(row);
        }
    }
}

void Treinamento::processando(const string& filename, vector<vector<int>>* classes, map<tuple<int, int>, vector<int>>* features) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erro ao abrir o arquivo" << endl;
        exit(1);
    }

    string line;
    int row = 1;
    vector<thread> threads;

    while (getline(file, line)) {
        threads.emplace_back(processarLinhaTreinamento, line, row, classes, features);

        if (threads.size() >= thread::hardware_concurrency()) {
            for (auto& th : threads) {
                th.join();
            }
            threads.clear();
        }
        row++;
    }

    for (auto& th : threads) {
        th.join();
    }

    file.close();
}






