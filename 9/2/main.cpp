#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <climits>
#include <queue>
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

    //------START-------
    long long res=0;
    int numrows=0;
    int numcols=0;
    for(int i=0;i<points.size();i++){
        // cout << points[i].second<<" ";
        // cout << points[i].first << " ";
        // cout<<endl;
        numrows = max(numrows, points[i].second);
        numcols = max(numcols, points[i].first);
    }

    vector<string> board(numrows + 2, string(numcols + 2, '.'));

    for (int i = 0; i < points.size(); i++)
    {
        int colNum=points[i].first;
        int rowNum= points[i].second;
        board[rowNum][colNum] = '#';
    }
    // Connect consecutive points with green tiles (following input order)
    for (int i = 0; i < points.size(); i++) {
        int nextIdx = (i + 1) % points.size();  // Wrap around to first
        int x1 = points[i].first, y1 = points[i].second;
        int x2 = points[nextIdx].first, y2 = points[nextIdx].second;
        
        // Fill line between consecutive points
        if (x1 == x2) {  // Same column - vertical line
            int minY = min(y1, y2);
            int maxY = max(y1, y2);
            for (int y = minY; y <= maxY; y++) {
                if (board[y][x1] != '#')
                    board[y][x1] = 'O';
            }
        } else if (y1 == y2) {  // Same row - horizontal line
            int minX = min(x1, x2);
            int maxX = max(x1, x2);
            for (int x = minX; x <= maxX; x++) {
                if (board[y1][x] != '#')
                    board[y1][x] = 'O';
            }
        }
    }
    
    cout << "looping1......" << endl;
    
    // Flood fill from outside to mark exterior cells as 'X' (iterative to avoid stack overflow)
    vector<vector<bool>> visited(board.size(), vector<bool>(board[0].size(), false));
    queue<pair<int,int>> q;
    
    // Start from all edges
    for (int i = 0; i < board.size(); i++) {
        if (board[i][0] != '#' && board[i][0] != 'O') {
            q.push({i, 0});
            visited[i][0] = true;
        }
        if (board[i][board[0].size()-1] != '#' && board[i][board[0].size()-1] != 'O') {
            q.push({i, board[0].size()-1});
            visited[i][board[0].size()-1] = true;
        }
    }
    for (int j = 0; j < board[0].size(); j++) {
        if (board[0][j] != '#' && board[0][j] != 'O') {
            q.push({0, j});
            visited[0][j] = true;
        }
        if (board[board.size()-1][j] != '#' && board[board.size()-1][j] != 'O') {
            q.push({board.size()-1, j});
            visited[board.size()-1][j] = true;
        }
    }
    
    // BFS to mark all exterior cells
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};
    
    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();
        board[y][x] = 'X';
        
        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            
            if (ny >= 0 && ny < board.size() && nx >= 0 && nx < board[0].size() &&
                !visited[ny][nx] && board[ny][nx] != '#' && board[ny][nx] != 'O') {
                visited[ny][nx] = true;
                q.push({ny, nx});
            }
        }
    }
    
    // Mark all non-exterior, non-green, non-red cells as interior (green)
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] == '.') {
                board[i][j] = 'O';  // Interior cell
            } else if (board[i][j] == 'X') {
                board[i][j] = '.';  // Restore exterior to '.'
            }
        }
    }
    
    cout << "looping2......" << endl;
    
    // Build 2D prefix sum for O(1) rectangle checking
    int boardRows = board.size();
    int boardCols = board[0].size();
    vector<vector<int>> prefix(boardRows, vector<int>(boardCols, 0));
    
    for (int i = 0; i < boardRows; i++) {
        for (int j = 0; j < boardCols; j++) {
            prefix[i][j] = (board[i][j] != '.' ? 1 : 0);
            if (i > 0) prefix[i][j] += prefix[i-1][j];
            if (j > 0) prefix[i][j] += prefix[i][j-1];
            if (i > 0 && j > 0) prefix[i][j] -= prefix[i-1][j-1];
        }
    }

    cout << "looping5......" << endl;
    pair<long long,long long> bestP1 = {-1,-1}, bestP2 = {-1,-1};
    int n = points.size();
    
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            int minX = min(points[i].first, points[j].first);
            int maxX = max(points[i].first, points[j].first);
            int minY = min(points[i].second, points[j].second);
            int maxY = max(points[i].second, points[j].second);
            
            // Use prefix sum to check if rectangle is filled in O(1)
            int sum = prefix[maxY][maxX];
            if (minY > 0) sum -= prefix[minY-1][maxX];
            if (minX > 0) sum -= prefix[maxY][minX-1];
            if (minY > 0 && minX > 0) sum += prefix[minY-1][minX-1];
            
            int expectedArea = (maxY - minY + 1) * (maxX - minX + 1);
            
            if (sum == expectedArea) {
                long long area = (long long)expectedArea;
                if (area > res) {
                    res = area;
                    bestP1 = points[i];
                    bestP2 = points[j];
                }
            }
        }
    }
    // if (bestP1.first != -1) {
    //     cout << "Best corners: (" << bestP1.first << "," << bestP1.second << ") and (" 
    //          << bestP2.first << "," << bestP2.second << ")\n";
    // }
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
