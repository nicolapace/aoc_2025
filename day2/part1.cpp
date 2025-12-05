#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

pair<string, string> parse_pair(const string &pair_str) {
    size_t split_idx = pair_str.find('-');
    string first = pair_str.substr(0, split_idx);
    string second = pair_str.substr(split_idx + 1);
    // cout << "Parsed pair: " << first << ", " << second << endl;
    return {first, second};
}

long generate_next_mirrored(long number) {
    string num_str = to_string(number); 
    long len = num_str.length(); // should be always even
    string first_half = num_str.substr(0, (len + 1) / 2);
    string second_half = num_str.substr((len + 1) / 2);
        
    // std::reverse(rev_part.begin(), rev_part.end());
    if (stol(first_half) > stol(second_half)) {
        string palindrome {first_half};
        palindrome += first_half;
        return stol(palindrome);
    }
    
    long next_palindrome_part_long = stol(first_half) + 1;
    first_half = to_string(next_palindrome_part_long);
    string next_palindrome = to_string(next_palindrome_part_long);
    next_palindrome += first_half;

    return stol(next_palindrome);
}

bool is_mirrored(const long &number) {
    string num_str = to_string(number);
    long len = num_str.length();
    for (long i = 0; i < len / 2; ++i) {
        if (num_str[i] != num_str[len/2 + i]) {
            return false;
        }
    }
    return true;
}

long count_fake_ids(const string &first, const string &second) {
    long start = stol(first);
    long end = stol(second);

    long fake_id_count = 0;

    if (first.length() % 2 == 1) {
        start = pow(10, first.length());
    }
    
    while (start <= end)
    {
        if (is_mirrored(start)) {
            cout << "Found mirrored: " << start << endl;
            fake_id_count+=start;
        }
        start = generate_next_mirrored(start);
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
