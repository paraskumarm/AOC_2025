#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
long long solve(const vector<vector<long long> >& numbers, const vector<char>& ops){
	long long res = 0;
	int rows = (int)numbers.size();
	if (rows == 0) return 0;
	int cols = (int)numbers[0].size();
	for (int i = 0; i < cols; ++i) {
		long long curr = (ops[i] == '*') ? 1LL : 0LL;
		if (ops[i] == '+') {
			for (int r = 0; r < rows; ++r) curr += numbers[r][i];
		} else if (ops[i] == '*') {
			for (int r = 0; r < rows; ++r) curr *= numbers[r][i];
		}
		res += curr;
	}
	return res;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	const string path = "input.txt"; // expects file in the same folder
	ifstream fin(path);
	if (!fin) {
		cerr << "Failed to open " << path << "\n";
		return 1;
	}

	vector<vector<string> > numbers;
	vector<char> ops;

	// Read all lines, last non-empty is operators
	vector<string> lines;
	string line;
	while (std::getline(fin, line)) {
		// trim trailing spaces
		while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r')) line.pop_back();
		if (!line.empty()) lines.push_back(line);
	}
	if (lines.empty()) {
		cerr << "Empty input file\n";
		return 1;
	}
	string opLine = lines.back();
	lines.pop_back();

	// Parse number rows
	// for (size_t r = 0; r < lines.size(); ++r) {
	// 	std::istringstream ls(lines[r]);
	// 	vector<long long> row;
	// 	long long x;
	// 	while (ls >> x) row.push_back(x);
	// 	if (row.empty()) {
	// 		cerr << "Row " << (r+1) << " has no numbers\n";
	// 		return 1;
	// 	}
	// 	// numbers.push_back(std::move(row));
	// }
	// Ensure all rows have equal columns
	// size_t cols = numbers[0].size();
	// for (size_t r = 1; r < numbers.size(); ++r) {
	// 	if (numbers[r].size() != cols) {
	// 		cerr << "Inconsistent column count: row " << (r+1) << " has " << numbers[r].size() << ", expected " << cols << "\n";
	// 		return 1;
	// 	}
	// }

	// Parse operators
	{
		std::istringstream ls(opLine);
		string tok;
		while (ls >> tok) {
			if (tok.size() != 1 || (tok[0] != '+' && tok[0] != '*' )) {
				cerr << "Invalid operator token: " << tok << "\n";
				return 1;
			}
			ops.push_back(tok[0]);
		}
		if (ops.size() != cols) {
			cerr << "Operators count " << ops.size() << " does not match columns " << cols << "\n";
			return 1;
		}
	}

	// Echo parsed input as a quick confirmation
	// cout << "Parsed numbers (3x4):\n";
	// for (const auto &row : numbers) {
	// 	for (size_t i = 0; i < row.size(); ++i) {
	// 		if (i) cout << ' ';
	// 		cout << row[i];
	// 	}
	// 	cout << '\n';
	// }
	// cout << "Operators: ";
	// for (size_t i = 0; i < ops.size(); ++i) {
	// 	if (i) cout << ' ';
	// 	cout << ops[i];
	// }
	// cout << '\n';

	cout << solve(lines, ops) << endl;
	return 0;
}