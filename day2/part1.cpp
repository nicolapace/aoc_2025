#include <iostream>
#include <fstream>
#include <cmath>

std::pair<std::string, std::string> parse_pair(const std::string &pair_str) {
    size_t split_idx = pair_str.find('-');
    std::string first = pair_str.substr(0, split_idx);
    std::string second = pair_str.substr(split_idx + 1);
    // std::cout << "Parsed pair: " << first << ", " << second << std::endl;
    return {first, second};
}

long generate_next_mirrored(long number) {
    std::string num_str = std::to_string(number); 
    long len = num_str.length(); // should be always even
    std::string first_half = num_str.substr(0, (len + 1) / 2);
    std::string second_half = num_str.substr((len + 1) / 2);
        
    // std::reverse(rev_part.begin(), rev_part.end());
    if (std::stol(first_half) > std::stol(second_half)) {
        std::string palindrome {first_half};
        palindrome += first_half;
        return std::stol(palindrome);
    }
    
    long next_palindrome_part_long = std::stol(first_half) + 1;
    first_half = std::to_string(next_palindrome_part_long);
    std::string next_palindrome = std::to_string(next_palindrome_part_long);
    next_palindrome += first_half;

    return std::stol(next_palindrome);
}

bool is_mirrored(const long &number) {
    std::string num_str = std::to_string(number);
    long len = num_str.length();
    for (long i = 0; i < len / 2; ++i) {
        if (num_str[i] != num_str[len/2 + i]) {
            return false;
        }
    }
    return true;
}

long count_fake_ids(const std::string &first, const std::string &second) {
    long start = std::stol(first);
    long end = std::stol(second);

    long fake_id_count = 0;

    if (first.length() % 2 == 1) {
        start = std::pow(10, first.length());
    }
    
    while (start <= end)
    {
        if (is_mirrored(start)) {
            std::cout << "Found mirrored: " << start << std::endl;
            fake_id_count+=start;
        }
        start = generate_next_mirrored(start);
    }
    
    return fake_id_count;
}

int main() {
	std::string myText;

	std::ifstream MyReadFile("input.txt");

    std::vector<std::pair<std::string, std::string>> pairs;
    while (std::getline(MyReadFile, myText)) {
        size_t start = 0;
        size_t pos;
        while ((pos = myText.find(',', start)) != std::string::npos) {
            auto pair = myText.substr(start, pos - start);
            pairs.push_back(parse_pair(pair));
            start = pos + 1;
        }
        // push the final token (or whole line if no comma)
        auto pair = myText.substr(start);
        pairs.push_back(parse_pair(pair));
    }
	// Close the file
	MyReadFile.close();

    long fake_ids = 0;
    for(auto &p : pairs) {
        std::cout << "Counting fake IDs for range: " << p.first << " - " << p.second << std::endl;
        fake_ids += count_fake_ids(p.first, p.second);
    }

    std::cout << "Total fake IDs: " << fake_ids << std::endl;

}
