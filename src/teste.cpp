#include "teste.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <iomanip> 
#include <thread>
#include <vector>
#include <mutex>
#include <map>
#include <set>

using namespace std;

mutex mtx; // Mutex para sincronização de acesso ao resultado

bool lsh(map<double, int>* map_lsh, 
         vector<tuple<int, int>> a, 
         vector<tuple<int, int>> b,  
         int* numero_classe,
         double* jaccard) {

    vector<tuple<int, int>> uniao; 
    vector<tuple<int, int>> interseccao; 

    set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(uniao));
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(interseccao)); 

    *jaccard = static_cast<double>(interseccao.size()) / uniao.size();
    *jaccard = std::round(*jaccard * 1000.0) / 1000.0;

    auto it = map_lsh->find(*jaccard);
    if (it != map_lsh->end()) {
        *numero_classe = it->second;
        return true;
    } else {
        return false;
    }
}

void calcularSuporte(
    vector<int> combinacoes, 
    const vector<vector<int>>& classes, 
    const int features_size,
    map<vector<int>, double> *result
) 
{   
    for (const auto& c : classes) {
        vector<int> intersecao;

        set_intersection(
            combinacoes.begin(), combinacoes.end(), 
            c.begin(), c.end(), 
            back_inserter(intersecao)
        );  

        int confianca = intersecao.size();
        
        if (confianca > 0) {
            double suporte = static_cast<double>(confianca) / features_size;

            lock_guard<mutex> lock(mtx); // Lock para garantir acesso seguro ao result
            if (result->find(c) == result->end()) {
                (*result)[c] = 0;
            }
            (*result)[c] += suporte;
        }
    }
}

void processarCombinacoes(long unsigned int inicio, long unsigned int fim,
                           map<tuple<int, int>, vector<int>> features, 
                           vector<vector<int>> map_classes,
                           map<vector<tuple<int, int>>, vector<int>> *cache,
                           const int features_size,
                           vector<tuple<int, int>> lista_elementos,
                           map<vector<int>, double> *result) {
    vector<int> linhas;
    vector<tuple<int, int>> combinacao_atual;

    for (long unsigned int i = inicio; i < fim; ++i) {
        combinacao_atual.clear();
        linhas.clear();
        bool primeiro_elemento = true;

        for (long unsigned int j = 0; j < lista_elementos.size(); ++j) {
            if (i & (1 << j)) {  
                combinacao_atual.push_back(lista_elementos[j]);

                auto it = features.find(lista_elementos[j]);  
                
                if (it != features.end()) {
                    if (it->second.empty()) {  
                        linhas.clear();
                        break;  
                    }

                    if (primeiro_elemento) {
                        linhas = it->second;  
                        cache->insert({combinacao_atual, linhas});  
                        primeiro_elemento = false;
                    } else {
                        auto it_cache = cache->find(combinacao_atual);
                        if (it_cache != cache->end()) {
                            linhas = it_cache->second;  
                        } else {
                            vector<int> temp;
                            set_intersection(linhas.begin(), linhas.end(),
                                             it->second.begin(), it->second.end(),
                                             back_inserter(temp));
                            linhas = temp;
                            cache->insert({combinacao_atual, linhas});  
                        }
                    }
                }
            }
        }

        if (!linhas.empty()) {
            calcularSuporte(linhas, map_classes, features_size, result);
        }
    }
}




int classificacao(map<tuple<int, int>, vector<int>> features, 
                  vector<vector<int>> map_classes, 
                  map<vector<tuple<int, int>>, vector<int>> *cache, 
                  const int features_size,
                  vector<tuple<int, int>> lista_elementos) {

    vector<thread> threads;
    map<vector<int>, double> result;
    long unsigned int n = lista_elementos.size();
    long unsigned int total_combinacoes = 1 << n;
    unsigned int num_threads = std::thread::hardware_concurrency(); // Número de threads suportadas
    long unsigned int chunk_size = total_combinacoes / num_threads;

    for (unsigned int i = 0; i < num_threads; ++i) {
        long unsigned int inicio = i * chunk_size;
        long unsigned int fim = (i == num_threads - 1) ? total_combinacoes : (i + 1) * chunk_size;

        threads.emplace_back(processarCombinacoes, inicio, fim, features, map_classes, cache, features_size, lista_elementos, &result);
    }

    for (auto& t : threads) {
        t.join();
    }

    // Ordena os resultados e retorna o de maior suporte
    vector<pair<vector<int>, double>> result_vector(result.begin(), result.end());
    sort(result_vector.begin(), result_vector.end(), [](const pair<vector<int>, double>& a, const pair<vector<int>, double>& b) {
        return a.second > b.second;
    });

    if (!result_vector.empty()) {
        auto it = find(map_classes.begin(), map_classes.end(), result_vector[0].first);
        int index = distance(map_classes.begin(), it);
        return index;
    } else {
        return -1; 
    }
}

Teste::Teste(){}

void Teste::testando(const string &filename_input, const string &filename_output, 
                     vector<vector<int>>* map_classes, 
                     map<tuple<int, int>, vector<int>>* map_features) {
                        
    ifstream file_input(filename_input);
    ofstream file_output(filename_output);

    if (!file_input) {
        cerr << "Erro ao abrir o arquivo" << endl;
        exit(1);
    }
    if (!file_output) {
        cerr << "Erro ao abrir o arquivo de saída" << endl;
        exit(1);
    }

    string line;
    map<vector<tuple<int, int>>, vector<int>> cache;
    map<double, int> map_lsh;

    int classe;
    int row = 1;
    int loss = 0;
    int accuracy = 0;

    // Criação da assinatura
    vector<tuple<int, int>> aux_assinatura;

    for(auto &i: *map_features){
        aux_assinatura.push_back(i.first);
    }
    sort(aux_assinatura.begin(), aux_assinatura.end());
    vector<tuple<int, int>> assinatura(aux_assinatura.end()-10, aux_assinatura.end());
   
    while (getline(file_input, line)) {
        vector<tuple<int, int>> list_line;
        map<tuple<int, int>, vector<int>> features;

        stringstream ss(line);
        string valor;
        int numero_classe;
        int chave = 1;

        while (getline(ss, valor, ',')) {
            if (ss.peek() == EOF) {
                classe = stoi(valor);
                break;
            }

            tuple<int, int> elemento(chave++, stoi(valor));
            list_line.push_back(elemento);

            if (map_features->find(elemento) != map_features->end()) {
                features[elemento] = map_features->at(elemento);
            } 
        }

        double jaccard;
        if(lsh(&map_lsh, assinatura, list_line, &numero_classe, &jaccard)){}
        else {
            numero_classe = classificacao(features, *map_classes, &cache, (*map_features).size(), list_line);
            if(jaccard > 0.7) {
                map_lsh[jaccard] = numero_classe;
            }
        }

        if (classe == numero_classe) {
            accuracy++;
        } else {
            loss++;
        }

        file_output << "Linha: " << row << " - Classe: " <<  numero_classe <<endl;

        if (row == MAX_LINE) {
            break;
        }
        row++;
    }

    double porcentagem =  static_cast<double>(accuracy) / (accuracy + loss);

    file_output << "Acertos: " << accuracy << " - Perda: " << loss << endl;
    file_output << "Acurácia: " << porcentagem * 100.00 << " %" << endl;

    file_output.close();
    file_input.close();
}