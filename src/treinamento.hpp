#ifndef TREINAMENTO_HPP
#define TREINAMENTO_HPP

#include <vector>
#include <tuple>
#include <string>
#include <map>

using namespace std;

class Treinamento {
    public:
        Treinamento();
        void processando(const string &filename, vector<vector<int>>* classes, map<tuple<int,int>,vector<int>> *features);
        void printarValores(vector<vector<int>> classes, map<tuple<int,int>,vector<int>> features) const;
};

#endif