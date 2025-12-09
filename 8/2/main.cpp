#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
long long par[100001] = {-1};

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
    }
}

long long cost(long long i, long long j, const vector<vector<long long>> &nums)
{
    long long dx = nums[i][0] - nums[j][0];
    long long dy = nums[i][1] - nums[j][1];
    long long dz = nums[i][2] - nums[j][2];
    return dx*dx + dy*dy + dz*dz;
}
vector<long long> findShortestFromParent(vector<vector<long long>> &nums, int i)
{
    long long n = nums.size();
    long long i1 = -1;
    long long i2 = -1;
    // max heap
    //  priority_queue<vector<long long>, vector<vector<long long>>> pq;
    long long mini = __LONG_LONG_MAX__;

    for (long long j = 0; j < n; j++)
    {
        if (i != j)
        {

            long long local_ans = cost(i, j, nums);
            if (mini > local_ans)
            {
                mini = local_ans;
                i1 = i;
                i2 = j;
            }
        }
    }

    return {i1, i2};
}

vector<long long> findShortestTwo(vector<vector<long long>> &nums, vector<vector<long long>> &vis)
{
    long long n = nums.size();
    long long i1 = -1;
    long long i2 = -1;
    long long mini = __LONG_LONG_MAX__;
    for (long long i = 0; i < n; i++)
    {
        for (long long j = i + 1; j < n; j++)
        {
            if (vis[i][j] == 0)
            {
                long long local_ans = cost(i, j, nums);
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

long long solve(vector<vector<long long>> &nums)
{
    int n = (int)nums.size();
    if (n == 0)
        return 0;
    // Build all edges with squared distance
    struct Edge
    {
        int u, v;
        long long w;
    };
    vector<Edge> edges;
    edges.reserve((long long)n * (n - 1) / 2);
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            edges.push_back({i, j, cost(i, j, nums)});
        }
    }
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b)
         { return a.w < b.w; });
    init_dsu(n);
    int comps = n;
    int lastU = -1, lastV = -1;
    for (const auto &e : edges)
    {
        long long pu = findPar(e.u);
        long long pv = findPar(e.v);
        if (pu != pv)
        {
            __union(pu, pv);
            lastU = e.u;
            lastV = e.v;
            if (--comps == 1)
                break;
        }
    }
    if (lastU == -1)
        return 0;
    // Multiply X coordinates of last connected pair
    long long x1 = nums[lastU][0];
    long long x2 = nums[lastV][0];
    return x1 * x2;
}


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
    cout << solve(input) << endl;
    // 2125138858919067648 not working
    return 0;
}
