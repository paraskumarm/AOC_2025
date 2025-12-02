#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;
/*
11-22,95-115,998-1012,1188511880-1188511890,222220-222224,
1698522-1698528,446443-446449,38593856-38593862,565653-565659,
824824821-824824827,2121212118-2121212124
*/

long long solvePart2(vector<vector<long long> > &v)
{
// cout<<"HI";
    long long ans=0;
    // cout<<v.size()<<" ";
    // 36151629247 not working
    unordered_map<long long,bool>mp;
    // 35947285815 not working
    for (int i = 0; i < (long long)v.size(); i++)
    {
        long long l=v[i][0];
        long long r=v[i][1];
        cout<<endl;
        cout<<l<<" "<<r<<endl;
     
        for (long long num = 1; num <= 999999; ++num)
        { 
            // for(int j=2;j<=6;j++){
                string s = to_string(num);
                string full=s;
                for(int k=0;k<=6;k++){
                    full+=s;
                    long long num = stoll(full);
                    if (num >= l && num <= r && mp[num] == false)
                    {
                        cout << "num->" << num << endl;
                        mp[num] = true;
                        ans += num;
                    }
                    if (num > r)
                        break;
                }

                
            // }
        }
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
    // Read entire file content (may be single line with commas)
    string content, line;
    while (getline(fin, line))
    {
        if (!content.empty())
            content += '\n';
        content += line;
    }

    // Split by commas and trim spaces
    vector<string> tokens;
    string cur;
    for (size_t i = 0; i < content.size(); ++i) {
        char ch = content[i];
        if (ch == ',') {
            // trim cur
            size_t start = cur.find_first_not_of(" \t\r\n");
            size_t end = cur.find_last_not_of(" \t\r\n");
            if (start != string::npos)
                tokens.push_back(cur.substr(start, end - start + 1));
            cur.clear();
        } else {
            cur.push_back(ch);
        }
    }
    // push last token
    size_t start = cur.find_first_not_of(" \t\r\n");
    size_t end = cur.find_last_not_of(" \t\r\n");
    if (start != string::npos) {
        tokens.push_back(cur.substr(start, end - start + 1));
    }

    // Debug-print tokens (each on its own line)
    vector<vector<long long > > vec;
    for (const auto &s : tokens)
    {
        // cout << s << "\n";
        int i = 0;
        long long num1 = 0;
        while (s[i] != '-')
        {
            num1 = num1 * 10 + s[i] - '0';
            i++;
        }
        i++;
        long long num2 = 0;
        while (i<s.size())
        {
            if (i >= s.size()) break; // Prevent out-of-bounds access
            num2 = num2 * 10 + s[i] - '0';
            i++;
        }
        vector<long long>t;
        t.push_back(num1);
        t.push_back(num2);
        vec.push_back(t);
    }
    // cout<<vec.size()<<endl;
    // cout<<"YO\n";
    sort(vec.begin(),vec.end());
    cout << solvePart2(vec) << endl;

    return 0;
}