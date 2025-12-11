#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

long long solve(vector<pair<long long, long long>> &points)
{
    long long res = 0;
    int n = points.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            long long dx = points[i].first - points[j].first+1;
            long long dy = points[i].second - points[j].second+1;
            long long local_ans = dx * dy;
            res = max(res, abs(local_ans));
        }
    }
    return res;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read from input.txt in the same directory
    ifstream fin("input.txt");
    if (!fin)
    {
        cerr << "Failed to open input.txt\n";
        return 1;
    }

    vector<pair<long long, long long>> points;
    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;
        // Trim whitespace
        auto trim = [](string &s)
        {
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == string::npos)
            {
                s.clear();
                return;
            }
            s = s.substr(a, b - a + 1);
        };
        trim(line);
        if (line.empty())
            continue;

        // Expect format like: 7,1 or possibly with spaces
        size_t comma = line.find(',');
        if (comma == string::npos)
        {
            cerr << "Invalid line (no comma): " << line << "\n";
            continue;
        }
        string a = line.substr(0, comma);
        string b = line.substr(comma + 1);
        trim(a);
        trim(b);
        long long x, y;
        try
        {
            x = stoll(a);
            y = stoll(b);
        }
        catch (...)
        {
            cerr << "Invalid numbers in line: " << line << "\n";
            continue;
        }
        points.emplace_back(x, y);
    }

    // Echo parsed points to stdout to confirm
    // cout << "Read " << points.size() << " points:\n";
    // for (auto &p : points) {
    // 	cout << p.first << "," << p.second << "\n";
    // }

    cout << solve(points) << endl;
    return 0;
}
