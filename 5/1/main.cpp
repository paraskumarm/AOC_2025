
using namespace std;

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

/*

3-5 
10-14 i
16-20
12-18

1 
5 + 
8 
11 +
17 j
32


*/

int solve(vector<pair<long long, long long>> ranges, vector<long long> values){
    sort(ranges.begin(),ranges.end());
    sort(values.begin(), values.end());
    int i=0;
    int j=0;
    int cnt=0;
    while(i<ranges.size() && j<values.size()){
        if(values[j]>=ranges[i].first && values[j]<=ranges[i].second){
            cnt+=1;
            j+=1;
        }else{
            if(ranges[i].first>values[j])j++;
            else if (ranges[i].second < values[j]) i++;
            else {

            }
        }
    }

    return cnt;
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

    vector<pair<long long, long long> > ranges;
    vector<long long> values;

    string line;
    bool readingRanges = true;
    while (getline(fin, line))
    {
        // trim whitespace
        auto start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos)
        { // empty line switches to values section
            readingRanges = false;
            continue;
        }
        auto end = line.find_last_not_of(" \t\r\n");
        string s = line.substr(start, end - start + 1);

        if (readingRanges)
        {
            size_t dash = s.find('-');
            if (dash == string::npos)
                continue; // skip malformed
            long long a = stoll(s.substr(0, dash));
            long long b = stoll(s.substr(dash + 1));
            ranges.emplace_back(a, b);
        }
        else
        {
            values.push_back(stoll(s));
        }
    }

    // Quick echo to verify parsing
    // cout << "Ranges:" << '\n';
    // for (auto &p : ranges)
    //     cout << p.first << "-" << p.second << '\n';
    // cout << "Values:" << '\n';
    // for (auto &v : values)
    //     cout << v << '\n';

    cout<<solve(ranges, values)<<endl;
    return 0;
}