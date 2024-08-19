#include "hash.hpp"

using namespace std;

void combinations(vector<vector<tuple<int, int>>>* dados_geral, map<tuple<int, int>, vector<int>>* features, vector<tuple<int, int>> lista_elementos, vector<vector<int>>* classes) {
    map<vector<tuple<int, int>>, double> suportes;
    int n = lista_elementos.size();
    //unsigned long int total_linhas = dados_geral->size();
    unsigned long int total_combinacoes = 1 << n;// Existem 2^n combinações possíveis
    
    vector<vector<int>>fase(*classes);
    fase.clear();
    vector<vector<tuple<int, int>>>data(*dados_geral);
    data.clear();


    for (long unsigned int i = 1; i < total_combinacoes; ++i) {
        vector<tuple<int, int>> combinacao_atual;
        vector<int> linhas;
        bool primeiro_elemento = true;
        
        // Etapa 2  - 1
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) { // Se o j-ésimo bit estiver setado, inclui o elemento na combinação
                combinacao_atual.push_back(lista_elementos[j]);
                if (features->find(lista_elementos[j]) != features->end()) {
                    if (primeiro_elemento) {
                        linhas = (*features)[lista_elementos[j]];
                        primeiro_elemento = false;
                    } else {
                        // Interseção de linhas
                        vector<int> intersect;
                        set_intersection(linhas.begin(), linhas.end(), (*features)[lista_elementos[j]].begin(), (*features)[lista_elementos[j]].end(), back_inserter(intersect));
                        linhas = intersect;
                    }
                }
            }
        }



        cout<<"Combinação: ";
        for(auto &p: combinacao_atual){
            cout<<"{"<<to_string(get<0>(p))<<","<<to_string(get<1>(p))<<"},";
        }
        cout<<   "     linha: ";
        for(auto &p: linhas){
            cout<<to_string(p)<<",";
        }
        cout<<endl;
    }

   
}



void Hash::print(const tuple<int,int> &elemento){
    cout<<"{"<<to_string(get<0>(elemento))<<","<<to_string(get<1>(elemento))<<"},";
}

Hash::Hash(){
}

void Hash::train(const string &nome_endereco, vector<vector<tuple<int,int>>>* dados_geral, map<tuple<int,int>,vector<int>> *features,  vector<vector<int>> *classes){

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
            tuple<int, int> elemento(numero_coluna, stoi(valor));
            lista_elementos.push_back(elemento);
            numero_coluna++;

            if (features->find(elemento)!= features->end()) {
                features->find(elemento)->second.push_back(numero_linha);
            } else {
                (*features)[elemento] = { numero_linha };
            }

        }


        int numero_classe = get<1>(lista_elementos.back());
        lista_elementos.pop_back();

        dados_geral->push_back(lista_elementos);

        if (numero_classe >= static_cast<int>(classes->size())) {
            classes->resize(numero_classe + 1);
        }
        (*classes)[numero_classe].push_back(numero_linha);


        lista_elementos.clear();
        numero_linha ++;
    }

    file.close();
}


void Hash::test(const string &nome_endereco, vector<vector<tuple<int,int>>>* dados_geral, map<tuple<int,int>,vector<int>> *features,  vector<vector<int>> *classes){
    
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
        int numero_coluna = 1;
        
        while(getline(aux_linha, valor, ',')){
            tuple<int, int> elemento(numero_coluna, stoi(valor));
            lista_elementos.push_back(elemento);
            numero_coluna++;
        }

        lista_elementos.pop_back();
        combinations(dados_geral, features, lista_elementos, classes);

    }
    file.close();
}


