#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

struct Row
{
    string pattern;             // e.g., ".##."
    vector<vector<int> > groups; // each group is a list of indices
    vector<int> numbers;        // final numbers in {}
};

vector<int> parseGroup(const string &s)
{
    vector<int> result;
    if (s.empty() || s == "()")
        return result;

    string nums = s.substr(1, s.length() - 2); // remove ()
    stringstream ss(nums);
    string token;
    while (getline(ss, token, ','))
    {
        if (!token.empty())
        {
            result.push_back(stoi(token));
        }
    }
    return result;
}

vector<int> parseNumbers(const string &s)
{
    vector<int> result;
    if (s.empty() || s == "{}")
        return result;

    string nums = s.substr(1, s.length() - 2); // remove {}
    stringstream ss(nums);
    string token;
    while (getline(ss, token, ','))
    {
        if (!token.empty())
        {
            result.push_back(stoi(token));
        }
    }
    return result;
}

Row parseLine(const string &line)
{
    Row row;
    stringstream ss(line);
    string token;

    // Parse pattern [.##.]
    ss >> token;
    row.pattern = token.substr(1, token.length() - 2); // remove []

    // Parse groups until we hit {
    while (ss >> token)
    {
        if (token[0] == '{')
        {
            // Parse final numbers
            string remaining;
            getline(ss, remaining);
            row.numbers = parseNumbers(token + remaining);
            break;
        }
        else
        {
            row.groups.push_back(parseGroup(token));
        }
    }

    return row;
}

int doDFS(int i, Row row, string temp)
{
    if (i == row.groups.size())
    {
        // cout<<row.pattern<<" "<<temp<<endl;
        if (temp == row.pattern){
            return 0;
        }
        return 1e9;
    }
    else
    {
        int ans1 = doDFS(i + 1, row, temp);
        for (int j = 0; j < row.groups[i].size(); j++)
        {
            int idx = row.groups[i][j];
            temp[idx] = temp[idx] == '#' ? '.' : '#';
        }
        int ans2 = 1 + doDFS(i + 1, row, temp);
        for (int j = 0; j < row.groups[i].size(); j++)
        {
            int idx = row.groups[i][j];
            temp[idx] = temp[idx] == '#' ? '.' : '#';
        }
        return min(ans1, ans2);
    }
}
int findMinimumOperations(Row row)
{
    int size=row.pattern.size();

    string temp;
    for(int i=0;i<size;i++){
        temp.push_back('.');
    }
    return doDFS(0, row, temp);
}

int solve(vector<Row> rows)
{
    int res = 0;
    for (int i = 0; i < rows.size(); i++)
    {
        int op = findMinimumOperations(rows[i]);
        cout<<op<<endl;
        res = res + op;
    }
    return res;
}
int main()
{
    ifstream fin("input.txt");
    if (!fin)
    {
        cerr << "Failed to open input.txt\n";
        return 1;
    }

    vector<Row> rows;
    string line;
    while (getline(fin, line))
    {
        if (!line.empty())
        {
            rows.push_back(parseLine(line));
        }
    }

    // // Print parsed data to verify
    // for (size_t i = 0; i < rows.size(); i++)
    // {
    //     cout << "Row " << i << ":\n";
    //     cout << "  Pattern: " << rows[i].pattern << "\n";
    //     cout << "  Groups (" << rows[i].groups.size() << "):\n";
    //     for (size_t j = 0; j < rows[i].groups.size(); j++)
    //     {
    //         cout << "    Group " << j << ": (";
    //         for (size_t k = 0; k < rows[i].groups[j].size(); k++)
    //         {
    //             if (k > 0)
    //                 cout << ",";
    //             cout << rows[i].groups[j][k];
    //         }
    //         cout << ")\n";
    //     }
    //     cout << "  Numbers: {";
    //     for (size_t j = 0; j < rows[i].numbers.size(); j++)
    //     {
    //         if (j > 0)
    //             cout << ",";
    //         cout << rows[i].numbers[j];
    //     }
    //     cout << "}\n\n";
    // }

    cout << solve(rows) << endl;

    return 0;
}