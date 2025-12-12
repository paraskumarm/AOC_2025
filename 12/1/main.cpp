#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;

struct Grid
{
    int cols, rows;
    vector<int> patternIndices;
};

struct PresentSlot
{
    long rows, cols, numPresents;

    bool fits() const
    {
        return numPresents <= (rows / 3) * (cols / 3);
    }
};

// ============= OLD COMPLEX LOGIC (Pattern Fitting) =============

vector<string> rotateBy90(const vector<string> &p)
{
    int n = p.size();
    vector<string> res(n, string(n, ' '));
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            res[j][n - 1 - i] = p[i][j];
        }
    }
    return res;
}

vector<string> mirrorOf(const vector<string> &p)
{
    vector<string> res = p;
    for (int i = 0; i < p.size(); i++)
    {
        reverse(res[i].begin(), res[i].end());
    }
    return res;
}

bool canFitAtPosition(int startI, int startJ, const vector<string> &box, const vector<string> &pattern)
{
    int patternRows = pattern.size();
    int patternCols = pattern[0].size();
    int boxRows = box.size();
    int boxCols = box[0].size();
    
    if (patternRows + startI > boxRows || patternCols + startJ > boxCols)
        return false;
    
    for (int i = 0; i < patternRows; i++)
    {
        for (int j = 0; j < patternCols; j++)
        {
            if (pattern[i][j] == '#' && box[i + startI][j + startJ] == '#')
                return false;
        }
    }
    return true;
}

void placePattern(int startI, int startJ, vector<string> &box, const vector<string> &pattern)
{
    int patternRows = pattern.size();
    int patternCols = pattern[0].size();
    
    for (int i = 0; i < patternRows; i++)
    {
        for (int j = 0; j < patternCols; j++)
        {
            if (pattern[i][j] == '#')
                box[i + startI][j + startJ] = '#';
        }
    }
}

void removePattern(int startI, int startJ, vector<string> &box, const vector<string> &pattern)
{
    int patternRows = pattern.size();
    int patternCols = pattern[0].size();
    
    for (int i = 0; i < patternRows; i++)
    {
        for (int j = 0; j < patternCols; j++)
        {
            if (pattern[i][j] == '#')
                box[i + startI][j + startJ] = '.';
        }
    }
}

bool canFitPatternsIntoBox(int patIdx, vector<string> &box, const vector<vector<string>> &patterns, 
                           vector<int> &patternIndice, const vector<vector<vector<string>>> &transformCache)
{
    // Find next pattern to place
    while (patIdx < patternIndice.size() && patternIndice[patIdx] == 0)
        patIdx++;
    
    if (patIdx == patternIndice.size())
    {
        return true;
    }
    
    int boxRows = box.size();
    int boxCols = box[0].size();
    
    // Try all 8 transformations
    for (int t = 0; t < 8; t++)
    {
        const vector<string> &pattern = transformCache[patIdx][t];
        int patternRows = pattern.size();
        int patternCols = pattern[0].size();
        
        // Early pruning: skip if pattern is too big
        if (patternRows > boxRows || patternCols > boxCols)
            continue;
        
        // Try all positions
        for (int i = 0; i <= boxRows - patternRows; i++)
        {
            for (int j = 0; j <= boxCols - patternCols; j++)
            {
                if (canFitAtPosition(i, j, box, pattern))
                {
                    placePattern(i, j, box, pattern);
                    patternIndice[patIdx]--;
                    
                    if (canFitPatternsIntoBox(patIdx, box, patterns, patternIndice, transformCache))
                    {
                        return true;
                    }
                    
                    patternIndice[patIdx]++;
                    removePattern(i, j, box, pattern);
                }
            }
        }
    }
    
    return false;
}

int solveWithPatternFitting(vector<Grid> &grids, const vector<vector<string>> &patterns)
{
    // Pre-compute all transformations for each pattern
    vector<vector<vector<string>>> transformCache(patterns.size());
    
    for (int i = 0; i < patterns.size(); i++)
    {
        transformCache[i].reserve(8);
        
        // 4 rotations
        vector<string> current = patterns[i];
        for (int j = 0; j < 4; j++)
        {
            transformCache[i].push_back(current);
            current = rotateBy90(current);
        }
        
        // 4 mirrored rotations
        current = mirrorOf(patterns[i]);
        for (int j = 0; j < 4; j++)
        {
            transformCache[i].push_back(current);
            current = rotateBy90(current);
        }
    }
    
    int res = 0;
    for (int i = 0; i < grids.size(); i++)
    {
        vector<string> box(grids[i].rows, string(grids[i].cols, '.'));
        if (canFitPatternsIntoBox(0, box, patterns, grids[i].patternIndices, transformCache))
        {
            res++;
        }
        cout<<"Processed grid "<<i<<endl;
    }
    return res;
}

vector<Grid> loadGridsAndPatterns(const string &filename, vector<vector<string>> &patterns)
{
    ifstream fin(filename);
    vector<Grid> grids;
    string line;

    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        // Check if it's a pattern header (like "0:" or "1:")
        if (line.find(':') != string::npos && line.find('x') == string::npos)
        {
            vector<string> pattern;
            pattern.reserve(3);
            // Read next 3 lines for this pattern
            for (int i = 0; i < 3; i++)
            {
                string patternLine;
                getline(fin, patternLine);
                pattern.push_back(patternLine);
            }
            patterns.push_back(pattern);
        }
        // Check if it's a grid configuration (like "4x4: 0 0 0 0 2 0")
        else if (line.find('x') != string::npos)
        {
            // Parse grid size and pattern indices
            size_t colonPos = line.find(':');
            string gridSize = line.substr(0, colonPos);
            string indices = line.substr(colonPos + 2);

            // Extract rows and cols
            size_t xPos = gridSize.find('x');
            int cols = stoi(gridSize.substr(0, xPos));
            int rows = stoi(gridSize.substr(xPos + 1));

            // Extract pattern indices
            vector<int> patternIndices;
            istringstream iss(indices);
            int idx;
            while (iss >> idx)
            {
                patternIndices.push_back(idx);
            }

            Grid grid{cols, rows, patternIndices};
            grids.push_back(grid);
        }
    }

    fin.close();
    return grids;
}

// ============= NEW SIMPLE LOGIC (Area Check) =============

vector<PresentSlot> loadPresents(const string &filename)
{
    ifstream fin(filename);
    vector<PresentSlot> slots;
    string line;

    while (getline(fin, line))
    {
        if (line.empty() || line.find(':') == string::npos || line.find('x') == string::npos)
            continue;

        // Parse "4x6: 1 2 3"
        size_t colonPos = line.find(':');
        string gridSize = line.substr(0, colonPos);
        string indices = line.substr(colonPos + 2);

        // Extract rows and cols
        size_t xPos = gridSize.find('x');
        long rows = stol(gridSize.substr(0, xPos));
        long cols = stol(gridSize.substr(xPos + 1));

        // Sum up all the pattern counts
        long numPresents = 0;
        istringstream iss(indices);
        int idx;
        while (iss >> idx)
        {
            numPresents += idx;
        }

        slots.push_back({rows, cols, numPresents});
    }

    fin.close();
    return slots;
}

int solveWithSimpleCheck(const vector<PresentSlot> &presentSlots)
{
    int numFits = 0;
    for (const auto &slot : presentSlots)
    {
        if (slot.fits())
        {
            numFits++;
        }
    }
    return numFits;
}

// ============= MAIN =============

int main()
{
    auto start = high_resolution_clock::now();
    cout << "Start time: " << duration_cast<milliseconds>(start.time_since_epoch()).count() << " ms" << endl;

    // Choose which method to use
    cout << "Choose method:" << endl;
    cout << "1. Simple area check (fast)" << endl;
    cout << "2. Pattern fitting (slow but accurate)" << endl;
    int choice;
    cin >> choice;

    int result = 0;
    
    if (choice == 1)
    {
        cout << "\n=== Using Simple Area Check ===" << endl;
        vector<PresentSlot> presentSlots = loadPresents("input.txt");
        result = solveWithSimpleCheck(presentSlots);
    }
    else if (choice == 2)
    {
        cout << "\n=== Using Pattern Fitting ===" << endl;
        vector<vector<string>> patterns;
        vector<Grid> grids = loadGridsAndPatterns("input.txt", patterns);
        result = solveWithPatternFitting(grids, patterns);
    }
    else
    {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "\nEnd time: " << duration_cast<milliseconds>(end.time_since_epoch()).count() << " ms" << endl;
    cout << "Time taken: " << duration.count() << " ms (" << duration.count() / 1000.0 << " seconds)" << endl;
    cout << "Result: " << result << endl;

    return 0;
}