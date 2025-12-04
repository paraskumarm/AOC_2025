#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
using namespace std;

int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};
int CountAdjacent(vector<string> lines, int m, int n)
{
    int cnt = 0;
    if(lines[m][n]=='.')return 0;
    for (int i = 0; i < 8; i++)
    {
            int X = m + dx[i];
            int Y = n + dy[i];
            if (Y >= 0 && Y < lines[0].size() && X >= 0 && X < lines.size())
            {
                if (lines[X][Y] == '@')
                {
                    cnt += 1;
                }
        }
    }
    return cnt;
}
int solve(vector<string> lines)
{
    int ans = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            if(lines[i][j]=='@'){
            int cnt= (CountAdjacent(lines, i, j));
            if(cnt<4){
                // lines[i][j]='x';
                ans += 1;
            }
        }
            
        }
        cout<<endl;
    }
    return ans;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto t0 = chrono::high_resolution_clock::now();

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
    auto t1 = chrono::high_resolution_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    cerr << "Runtime: " << ms << " ms\n";
    return 0;
}
