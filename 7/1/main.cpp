#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

long long solve(const vector<string>& grid){
	if (grid.empty()) { return 0; }

	int rows = (int)grid.size();
	int cols = (int)grid[0].size();

	int sRow = -1, sCol = -1;
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < (int)grid[r].size(); ++c) {
			if (grid[r][c] == 'S') { sRow = r; sCol = c; break; }
		}
		if (sRow != -1) break;
	}

	if (sRow == -1 || sCol == -1) { return 0; }

	long long splitCount = 0;
	struct State { int rowStart; int col; };
	queue<State> q;
	q.push({sRow, sCol});
	vector<vector<bool>> splitVisited(rows, vector<bool>(cols, false));

	while (!q.empty()) {
		State cur = q.front(); q.pop();
		int r = cur.rowStart + 1;
		while (r < rows) {
			if (cur.col < 0 || cur.col >= cols) break;
			char cell = grid[r][cur.col];
			if (cell == '^') {
				if (splitVisited[r][cur.col]) {
					break;
				}
				splitVisited[r][cur.col] = true;
				++splitCount;
				if (cur.col - 1 >= 0) q.push({r, cur.col - 1});
				if (cur.col + 1 < cols) q.push({r, cur.col + 1});
				break;
			}
			++r;
		}
	}

	return splitCount;
}

// Part 2: number of distinct timelines using many-worlds interpretation.
// From a state (rowStart, col), the particle moves downward until a splitter.
// On splitter at (r, col), it branches to (r, col-1) and (r, col+1) and continues below.
// Timelines can merge; memoize states to avoid double-counting.
struct Key { int rowStart; int col; };
struct KeyHash { size_t operator()(const Key& k) const noexcept { return ((size_t)k.rowStart << 20) ^ (size_t)(k.col + 100000); } };
struct KeyEq { bool operator()(const Key& a, const Key& b) const noexcept { return a.rowStart == b.rowStart && a.col == b.col; } };

long long dfs_timelines(const vector<string>& grid, int rowStart, int col, vector<vector<long long>>& memo, vector<vector<char>>& seen){
	int rows = (int)grid.size();
	int cols = (int)grid[0].size();
	if (col < 0 || col >= cols) return 1; // exits immediately out of bounds
	if (rowStart >= rows - 1) return 1;   // next step exits bottom
	if (memo[rowStart][col] != -1) return memo[rowStart][col];

	int r = rowStart + 1;
	while (r < rows) {
		if (col < 0 || col >= cols) return memo[rowStart][col] = 1;
		char cell = grid[r][col];
		if (cell == '^') {
			long long left = dfs_timelines(grid, r, col - 1, memo, seen);
			long long right = dfs_timelines(grid, r, col + 1, memo, seen);
			return memo[rowStart][col] = left + right;
		}
		++r;
	}
	return memo[rowStart][col] = 1; // exits bottom without hitting splitter
}

long long solve_quantum(const vector<string>& grid){
	if (grid.empty()) return 0;
	int rows = (int)grid.size();
	int cols = (int)grid[0].size();
	int sRow = -1, sCol = -1;
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < (int)grid[r].size(); ++c) {
			if (grid[r][c] == 'S') { sRow = r; sCol = c; break; }
		}
		if (sRow != -1) break;
	}
	if (sRow == -1 || sCol == -1) return 0;
	vector<vector<long long>> memo(rows, vector<long long>(cols, -1));
	vector<vector<char>> seen(rows, vector<char>(cols, 0));
	return dfs_timelines(grid, sRow, sCol, memo, seen);
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	vector<string> grid;
	string line;
	while (getline(cin, line)) {
		if (!line.empty() && (line.back() == '\r' || line.back() == '\n')) {
			while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
		}
		// Preserve even empty lines; they'll be padded later.
		grid.push_back(line);
	}
	// Pad lines to uniform width with '.' to represent empty space.
	size_t maxCols = 0;
	for (const auto& row : grid) maxCols = max(maxCols, row.size());
	if (maxCols == 0) { cout << 0 << "\n"; return 0; }
	for (auto& row : grid) if (row.size() < maxCols) row.append(maxCols - row.size(), '.');
	long long ans2 = solve_quantum(grid);
	cout << ans2 << "\n";
	return 0;
}