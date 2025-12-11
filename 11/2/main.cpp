#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
using namespace std;

map<tuple<string, int, int>, long long> memo;

long long solve(string key,map<string, vector<string> > &graph,int p1,int p2){
    auto state = make_tuple(key, p1, p2);
    if(memo.find(state) != memo.end()){
        return memo[state];
    }
    
    if(key=="out"){
        if(p1&&p2)
        return memo[state] = 1;
        else return memo[state] = 0;
    }
    long long cnt=0;
    if(key=="dac")p1=1;
    if(key=="fft")p2=1;
    for(int i=0;i<graph[key].size();i++){
        cnt += solve(graph[key][i], graph,p1,p2);
    }
    return memo[state] = cnt;
} int main()
{
    ifstream fin("input.txt");
    string line;
    map<string, vector<string> > graph;
    
    while(getline(fin, line)){
        size_t colonPos = line.find(':');
        string key = line.substr(0, colonPos);
        string valuesStr = line.substr(colonPos + 2); // Skip ": "
        
        vector<string> values;
        stringstream ss(valuesStr);
        string value;
        while(ss >> value){
            values.push_back(value);
        }
        
        graph[key] = values;
    }
    
    int src=-1;
    // Print the parsed data
    for(auto& [key, values] : graph){
        cout << key << ": ";
        for(int i = 0; i < values.size(); i++){
            cout << values[i];
            if(i < values.size() - 1) cout << " ";
        }
        cout << endl;
    }
    
    fin.close();

    cout << solve("svr", graph, 0,0 ) << endl;
    return 0;
}