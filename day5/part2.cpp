#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

pair<long, long> parse_pair(const string &pair_str) {
    size_t split_idx = pair_str.find('-');
    long first = stol(pair_str.substr(0, split_idx));
    long second = stol(pair_str.substr(split_idx + 1));
    // cout << "Parsed pair: " << first << ", " << second << endl;
    return {first, second};
}

int main() {
	string line;
	long total {};
    
    vector<pair<long, long>> db{};
    // fill db
	ifstream input_file("input.txt");
    while (getline(input_file, line)) {
        if (line.length() == 0) {
            break;
        }
        auto range = parse_pair(line);
        db.push_back(range);
    }
    // Close the file
	input_file.close();
    sort(db.begin(), db.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    vector<pair<long, long>> merged_db{};
    for (const auto& range : db) {
        if (merged_db.empty()) {
            merged_db.push_back(range);
        } else if (range.first <= merged_db.back().second) {
            merged_db.back() = pair<long, long>{min(merged_db.back().first, range.first), max(merged_db.back().second, range.second)};
        } else {
            merged_db.push_back(range);
        }
    }
    for (const auto& range : merged_db) {
        cout << "(" << range.first << "-" << range.second << ")" << endl;
        total += (range.second - range.first + 1);
    }
    cout << "Total: " << total << endl;
}
