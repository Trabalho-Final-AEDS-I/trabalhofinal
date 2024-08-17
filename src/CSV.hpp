#ifndef CSV_HPP
#define CSV_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <sstream>

using namespace std;

class CSV{
public:
    CSV();
    void print(const tuple<int,int> &elemento);
    void readCSV(const string &nome_endereco, vector<vector<tuple<int,int>>>* dados_geral, vector<tuple<int,int>> *features,     vector<vector<int>> *classes);
};

#endif