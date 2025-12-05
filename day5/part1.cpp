#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

std::pair<long, long> parse_pair(const std::string &pair_str) {
    size_t split_idx = pair_str.find('-');
    long first = std::stol(pair_str.substr(0, split_idx));
    long second = std::stol(pair_str.substr(split_idx + 1));
    // std::cout << "Parsed pair: " << first << ", " << second << std::endl;
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
	std::string line;

	std::ifstream MyReadFile("input.txt");
	long total {};
    
    vector<pair<long, long>> db{};
    // fill db
    while (std::getline(MyReadFile, line)) {
        if (line.length() == 0) {
            break;
        }
        auto range = parse_pair(line);
        db.push_back(range);
    }
    std::sort(db.begin(), db.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    // check ingredients
    while (std::getline (MyReadFile, line)) {
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

	std::cout << "Total: " << total << std::endl;
	// Close the file
	MyReadFile.close();

}
