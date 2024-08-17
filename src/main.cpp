#include "CSV.hpp"

int main(){
    CSV csv;
    const string nome_endereco = "dataset/teste2.data";
    vector<vector<tuple<int,int>>> dados_geral;
    vector<tuple<int,int>> features;
    vector<vector<int>> classes;

    csv.readCSV(nome_endereco,&dados_geral,&features,&classes);

    cout<<"Features: ";
    for(auto &i: features){
        csv.print(i);
    }
    cout<<endl<<endl;
    cout<<"Classes:\n";
    int m = 0;
    for(auto &i : classes){
        cout<<to_string(m++)<<": ";
        for(auto &j: i){
            cout<<to_string(j)<<", ";
        }
        cout<<endl;
    }
    return 0;
}