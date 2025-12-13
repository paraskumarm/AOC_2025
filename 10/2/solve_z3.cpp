#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <z3++.h>

using namespace std;
using namespace z3;

struct Row
{
    string pattern;
    vector<vector<int>> groups;
    vector<int> numbers;
};

vector<int> parseGroup(const string &s)
{
    vector<int> result;
    if (s.empty() || s == "()")
        return result;

    string nums = s.substr(1, s.length() - 2);
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

    string nums = s.substr(1, s.length() - 2);
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

    ss >> token;
    row.pattern = token.substr(1, token.length() - 2);

    while (ss >> token)
    {
        if (token[0] == '{')
        {
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

int solveMachine(const Row &row)
{
    try
    {
        context ctx;
        optimize opt(ctx);

        int numButtons = row.groups.size();
        int numCounters = row.numbers.size();

        // Create integer variables for each button (number of presses)
        vector<expr> buttons;
        for (int i = 0; i < numButtons; i++)
        {
            buttons.push_back(ctx.int_const(("b" + to_string(i)).c_str()));
            // Each button must be pressed >= 0 times
            opt.add(buttons[i] >= 0);
        }

        // For each counter, sum of button presses affecting it must equal target
        for (int counterIdx = 0; counterIdx < numCounters; counterIdx++)
        {
            expr_vector contributions(ctx);
            
            for (int buttonIdx = 0; buttonIdx < numButtons; buttonIdx++)
            {
                // Check if this button affects this counter
                bool affects = false;
                for (int idx : row.groups[buttonIdx])
                {
                    if (idx == counterIdx)
                    {
                        affects = true;
                        break;
                    }
                }
                
                if (affects)
                {
                    contributions.push_back(buttons[buttonIdx]);
                }
            }

            if (contributions.size() > 0)
            {
                opt.add(sum(contributions) == row.numbers[counterIdx]);
            }
            else
            {
                // No button affects this counter - target must be 0
                if (row.numbers[counterIdx] != 0)
                {
                    return -1; // Impossible
                }
            }
        }

        // Minimize total button presses
        expr_vector allButtons(ctx);
        for (const auto &b : buttons)
        {
            allButtons.push_back(b);
        }
        opt.minimize(sum(allButtons));

        // Solve
        if (opt.check() == sat)
        {
            model m = opt.get_model();
            int total = 0;
            for (const auto &b : buttons)
            {
                total += m.eval(b).get_numeral_int();
            }
            return total;
        }
        else
        {
            return -1; // No solution
        }
    }
    catch (const z3::exception &e)
    {
        cerr << "Z3 Error: " << e.what() << endl;
        return -1;
    }
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

    int total = 0;
    for (const auto &row : rows)
    {
        int result = solveMachine(row);
        cout << result << endl;
        if (result > 0)
        {
            total += result;
        }
    }

    cout << total << endl;

    return 0;
}
