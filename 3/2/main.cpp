#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

/*
434234234278
4

8
18181911112111


8 k =11
1 k=10
1 k=9
1 k =8
1 k=7
1 k=6
1 k=5
11111119, k=4


k->5
line->111111119

*/
string removePrevFromLine(string line, int i)
{
    string ans;
    for (int j = i + 1; j < line.size(); j++)
    {
        ans.push_back(line[j]);
    }
    return ans;
}
long long convertToLongLong(string s)
{
    long long ans = 0;
    for (int i = 0; i < s.size(); i++)
    {
        ans = ans * 10 + (s[i] - '0');
    }
    return ans;
}
long long findMax12(string line)
{
    string res;
    for (int k = 11; k >= 0; k--)
    {
        int n = line.size();
        // cout<<"line->"<<line<<endl<<endl;
        string candidate = line.substr(0, n - k);
        // cout << "candidate->" << candidate << endl;
        // cout<<"k->"<<k<<endl;

        if (candidate.size()==0){

            break;
        }
        
        int maxiIndx = 0;
        for (int i = 0; i < candidate.size(); i++)
        {
            if (candidate[i] > candidate[maxiIndx])
            {
                maxiIndx = i;
            }
        }
        // cout<<"maxidx->"<<maxiIndx<<endl<<endl<<endl;;
        res.push_back(candidate[maxiIndx]);
        line = removePrevFromLine(line, maxiIndx);
    }
    return convertToLongLong(res);
}
long long solve(vector<string> lines)
{
    long long ans = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        long long temp = findMax12(lines[i]);
        // cout<<temp<<endl;
        ans += temp;
    }
    return ans;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");
    if (!fin)
    {
        cerr << "Failed to open input.txt\n";
        return 1;
    }

    vector<string> lines;
    string line;
    while (getline(fin, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back(); // handle CRLF
        lines.push_back(line);
    }

    cout << solve(lines) << endl;
    return 0;
}
