#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
int solve(string key,map<string, vector<string> > &graph){
    if(key=="out")return 1;
    int cnt=0;
    for(int i=0;i<graph[key].size();i++){
        cnt += solve(graph[key][i], graph);
    }
    return cnt;
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
  
    cout << solve("you",graph) << endl;
    return 0;
}