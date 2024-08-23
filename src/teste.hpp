#ifndef TESTE_HPP
#define TESTE_HPP

#include <vector>
#include <tuple>
#include <string>
#include <map>

#define COMBINATION_MAX_LINE (1 << 10)

#define MAX_LINE 100000

using namespace std;

class Teste {
public:
    Teste();
    void testando(const string &filename_input, const string &filename_output, vector<vector<int>>* classes, map<tuple<int,int>,vector<int>> *features);

};          
#endif