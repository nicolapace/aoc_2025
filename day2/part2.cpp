#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

pair<string, string> parse_pair(const string &pair_str) {
    size_t split_idx = pair_str.find('-');
    string first = pair_str.substr(0, split_idx);
    string second = pair_str.substr(split_idx + 1);
    return {first, second};
}

bool is_fake(const long &number) {
    string num_str = to_string(number);
    int split {1};
    while (split <= num_str.length() / 2){
        string part0 = num_str.substr(0, split);
        string part1 = num_str.substr(split, min<int>( { split, (int)num_str.length()-split }) );
        int next_split = split;
        while (part0 == part1 && next_split < num_str.length()) {
            part1 = num_str.substr(next_split, min<int>( { split, (int)num_str.length()-next_split }) );
            next_split += split;
        }
        if (part0 == part1 && next_split == num_str.length()) {
            return true;
        }
        split++;
        next_split = split;
    }
    return false;
}

long count_fake_ids(const string &first, const string &second) {
    long start = stol(first);
    long end = stol(second);

    long fake_id_count = 0;

    while (start <= end)
    {
        if (is_fake(start)) {
            cout << "Found fake: " << start << endl;
            fake_id_count+=start;
        }
        start++;
    }
    return fake_id_count;
}

int main() {
	string line;
	ifstream input_file("input.txt");

    vector<pair<string, string>> pairs;
    while (getline(input_file, line)) {
        size_t start = 0;
        size_t pos;
        while ((pos = line.find(',', start)) != string::npos) {
            auto pair = line.substr(start, pos - start);
            pairs.push_back(parse_pair(pair));
            start = pos + 1;
        }
        // push the final token (or whole line if no comma)
        auto pair = line.substr(start);
        pairs.push_back(parse_pair(pair));
    }
	// Close the file
	input_file.close();
    long fake_ids = 0;
    for(auto &p : pairs) {
        cout << "Counting fake IDs for range: " << p.first << " - " << p.second << endl;
        fake_ids += count_fake_ids(p.first, p.second);
    }
    cout << "Total fake IDs: " << fake_ids << endl;
}
