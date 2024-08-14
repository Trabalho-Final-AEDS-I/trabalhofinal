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
    void readCSV(const string &nome_endereco);
};

#endif