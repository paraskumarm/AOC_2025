#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
long long solve(const vector<string> &lines, const vector<char> &ops)
{
    long long res = 0;
    int rows = (int)lines.size();
    if (rows == 0)
        return 0;
    int cols = (int)lines[0].size();
    for (int i = 0; i < rows; i++)
    {
        cols = max(cols, (int)lines[i].size());
    }
    int opsi = 0;

    long long curr = 0;
    if (ops[opsi] == '*')
        curr = 1;
    for (int i = 0; i <= cols; ++i)
    {

        long long digit = 0;
        for (int row = 0; row < rows; row++)
        {
            char ch = lines[row][i];

            if (!ch || ch == ' ')
                continue;
            digit = digit * 10 + (ch - '0');
        }
        if (digit == 0)
        {
            res += curr;
            opsi++;
            curr = 0;
            if (ops[opsi] == '*')
                curr = 1;
            continue;
        }
        if (ops[opsi] == '+')
        {
            curr = curr + digit;
        }
        else
        {
            curr = curr * digit;
        }
    }
    return res;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const string path = "input.txt";
    ifstream fin(path);
    if (!fin)
    {
        cerr << "Failed to open " << path << "\n";
        return 1;
    }
    vector<char> ops;

    vector<string> lines;
    string line;
    while (std::getline(fin, line))
    {
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r'))
            line.pop_back();
        if (!line.empty())
            lines.push_back(line);
    }
    if (lines.empty())
    {
        cerr << "Empty input file\n";
        return 1;
    }
    string opLine = lines.back();
    lines.pop_back();

    // Parse operators
    {
        std::istringstream ls(opLine);
        string tok;
        while (ls >> tok)
        {
            if (tok.size() != 1 || (tok[0] != '+' && tok[0] != '*'))
            {
                cerr << "Invalid operator token: " << tok << "\n";
                return 1;
            }
            ops.push_back(tok[0]);
        }
    }

    cout << solve(lines, ops) << endl;

    return 0;
}