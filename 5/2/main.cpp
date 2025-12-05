
using namespace std;

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>


long long solve(vector<pair<long long, long long>> ranges)
{
    sort(ranges.begin(), ranges.end());

    vector<pair<long long, long long>> merge;
    long long prev_end=-1;
    long long prev_start=-1;
     for (int i = 0; i < ranges.size(); i++)
    {
        long long start = ranges[i].first;
        long long end = ranges[i].second;

        if (start>prev_end){
            if (prev_start!=-1)
                merge.push_back({prev_start, prev_end});
            prev_start=start;
            prev_end=end;
        }
        else if (start <= prev_end)
        {
            if (end > prev_end) prev_end=end;
        }
    }
    merge.push_back({prev_start, prev_end});
    long long ans=0;
    for(int i=0;i<merge.size();i++){
        ans += (merge[i].second - merge[i].first+1);
    }
    return ans;
}
// 350774409766790 not working
// 361615643045059
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    using clock_t = chrono::steady_clock;
    auto t0_total = clock_t::now();

    ifstream fin("input.txt");
    if (!fin)
    {
        cerr << "Failed to open input.txt\n";
        return 1;
    }

    vector<pair<long long, long long>> ranges;
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
   
    }

    // Time only the solve phase
    auto t0 = clock_t::now();
    auto result = solve(ranges);
    auto t1 = clock_t::now();
    auto us = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    cout << result << endl;
    cerr << "Solve runtime: " << us << " us\n";

    // Also log total runtime
    auto t1_total = clock_t::now();
    auto ms_total = chrono::duration_cast<chrono::milliseconds>(t1_total - t0_total).count();
    cerr << "Total runtime: " << ms_total << " ms\n";
    return 0;
}