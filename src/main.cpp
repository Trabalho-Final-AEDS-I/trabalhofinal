#include "hash.hpp"

int main(){
    Hash hash;
    const string input_endereco = "dataset/poker-hand-training.data";
    const string output_endereco = "dataset/testeout.data";

    vector<vector<tuple<int,int>>> dados_geral;
    map<tuple<int,int>,vector<int>> features;

    vector<vector<int>> classes;


    hash.train(input_endereco,&dados_geral,&features,&classes);
/*
    cout<<"Features:\n";
    for(auto &i: features){
        cout<<"{"<<to_string(get<0>(i.first))<<","<<to_string(get<1>(i.first))<<"}: ";
        for(auto &j: i.second){
            cout<<to_string(j)<<", ";
        }
        cout<<endl; 
    }

    cout<<endl;
    cout<<"Classes:\n";
    int m = 0;
    for(auto &i : classes){
        cout<<to_string(m++)<<": ";
        for(auto &j: i){
            cout<<to_string(j)<<", ";
        }
        cout<<endl;
    }
*/
    hash.test(output_endereco,&dados_geral,&features,&classes);
    
    return 0;
}