#include "teste.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>

using namespace std;

int calcularSuporte(
    const vector<vector<int>>& combinacoes, 
    const vector<vector<int>>& classes, 
    double min_suporte,
    const vector<int>& features,
    const map<tuple<int, int>, vector<int>> map_features
) {
    map<int, double> result;

    for (const auto& v : combinacoes) {
        for (const auto& c : classes) {

            vector<int> intersecao;
            set_intersection(map_features.at({v}).begin(), map_features.at(v).end(), c.begin(), c.end(), back_inserter(intersecao));

            int confianca = intersecao.size();

            if (confianca > min_suporte) {

                double suporte = static_cast<double>(confianca) / static_cast<double>(features.size());
                
                int classe_index = &c - &classes[0];
                result[classe_index] += suporte;
            }
        }
    }

    auto melhor_resultado = max_element(result.begin(), result.end(), 
        [](const pair<int, double>& a, const pair<int, double>& b) {
            return a.second < b.second;
        });

    return melhor_resultado->first;
}

Teste::Teste() {}

void Teste::testando(const string &filename_input, const string &filename_output, 
                     vector<vector<int>>* map_classes, 
                     map<tuple<int, int>, vector<int>>* map_features) {
                        
    ifstream file_input(filename_input);
    ofstream file_output(filename_output);
    map_classes->clear();

    if (!file_input) {
        cerr << "Erro ao abrir o arquivo" << endl;
        exit(1);
    }
    if (!file_output) {
        cerr << "Erro ao abrir o arquivo de saída" << endl;
        exit(1);
    }

    string line;
    int row = 1;
    int classe = 0;
    int loss = 0;
    int accuracy = 0;

    while (getline(file_input, line)) {
        cout << "Linha: " << row << endl;
        vector<tuple<int, int>> list_line;
        map<tuple<int, int>, vector<int>> features;

        stringstream ss(line);
        string valor;
        int chave = 1;
        int numero_classe = 0;  // Inicializando corretamente
        
        while (getline(ss, valor, ',')) {
            if (ss.peek() == EOF) {
                numero_classe = stoi(valor);
                break;
            }

            tuple<int, int> elemento(chave++, stoi(valor));
            list_line.push_back(elemento);

            if(map_features->find(elemento) != map_features->end()) {
                features[elemento] = map_features->at(elemento);
            } else {
                features[elemento] = {};
            }
        }

        vector< vector<tuple<int, int>>> combinacoes;  // Para armazenar todas as combinações geradas

        for (long unsigned int i = 1; i <= COMBINATION_MAX_LINE; i++) {  // Começamos de 1 para evitar a combinação vazia
            vector<tuple<int, int>> combination;
            vector<int> lines;
            bool primeiro_elemento = true;
            map<vector<tuple<int, int>>, vector<int>> cache;


            for (long unsigned int j = 0; j < list_line.size(); j++) {

                if (i & (1 << j)) {  // Se o bit j estiver definido, incluímos o elemento correspondente
                    combination.push_back(list_line[j]);

                    if (primeiro_elemento) {
                        // Primeira entrada, adicionar diretamente
                        lines = features[list_line[j]];
                        cache[combination] = lines;
                        primeiro_elemento = false;
                    } 
                    else {                     
                        if (cache.find(combination) != cache.end()) {
                            // Se a combinação já existe, use o valor do dicionário
                            lines = cache.at(combination);
                        } 
                        else {
                            vector<int> insert = features[list_line[j]];

                            vector<int> temp;
                            set_intersection(lines.begin(), lines.end(), insert.begin(), insert.end(), back_inserter(temp));

                            lines = temp;
                            cache[combination] = lines;
                        }
                    }


                    combinacoes.push_back(combination);

                }
            }

            
            
            
            
            // Armazenar a combinação resultante
        }

        // Corrigindo a chamada para calcularSuporte
        double min_suporte = 0.7;  // Exemplo, ajuste conforme necessário
        if (!combinacoes.empty()) {
            classe = calcularSuporte(combinacoes, *map_classes, min_suporte, features[list_line[0]]);
            cout << "Classe: " << classe << endl;
        }

        if (classe == numero_classe) {
            accuracy++;
        } else {
            loss++;          
        }

        file_output << "Linha: " << row << " - Classe: " << classe << endl;

        if (row == MAX_LINE) {
            break;
        }

        row++;
    }

    file_output << "Acurácia: " << accuracy << " - Perda: " << loss << endl;

    file_output.close();
    file_input.close();
}
