<h1 align="center">Trabalho AEDS - LAC</h1>

<div align="center">
    <img align="center" height="20px" width="70px" src="https://img.shields.io/badge/c++-%2300599C.svg?logo=c%2B%2B&logoColor=white"/>
    <img align="center" height="20px" width="80px" src="https://img.shields.io/badge/VS%20Code-blue?logo=visual%20studio%20code"/>
    <img align="center" alt="CMake" src="https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black" />
</div>

##  Índice :scroll:

* [Trabalho](#trabalho)
  * [Treinamento](#treinamento)
  * [Teste](#teste)
* [Tabela Hash](#tabela-hash)
* [LAC](#lac)
* [Implementação](#implementação)
  * [LSH](#lsh)
  * [calcularSuporte](#calcularsuporte)
  * [classificação](#classificação)
  * [testando](#testando)
  * [processando](#processando)
* [Conclusão](#conclusão)



## Trabalho
Este trabalho foi proposto na disciplina de Algoritmos e Estruturas de Dados pelo professor Michel Pires da Silva do Centro Federal de Educação Tecnológica de Minas Gerais [GitHub](https://github.com/mpiress). O presente trabalho propõe a criação de um algoritmo de classificação, utilizando estruturas de dados e uma abordagem focada na utilização do algoritmo Lazy Associative Classification (LAC). O objetivo deste trabalho é desenvolver uma solução que seja eficiente e adequada às necessidades de um sistema de classificação em tempo real. Desenvolvendo nossa capacidade de implementação de tabelas hash e 
### Treinamento 
Na fase de treinamento é lido somente o primeiro arquivo [poker-hand-training.data](dataset/poker-hand-training.data) que contém os elementos das linhas e sua classe. Dessa forma para primeira parte é guardado em um vetor as classes e são montadas as Tuplas <coluna, elemento> que são guardadas em map<tuple<int,int> para depois ser acessado
### Teste 

## Tabela Hash 

## LAC

##  Implementação 
Nessa parte tem-se uma análise mais completa e dedicada a cada função utilizada no trabalho para o seu funcionamento. Assim sendo, explicando seus parâmetros, próposito, funcionamento e tempo gasto previsto. Sendo essas funções [LSH](lsh), [calcularSuporte](calcularsuporte), 
## LSH 
```Markdown
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

    if (0.1 < *jaccard && map_lsh->find(*jaccard) != map_lsh->end()) {
        *numero_classe = map_lsh->at(*jaccard);
        return true;
    } else {
        return false;
    }
}
```
## calcularSuporte 
```Markdown
void calcularSuporte(
    vector<int>combinacoes, 
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
                double suporte = confianca / features_size;

                if (result->find(c) == result->end()) {
                    (*result)[c] = 0;
                }
            (*result)[c] += suporte;
            }
        }
}
```
## classificação 
```Markdown
int classificacao(map<tuple<int, int>, vector<int>> features, 
                  vector<vector<int>> map_classes, 
                  map<vector<tuple<int, int>>, vector<int>> *cache, 
                  const int features_size,
                  vector<tuple<int, int>> lista_elementos
                  ) {

    vector<int> linhas;
    vector<tuple<int, int>> combinacao_atual;
    map<vector<int>, double> result;


    int n = lista_elementos.size();
    long unsigned int total_combinacoes = 1 << n;


    for (long unsigned int i = 1; i < total_combinacoes; ++i) {
        combinacao_atual.clear();
        linhas.clear();
        bool primeiro_elemento = true;

        for (int j = 0; j < n; ++j) {
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
                        if (cache->find(combinacao_atual) != cache->end()) {
                            linhas = cache->at(combinacao_atual);  
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
            calcularSuporte(linhas, map_classes, features_size, &result);
        }
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
        } 
    else {
            return -1; 
        }
    }
```

## testando 
```Markdown
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

    // criação da assinatura
    vector<tuple<int, int>> aux_assinatura;

    for(auto &i: *map_features){
        aux_assinatura.push_back(i.first);
    }
    sort(aux_assinatura.begin(), aux_assinatura.end());
    vector<tuple<int, int>> assinatura(aux_assinatura.begin(), aux_assinatura.begin() + 10);
   
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
        if(lsh(&map_lsh,assinatura, list_line, &numero_classe,&jaccard)){}
        else{
            numero_classe = classificacao(features, *map_classes, &cache, (*map_features).size(), list_line);
            if(jaccard > 0.1){
                map_lsh[jaccard] = numero_classe;
            }
        }

        if (classe == numero_classe) {
            accuracy++;
        } else {
            loss++;
        }

        file_output << "Linha: " << row << " - Classe: " <<  numero_classe << endl;

        if (row == MAX_LINE) {
            break;
        }
        row++;
    }

    file_output << "Acurácia: " << accuracy << " - Perda: " << loss << endl;

    file_output.close();
    file_input.close();
}
```
## processando 
```Markdown
void Treinamento::processando(const string &filename, vector<vector<int>> *classes, map<tuple<int,int>,vector<int>> *features){

    ifstream file(filename);
    if(!file){
        cerr<<"Erro ao abrir o arquivo"<<endl;
        exit(1);
    }

    string line;
    int row = 1;
    while(getline(file,line)){
        stringstream ss(line);
        string valor;
        int chave = 1;
        int numero_classe;
    
        while(getline(ss, valor, ',')){
            if (ss.peek() == EOF) {
                numero_classe = stoi(valor);
                if (numero_classe >= static_cast<int>(classes->size())) {
                    classes->resize(numero_classe + 1);
                }

                (*classes)[numero_classe].push_back(row);
            }
            else{
                tuple<int, int> elemento(chave++, stoi(valor));
                (*features)[elemento].push_back(row);
            }
        }
        row ++;
    }

    file.close();
}
```
## Conclusão 

## Compilação :hammer_and_wrench:
Para executar o programa foi utilizado um arquivo Makefile que realiza o processo de compilação e execução. Arquivo o qual requer as seguintes diretrizes de execução:

| Comando                |  Função                                                                                               |                     
| -----------------------| ------------------------------------------------------------------------------------------------------|
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                            |
|  `make`                | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build               |
|  `make run`            | Executa o programa da pasta build após a realização da compilação                                     |


## Contatos :e-mail:



