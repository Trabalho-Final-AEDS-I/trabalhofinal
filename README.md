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
* [Similaridade Jaccard](#similaridade-jaccard)
* [LAC](#lac)
* [Implementação](#implementação)
  * [LSH](#lsh)
  * [CalcularSuporte](#calcularsuporte)
  * [Classificação](#classificação)
  * [Testando](#testando)
  * [Processando](#processando)
* [Conclusão](#conclusão)
* [Contatos](#contatos)


## Trabalho
Este trabalho foi proposto na disciplina de Algoritmos e Estruturas de Dados pelo professor Michel Pires da Silva([GitHub](https://github.com/mpiress)) do Centro Federal de Educação Tecnológica de Minas Gerais. O presente trabalho propõe a criação de um algoritmo de classificação, utilizando estruturas de dados e uma abordagem focada na utilização do algoritmo Lazy Associative Classification (LAC). O objetivo deste trabalho é desenvolver uma solução que seja eficiente e adequada às necessidades de um sistema de classificação. Desenvolvendo nossa capacidade de implementação de tabela hash para uma execução mais rápida e de construção de métodos para uma acurácia melhor. Sendo dividido em duas partes que se complementam [Treinamento](#treinamento) e [Teste](#teste)

### Treinamento 
Na fase de treinamento é lido somente o primeiro arquivo [poker-hand-training.data](dataset/poker-hand-training.data) que contém as linhas que serão transformadas em Features e classes. Dessa forma, o vetor de entrada do treinamento é, primeiramente, adaptado para o formato de Tupla, que consiste em várias Features <coluna, elemento da coluna> e o número da classe<int> da linha no final, para que seja processado posteriormente. 

Após a formação das Tuplas, essas são processadas e adaptadas ao formato de Features para servirem em um modelo <chave, valor>, dentro de uma tabela de Features. Assim cada Feature desta tabela aponta para as linhas em que ela se encontra, uma vez que pode haver Features iguais em linhas diferentes, assim como será mostrado na figura. Após isso, as Features e suas respectivas linhas são guardadas em map <tuple<int,int> para depois ser acessado novamente.
Nessa parte final, as classes também são processadas, sendo guardadas em um vetor de vetores, associando cada classe às suas respectivas linhas, sendo também um modelo <chave,valor>.

Esse processo é exemplificado na figura abaixo:

<p align="center">
  <img
    src="https://cdn.discordapp.com/attachments/1280286232215027817/1280326964837875754/treinamento.png?ex=66d7ace7&is=66d65b67&hm=b918e7d0d3cdce8fea53822d8790aed4588b6fcee4f607fe78d599f355183962&"
    width="750"
    height="400"
    />
</p>

### Teste 
Na fase de teste, o segundo arquivo, [poker-hand-testing.data](dataset/poker-hand-testing.data), é lido para avaliar a eficácia do modelo treinado. Este arquivo contém novas instâncias que precisam ser classificadas, e o processo de teste se inicia pela leitura dessas instâncias. Cada linha do arquivo de teste representa uma entrada que será convertida em Features no formato <coluna, elemento>, da mesma maneira que foi feito durante a fase de treinamento.

Após essa conversão, a última coluna de cada linha, que indica a classe verdadeira da instância, é separada para servir como referência na avaliação da precisão do modelo. O processo de classificação começa com o algoritmo LAC (Lazy Associative Classification) utilizando as estruturas de dados (map de features e vetor de classes) construídas durante o treinamento.

Para cada nova instância, o algoritmo busca correspondências nas features já conhecidas e tenta identificar padrões que permitam prever a classe correta. A classe prevista pelo modelo é então comparada com a classe real da instância. Os resultados dessa comparação são registrados para determinar a precisão do modelo: acertos aumentam a contagem de acurácia, enquanto erros aumentam a contagem de perdas.

Ao final do processo, o número da linha e a classe atribuída pelo modelo são gravados no arquivo de saída output.txt. Este arquivo serve como um log detalhado da fase de teste, permitindo a análise dos resultados.


##  Similaridade Jaccard
A similaridade de Jaccard é uma medida estatística usada para comparar a semelhança e a diversidade de conjuntos de amostras.  
Ele é definido como o tamanho da interseção dos conjuntos dividido pelo tamanho da união dos conjuntos:  
Fórmula: J(A,B) = |A ∩ B| / |A ∪ B|

<p align="center">
  <img
    src="https://cdn.discordapp.com/attachments/1280286232215027817/1280320235659858081/laccard.png?ex=66d7a6a3&is=66d65523&hm=e73ca72a217c2780229dfd5d479068bdb388f8d239d97da131cf9a9395b3fdb0&"
    width="750"
    height="400"
    />
</p>

### Interpretação do Índice de Jaccard
O índice de Jaccard retorna um valor entre 0 e 1, onde:

- **0**: Indica que os conjuntos não têm elementos em comum.
- **1**: Indica que os conjuntos são idênticos.
- Um valor entre 0 e 1 indica uma similaridade parcial. Quanto mais próximo de 1, maior a similaridade entre os conjuntos.

Ele é usado para medir rapidamente quão similares são duas entradas de dados. Se a similaridade for maior que 0.7 no nosso caso, o algoritmo assume que as entradas são suficientemente parecidas para pertencer à mesma classe, o que permite uma classificação rápida sem passar pelo processo completo do LAC, potencialmente economizando tempo de processamento.


## LAC
Lazy Associative Classification (LAC) é uma técnica que realiza a classificação de dados utilizando uma abordagem "preguiçosa". Em vez de construir um modelo de classificação completo durante o treinamento, o LAC simplesmente armazena associações entre características dos dados e suas classes.

Dessa forma na fase de treinamento, o LAC processa um arquivo de dados para criar duas tabelas principais: uma tabela de Features, que associa combinações de coluna e valor a linhas de dados, e uma tabela de classes, que organiza as instâncias conforme suas classes.

Durante a fase de teste, quando uma nova instância é recebida, o LAC compara suas características com a tabela de Features para encontrar linhas associadas. Em seguida, realiza uma análise combinatória das Features para calcular o suporte e a confiança de cada combinação, determinando a classe da nova instância com base no maior suporte.

Para otimizar a performance, foi utilizado técnicas de cache e análise combinatória eficiente, para buscar um bom funcionamento e velocidade mesmo que com grandes volumes de dados.


##  Implementação 
Nessa parte tem-se uma análise mais completa e dedicada à cada função utilizada no trabalho para o seu funcionamento, explicando seus parâmetros, próposito, funcionamento e tempo gasto previsto, sendo essas funções [LSH](#lsh), [calcularSuporte](#calcularsuporte), [Classificação](#classificação), [Testando](#testando) e [Processando](#processando).


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


    if (0.7 < *jaccard && map_lsh->find(*jaccard) != map_lsh->end()) {
        *numero_classe = map_lsh->at(*jaccard);
        return true;
    } else {
        return false;
    }
}
```
Propósito: Essa função verifica se a similaridade de Jaccard entre dois conjuntos é suficiente para associar um determinado item a uma classe conhecida. Se a similaridade for acima de um determinado limiar e a classe correspondente for encontrada em um mapa, a função retorna true e define numero_classe para a classe correspondente.

Parâmetros:
- map_lsh: Mapa que relaciona a similaridade de Jaccard às classes.
- a, b: Dois vetores de tuplas representando as características dos elementos que estão sendo comparados.
- numero_classe: Ponteiro para um inteiro onde será armazenada a classe correspondente, se encontrada.
- jaccard: Ponteiro para um valor double onde será armazenada a similaridade de Jaccard calculada.

Funcionamento:
- Calcula a união e a interseção dos conjuntos a e b.
- Calcula a similaridade de Jaccard entre a e b.
- Se a similaridade for maior que 0,1 e a similaridade existir no map_lsh, a função atribui numero_classe com a classe correspondente e retorna true.


## CalcularSuporte 
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
            double suporte = static_cast<double>(confianca) / features_size;

            if (result->find(c) == result->end()) {
                (*result)[c] = 0;
            }
            (*result)[c] += suporte;
        }
    }
}
```
Propósito: Essa função calcula o suporte de um conjunto de combinações de elementos, comparando-o com classes existentes.

Parâmetros:
- combinacoes: Vetor de inteiros representando combinações de elementos.
- classes: Vetor de vetores de inteiros que representam as classes.
- features_size: Tamanho das características.
- result: Mapa onde os resultados dos cálculos de suporte são armazenados.

Funcionamento:
- Para cada classe, calcula a interseção entre as combinações e os elementos da classe.
- Se a interseção não for vazia, calcula o suporte (a confiança) e adiciona ao resultado.


## Classificação 
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
    } else {
        return -1; 
    }
}
```
Propósito: Essa função realiza a classificação de um conjunto de características com base nas interseções dessas características com um conjunto de classes conhecidas. O objetivo é encontrar a classe que melhor corresponde às características dadas.

Parâmetros:

- features: Mapa que mapeia tuplas de características a vetores de inteiros.
- map_classes: Vetor de vetores que representam as classes.
- cache: Mapa que armazena combinações previamente calculadas para otimizar futuras operações.
- features_size: Tamanho das características.
- lista_elementos: Vetor de tuplas que representam os elementos a serem classificados.

Funcionamento:

- Gera todas as combinações possíveis dos elementos em lista_elementos.
- Para cada combinação, calcula a interseção com os vetores de características e armazena no cache para otimizar futuras consultas.
- Calcula o suporte de cada combinação e armazena no mapa result.
- Ordena os resultados pelo suporte e retorna a classe que tem o maior suporte.


## Testando 
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

    double porcentagem =  static_cast<double>(accuracy) / (accuracy+loss);

    file_output << "Acertos: " << accuracy << " - Perda: " << loss << endl;
    file_output << "Acurácia: " << porcentagem*100.00 <<" %"<< endl;

    file_output.close();
    file_input.close();
}
```
### Classe Teste e Método testando
Propósito: A classe Teste contém métodos para realizar o teste do modelo de classificação. O método testando processa um arquivo de entrada, classifica cada linha de dados e salva os resultados em um arquivo de saída.

Funcionamento:

- Leitura de Arquivos: Abre o arquivo de entrada (filename_input) e cria o arquivo de saída (filename_output).
- Inicialização: Cria uma assinatura baseada nas características fornecidas.
- Para cada linha do arquivo de entrada, extrai as características e a classe real.
- Calcula a similaridade de Jaccard usando a função lsh para determinar se a classe pode ser identificada com base nessa métrica.
- Se não puder ser identificada com lsh, a função classificacao é chamada para determinar a classe usando o método de combinação de interseções.
- Calcula a acurácia e a perda comparando a classe prevista com a classe real.
- Escreve os resultados no arquivo de saída.


## Processando 
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
A função *Treinamento::processando* é responsável por processar um arquivo de entrada que contém dados para o treinamento de um modelo. Ela lê os dados do arquivo, organiza-os em classes e características, e os armazena em estruturas apropriadas para serem usados posteriormente no treinamento ou em previsões.

Parâmetros:
- filename: O nome do arquivo que contém os dados de entrada para o treinamento.
- classes: Um ponteiro para um vetor de vetores de inteiros. Cada vetor interno representa uma classe e armazena os números das linhas que pertencem a essa classe.
- features: Um ponteiro para um mapa que mapeia tuplas de inteiros (características) para vetores de inteiros. Cada vetor armazena os números das linhas em que a característica aparece.

### Funcionamento 
1. Abertura do Arquivo:
    - A função tenta abrir o arquivo especificado pelo nome filename usando um objeto ifstream.
    - Se o arquivo não puder ser aberto, uma mensagem de erro é exibida e o programa é encerrado.

2. Leitura Linha por Linha:
    - A função lê o arquivo linha por linha usando getline.
    - Cada linha representa uma instância de dados (um conjunto de características seguido por uma classe).

3. Processamento de Cada Linha:
    - A linha é dividida em valores separados por vírgulas usando um stringstream.
    - chave é inicializada em 1 e é usada para criar tuplas de características.

4. Classificação e Características:
    1. Classe:
        - O último valor em cada linha é a classe (numero_classe).
        - Se o número da classe (numero_classe) for maior ou igual ao tamanho atual do vetor classes, o vetor é redimensionado para acomodar essa nova classe.
        - O número da linha (row) é então adicionado ao vetor correspondente à classe em classes.
    2. Características:
        - Todos os valores anteriores ao último na linha são características.
        - Para cada característica, uma tupla é criada usando a chave e o valor da característica.
        - Essa tupla é usada como chave no mapa features, e o número da linha é adicionado ao vetor correspondente no mapa.

5. Incremento de Linha:
    - O número da linha (row) é incrementado a cada iteração do loop principal para acompanhar a linha atual que está sendo processada.

6. Fechamento do Arquivo:
    - O arquivo é fechado após o término do processamento.


## Conclusão 
Neste trabalho, foi implementado o algoritmo Lazy Associative Classification (LAC)


notou-se que a implementação de uma tabela hash para busca de dados foi primordial para a diminuição do tempo de execução.




## Compilação :hammer_and_wrench:
Para executar o programa foi utilizado um arquivo Makefile que realiza o processo de compilação e execução. Arquivo o qual requer as seguintes diretrizes de execução:

| Comando                |  Função                                                                                               |                     
| -----------------------| ------------------------------------------------------------------------------------------------------|
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                            |
|  `make`                | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build               |
|  `make run`            | Executa o programa da pasta build após a realização da compilação                                     |


## Contatos
<div align="center"> 
    
| Participante           |  Contato  |                     
| -----------------------| ----------|
|  Anderson Rodrigues Dos Santos | andersonifnmg.info@gmail.com :email: |
|  Élcio Costa Amorim Neto | elcioamorim12@gmail.com :email: |
|  Humberto Henrique Lima Cunha | humberto17henrique@gmail.com  :email: |
|  Kauã Lucas De Jesus Silva | kaualucas396@gmail.com :email: |
|  Sergio Henrique Quedas Ramos | sergiohenriquequedasramos@gmail.com :email: |  

</div>


