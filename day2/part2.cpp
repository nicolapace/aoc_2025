#include <iostream>
#include <fstream>

std::pair<std::string, std::string> parse_pair(const std::string &pair_str) {
    size_t split_idx = pair_str.find('-');
    std::string first = pair_str.substr(0, split_idx);
    std::string second = pair_str.substr(split_idx + 1);
    return {first, second};
}

bool is_fake(const long &number) {
    std::string num_str = std::to_string(number);
    int split {1};
    while (split <= num_str.length() / 2){
        std::string part0 = num_str.substr(0, split);
        std::string part1 = num_str.substr(split, std::min<int>( { split, (int)num_str.length()-split }) );
        int next_split = split;
        while (part0 == part1 && next_split < num_str.length()) {
            part1 = num_str.substr(next_split, std::min<int>( { split, (int)num_str.length()-next_split }) );
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

long count_fake_ids(const std::string &first, const std::string &second) {
    long start = std::stol(first);
    long end = std::stol(second);

    long fake_id_count = 0;

    while (start <= end)
    {
        if (is_fake(start)) {
            std::cout << "Found fake: " << start << std::endl;
            fake_id_count+=start;
        }
        start++;
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
