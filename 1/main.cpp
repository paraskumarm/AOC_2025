#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// Day1 Part2
int solve(vector<string> lines)
{
    int curr = 50;
    int cnt = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        string s = lines[i];
        // cout<<s<<endl;
        char dir = s[0];
        int num = 0;
        for (int j = 1; j < lines[i].size(); j++)
        {
            num = num * 10 + lines[i][j] - '0';
        }
        // cout<<num<<" ";.
        if (dir == 'L')
        {
            int prev = curr;
            curr = (curr - num); //% 100;
            int plus = 0;
            if (curr <= 0)
            {
                int t = -curr;
                if (prev == 0)
                {
                    plus = t / 100;
                }
                else
                {
                    plus = (t / 100) + 1;
                }
                cnt = cnt + plus;
                curr = curr % 100;
            }
            // cout << "+" << plus << endl;
        }
        else
        {
            curr = (curr + num);
            int plus = curr / 100;
            // cout<<"+"<<plus<<endl;
            cnt = cnt + plus;
            curr = curr % 100;
        }
        curr = ((curr % 100) + 100) % 100;

        // cout<<curr<<endl<<endl;;
    }
    return cnt;

    // -18 82 +1
    //  52 52 +0
    // 100 0  +1
    // - 5 95
    // 155 55
    // 0 0
    // -1 99
    // 0 0
    // 14 14
    // - 68 32
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const string path = "/Users/paraskumarmahour/Documents/mydoc/personal/Coding/adventOfCode2025/1/input.txt";
    ifstream fin(path);
    if (!fin)
    {
        cerr << "Failed to open file: " << path << "\n";
        return 1;
    }
    vector<string> lines;
    string line;
    while (getline(fin, line))
    {
        lines.push_back(line);
    }

    cout << solve(lines) << endl;
    return 0;
}