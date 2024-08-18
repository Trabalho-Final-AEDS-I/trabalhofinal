#ifndef HASH_HPP
#define HASH_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>


using namespace std;

class Hash{
public:
    Hash();
    void print(const tuple<int,int> &elemento);
    void train(const string &nome_endereco, vector<vector<tuple<int,int>>>* dados_geral, map<tuple<int,int>,vector<int>> *features,  vector<vector<int>> *classes);
    void test(const string &nome_endereco, vector<vector<tuple<int,int>>>* dados_geral, map<tuple<int,int>,vector<int>> *features,  vector<vector<int>> *classes);

};

#endif