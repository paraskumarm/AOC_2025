#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <climits>
using namespace std;
/**
 *
        ..............
        .......#...#..
        ..............
        ..#....#......
        ..............
        ..#......#....
        ..............
        .........#.#..
        ..............

 *
 */
long long solve(vector<pair<int, int>> &points)
{
    long long res = 0;
    pair<long long,long long> bestP1 = {-1,-1}, bestP2 = {-1,-1};
    int n = points.size();
    // Initialize min/max trackers for each row (y) and column (x)
    vector<vector<int>> rows(10000, {INT_MAX, INT_MIN});
    vector<vector<int>> cols(10000, {INT_MAX, INT_MIN});
    //rows={{},{}}
    for (int i = 0; i < points.size(); i++) {
        int x = points[i].first;
        int y = points[i].second;
        rows[y][0] = min(rows[y][0], x);
        rows[y][1] = max(rows[y][1], x);

        cols[x][0] = min(cols[x][0], y);
        cols[x][1] = max(cols[x][1], y);
    }
    // for(int i=0;i<14;i++){
    //     cout<<"i-->"<<i<<" val-->"<<cols[i][0]<<"->"<<cols[i][1]<<endl;
    // }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            pair<long long, long long> p1 = points[i];
            pair<long long, long long> p2 = points[j];
            //wrt p1
                    if ((p2.first >= rows[p1.second][0] && p2.first <= rows[p1.second][1]) && (p2.second >= cols[p1.first][0] && p2.second <= cols[p1.first][1]))
            {
                
                // Rectangle extent is defined by extremes on p1's row and column
                        long long dx = p1.first - p2.first + 1;
                        long long dy = p1.second - p2.second + 1;
                        long long local_ans = abs(dx * dy);

                        // Debug log: variables contributing to decision
                        cout << "IF match: p1=(" << p1.first << "," << p1.second << ") p2=(" 
                             << p2.first << "," << p2.second << ") | rows[y]=[" 
                             << rows[p1.second][0] << "," << rows[p1.second][1] << "] cols[x]=[" 
                             << cols[p1.first][0] << "," << cols[p1.first][1] << "] dx=" << dx 
                             << " dy=" << dy << " area=" << local_ans << "\n";

                        if (local_ans > res) {
                            res = local_ans;
                            bestP1 = p1;
                            bestP2 = p2;
                }
            }
        }
    }
    if (bestP1.first != -1) {
        cout << "Best corners: (" << bestP1.first << "," << bestP1.second << ") and (" 
             << bestP2.first << "," << bestP2.second << ")\n";
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

    vector<pair<int,int>> points;
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
        int x, y;
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
