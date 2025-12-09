#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
long long par[100001] = {-1};
// long long __size[100001]={0};

long long findPar(long long x)
{
    if (par[x] != x)
        par[x] = findPar(par[x]); // path compression
    return par[x];
}
void __union(long long x, long long y)
{
    long long p1 = findPar(x);
    long long p2 = findPar(y);
    if (p1 != p2)
    {
        par[p2] = p1;
    }
}

void init_dsu(long long n)
{
    for (long long i = 0; i < n; ++i)
    {
        par[i] = i;
        // __size[i] = 1;
    }
}

double cost(long long i, long long j, vector<vector<long long>> &nums)
{
    return pow(nums[i][0] - nums[j][0], 2) + pow(nums[i][1] - nums[j][1], 2) + pow(nums[i][2] - nums[j][2], 2);
}
vector<long long> findShortestTwo(vector<vector<long long>> &nums, vector<vector<long long>> &vis)
{
    long long n = nums.size();
    long long i1 = -1;
    long long i2 = -1;
    // max heap
    //  priority_queue<vector<long long>, vector<vector<long long>>> pq;
    long long mini = __LONG_LONG_MAX__;
    for (long long i = 0; i < n; i++)
    {
        for (long long j = 0; j < n; j++)
        {
            if (i != j && vis[i][j] == 0)
            {

                double local_ans = cost(i, j, nums);
                if (mini > local_ans)
                {
                    mini = local_ans;
                    i1 = i;
                    i2 = j;
                }
            }
        }
    }

    return {i1, i2};
}
long long solve(vector<vector<long long>> &nums, long long k)
{
    long long res = 1;
    long long n = nums.size();
    vector<vector<long long>> vis(n, vector<long long>(n, 0));
    for (long long i = 0; i < k; i++)
    {
        vector<long long> ans = findShortestTwo(nums, vis);
        __union(ans[0], ans[1]);
        vis[ans[0]][ans[1]] = 1;
        vis[ans[1]][ans[0]] = 1;
        // cout<<ans[0]<<" "<<ans[1]<<endl;
    }
    // for (long long i = 0; i < nums.size(); i++)
    // {
    //     cout << "i->" << i << " rep->" << findPar(i) << "\n";
    // }

    // for (long long i = 0; i < nums.size(); i++)
    // {
    //     cout << findPar(i) <<" ";
    // }
    unordered_map<long long, long long> mp;

    for (long long i = 0; i < nums.size(); i++)
    {
        // cout<<par[i]<<" ";
        mp[findPar(findPar(i))]++;
    }
    // cout<<"res-->"<<endl;

    priority_queue<double,vector<double>,greater<double>>pq;
    for (long long i = 0; i < nums.size(); i++)
    {
        // cout << "i->"<<i<<" freq->" << mp[i] << endl;
        // ;
        if (mp[i] > 1)
        {
            pq.push(mp[i]);
            if(pq.size()>3)pq.pop();
            // cout<<"mp[i]->"<<mp[i]<<endl;;
            // cout<<res<<endl;
        }
    }
    while(!pq.empty()){
        res=res*pq.top();
        pq.pop();
    }

    return res;
};
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const string inputPath = "input.txt"; // relative to current directory
    ifstream in(inputPath);
    if (!in)
    {
        cerr << "Failed to open " << inputPath << "\n";
        return 1;
    }

    string line;
    long long grandTotal = 0;
    size_t lineNo = 0;
    vector<vector<long long>> input;
    while (getline(in, line))
    {
        ++lineNo;
        if (line.empty())
            continue;

        // Split by comma
        vector<long long> nums;
        string cur;
        stringstream ss(line);
        while (getline(ss, cur, ','))
        {
            // trim spaces
            auto l = cur.find_first_not_of(" \t\r\n");
            auto r = cur.find_last_not_of(" \t\r\n");
            if (l == string::npos)
                continue;
            string token = cur.substr(l, r - l + 1);
            try
            {
                long long v = stoll(token);
                nums.push_back(v);
            }
            catch (const exception &)
            {
                cerr << "Warning: non-long longeger token on line " << lineNo << ": '" << token << "'\n";
            }
        }

        if (nums.empty())
            continue;

        long long lineSum = 0;
        for (auto v : nums)
            lineSum += v;
        grandTotal += lineSum;
        input.push_back(nums);
    }

    // for(long long i=0;i<input.size();i++){
    //     for(long long j=0;j<input[i].size();j++){
    //         cout<<input[i][j]<<" ";
    //     }
    //     cout<<endl;

    init_dsu(input.size());
    cout << solve(input, 1000) << endl;
    // 2125138858919067648 not working
    return 0;
}
