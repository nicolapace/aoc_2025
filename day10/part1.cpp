#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <deque>

using namespace std;

uint16_t parse_objective(const string& objective_str) {
    int pow_of_two = objective_str.length()-1;
    uint16_t objective = 0;
    while ( pow_of_two >= 0 ) {
        if ( objective_str[pow_of_two] == '#' ) {
            objective += (1 << pow_of_two);
        }
        pow_of_two--;
    }
    return objective;
}

uint16_t parse_element(const string& element_str) {
    uint16_t element = 0;
    size_t prev_pos = 0;
    // cout << "Parsing element string: " << element_str << endl;
    auto pos = element_str.find(',');
    while( pos != string::npos ) {
        // cout << "Parsing substring: " << element_str.substr(prev_pos, pos) << endl;
        element += (1 << stoi( element_str.substr(prev_pos, pos) ));
        prev_pos = pos + 1;
        pos = element_str.find(',', prev_pos);
    }
    element += (1 << stoi( element_str.substr(prev_pos) ));
    return element;
}

vector<uint16_t> parse_elements(const string& elements_str) {
    vector<uint16_t> elements {};
    size_t left_pos = elements_str.find('(');
    while( left_pos != string::npos ) {
        size_t right_pos = elements_str.find(')', left_pos);
        string element_str = elements_str.substr(left_pos + 1, right_pos - left_pos - 1);
        elements.push_back( parse_element(element_str) );
        left_pos = elements_str.find('(', right_pos);
    }
    return elements;
}

uint16_t bfs(uint16_t objective, const vector<uint16_t>& elements) {
    
    unordered_set<uint16_t> visited {0};
    deque<pair<uint16_t, unordered_set<uint16_t>>> queue {{0, {}}};
    while ( !queue.empty() ) {
        uint16_t current = queue.front().first;
        unordered_set<uint16_t> history = queue.front().second;
        queue.pop_front();
        if ( current == objective ) {
            return history.size();
        }
        visited.insert(current);
        for ( const auto& element : elements ) {
            uint16_t next = current ^ element;
            if (next == objective ) {
                history.insert(element);
                cout << "Found objective with history: ";
                for ( const auto& h : history ) {
                    cout << to_string(h) << " ";
                }
                cout << endl << endl;
                return history.size();
            }
            if ( visited.find(next) == visited.end() ) {
                visited.insert(next);
                unordered_set<uint16_t> new_history = history;
                new_history.insert(element);
                queue.push_back({next, new_history});
            }
        }
    }
    
    return 0;
}

int main() {
	string line;
	ifstream input_file("input.txt");

    vector<pair<string, string>> pairs;
    long total = 0;
    while (getline(input_file, line)) {
        auto objective = parse_objective(line.substr(1, line.find(']') - 1));
        auto elements = parse_elements(line);
        
        cout << "Objective: " << to_string(objective) << ", Elements: ";
        for ( const auto& element : elements ) {
            cout << to_string(element) << " ";
        }
        cout << endl;

        uint16_t result = bfs(objective, elements);
        total += result;
    }
	// Close the file
	input_file.close();

    cout << "Total: " << total << endl;
}
