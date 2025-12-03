#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int findMaxPair (string line){
    int end=line.size()-1;
    int curr_maxi=line[end]-'0';
    int ans=0;
    end--;
    while(end>=0){
        ans = max(ans, (line[end] - '0') * 10 + curr_maxi);
        curr_maxi = max(curr_maxi,line[end]-'0');
        end--;
    }
    return ans;
}
int solve(vector<string> lines)
{
    int ans=0;
    for(int i=0;i<lines.size();i++){
        ans+=findMaxPair(lines[i]);
    }
    return ans;
}
int main()
{
    ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ifstream fin("input.txt");
	if (!fin) {
		cerr << "Failed to open input.txt\n";
		return 1;
	}

	vector<string> lines;
	string line;
	while (getline(fin, line)) {
		if (!line.empty() && line.back() == '\r') line.pop_back(); // handle CRLF
		lines.push_back(line);
	}

	cout<<solve(lines)<<endl;
	return 0;
}
