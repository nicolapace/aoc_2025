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

int binary_search_bound(const vector<pair<long, long>> &db, const long &value) {
    size_t left = 0;
    size_t right = db.size();
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (db[mid].first < value) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int main() {
	string line;

	ifstream input_file("input.txt");
	long total {};
    
    vector<pair<long, long>> db{};
    // fill db
    while (getline(input_file, line)) {
        if (line.length() == 0) {
            break;
        }
        auto range = parse_pair(line);
        db.push_back(range);
    }
    sort(db.begin(), db.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    // check ingredients
    while (getline(input_file, line)) {
        auto ingedient = stol(line);
        //binary search the first element that is >= ingedient
        int i = 0;
        while (i < binary_search_bound(db, ingedient))
        {
            if (ingedient >= db[i].first && ingedient <= db[i].second) {
                cout << "Found in range: " << db[i].first << "-" << db[i].second << " for ingredient " << ingedient << endl;
                total += 1;
                break;
            }
            i++;
        }

	}
	input_file.close();

	cout << "Total: " << total << endl;
}
