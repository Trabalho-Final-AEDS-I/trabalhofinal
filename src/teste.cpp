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
#include <cassert>

using namespace std;
int findClasseMaisProxima(const vector<int>& result_combination, 
                          const vector<vector<int>>& map_classes) {
    int melhor_indice = -1;
    long unsigned int max_intersecao = 0;

    for (size_t i = 0; i < map_classes.size(); ++i) {
        vector<int> intersecao;
        set_intersection(result_combination.begin(), result_combination.end(),
                         map_classes[i].begin(), map_classes[i].end(),
                         back_inserter(intersecao));

        // Se a interseção for maior que a interseção máxima encontrada, atualize o índice
        if (intersecao.size() > max_intersecao) {
            max_intersecao = intersecao.size();
            melhor_indice = i;
        }
    }

    return melhor_indice;
}



void calcularSuporte(
    vector<int>combinacoes, 
    const vector<vector<int>>& classes, 
    const int features_size,
    map<vector<int>, double> *result
) 
{   
        //cout<<"combinacoes: "<<v.first.size()<<endl;
        for (const auto& c : classes) {
           // cout<<"classes: "<<c.size()<<endl;
            vector<int> intersecao;
            
            set_intersection(
                combinacoes.begin(), combinacoes.end(), 
                c.begin(), c.end(), 
                back_inserter(intersecao)
        );  

            int confianca = intersecao.size();
            
            if (confianca > 0) {
                double suporte = confianca / features_size;
                //cout<<"suporte: "<<suporte<<endl;

                if (result->find(c) == result->end()) {
                    (*result)[c] = 0;
                }
            (*result)[c] += suporte;
             //cout<<"resultado: "<<(*result)[c]<<endl;
            }
        }
}


int classificacao(map<tuple<int, int>, vector<int>> features, 
                  vector<vector<int>> map_classes, 
                  map<vector<tuple<int, int>>, vector<int>> *cache, 
                  const int features_size,
                   vector<tuple<int, int>> lista_elementos
                  ) {
    
    // Lista de elementos, onde cada elemento é um par (feature, valor)
    //int xxx = features_size;
    //xxx++;
    //map_classes.clear();
    // Variáveis de controle
    vector<int> linhas;
    vector<tuple<int, int>> combinacao_atual;
    map<vector<int>, double> result;

    // Total de combinações possíveis (2^n)
    int n = lista_elementos.size();
    long unsigned int total_combinacoes = 1 << n;

    // Loop por todas as combinações de features (de 1 a n)
    for (long unsigned int i = 1; i < total_combinacoes; ++i) {
        combinacao_atual.clear();
        linhas.clear();
        bool primeiro_elemento = true;

        // Gerar combinação de features atual
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {  // Verifica se o bit j está setado
                combinacao_atual.push_back(lista_elementos[j]);

                auto it = features.find(lista_elementos[j]);  // Procura a feature no mapa
                
                if (it != features.end()) {
                    if (it->second.empty()) {  // Se a lista de linhas estiver vazia, não há interseção possível
                        linhas.clear();
                        break;  // Interrompe o loop para esta combinação
                    }

                    if (primeiro_elemento) {
                        linhas = it->second;  // Primeira feature, inicializa a lista de linhas
                        cache->insert({combinacao_atual, linhas});  // Armazena no cache
                        primeiro_elemento = false;
                    } else {
                        // Verifica se já existe no cache
                        if (cache->find(combinacao_atual) != cache->end()) {
                            linhas = cache->at(combinacao_atual);  // Usa o cache
                        } else {
                            // Calcula a interseção das linhas
                            vector<int> temp;
                            set_intersection(linhas.begin(), linhas.end(),
                                             it->second.begin(), it->second.end(),
                                             back_inserter(temp));
                            linhas = temp;
                            cache->insert({combinacao_atual, linhas});  // Armazena no cache
                        }
                    }
                }
            }
        }

       // cout << "Combinação: ";
        //for (const auto& p : combinacao_atual) {
        //    cout << "{" << get<0>(p) << "," << get<1>(p) << "}, ";
        //}
        //cout << " -> Linhas: ";
       // for (const auto& linha : linhas) {
       //     cout << linha << ", ";
        //}
        //cout << endl;


        if (!linhas.empty()) {
            calcularSuporte(linhas, map_classes, features_size, &result);
        }
    }

    // Ordena os resultados e retorna o de maior suporte
    vector<pair<vector<int>, double>> result_vector(result.begin(), result.end());
    sort(result_vector.begin(), result_vector.end(), [](const pair<vector<int>, double>& a, const pair<vector<int>, double>& b) {
      return a.second > b.second;
    });

    if (!result_vector.empty()) {
        int indice_classe = findClasseMaisProxima(result_vector.front().first, map_classes);
        if (indice_classe != -1) {
            return indice_classe;
        } else {
            cerr << "Nenhuma classe semelhante encontrada!" << endl;
            return -1;  // Valor de erro caso não haja resultado
        }
    } else {
        cerr << "Nenhuma classe encontrada!" << endl;
        return -1;  // Valor de erro caso não haja resultado
    }

        //return -1;
    }


Teste::Teste(){

}

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
    int row = 1;
    int classe = 0;
    int loss = 0;
    int accuracy = 0;
    map<vector<tuple<int, int>>, vector<int>> cache;

    while (getline(file_input, line)) {
        vector<tuple<int, int>> list_line;
        map<tuple<int, int>, vector<int>> features;
        cout<<"Linha: "<<row<<endl;

        stringstream ss(line);
        string valor;
        int chave = 1;
        int numero_classe = 0;

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
 
        numero_classe = classificacao(features, *map_classes, &cache, (*map_features).size(), list_line);

    
        if (classe == numero_classe) {
            accuracy++;
        } else {
            loss++;
        }

        file_output << "Linha: " << row << " - Classe: " << classe << " - " << numero_classe << endl;

        if (row == MAX_LINE) {
            break;
        }
        row++;
    }

    file_output << "Acurácia: " << accuracy << " - Perda: " << loss << endl;

    file_output.close();
    file_input.close();
}
