#ifndef TESTE_HPP
#define TESTE_HPP

#include <string>
#include <vector>
#include <map>
#include <tuple>

#define MAX_LINE 500

using namespace std;

class Teste {
public:
    Teste();
    void testando(const string& filename_input, const string& filename_output, vector<vector<int>>* map_classes,  map<tuple<int, int>,vector<int>>* map_features);
};

#endif 
