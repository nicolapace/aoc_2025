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

vector<uint8_t> parse_objective(const string& objective_str_orig) {
    string objective_str = objective_str_orig;
    vector<uint8_t> objective {};
    auto comma = objective_str.find(',');
    while ( comma != string::npos ) {
        objective.push_back( stoi(objective_str.substr(0, comma)) );
        objective_str = objective_str.substr(comma + 1);
        comma = objective_str.find(',');
    }
    objective.push_back( stoi(objective_str) );
    return objective;
}

vector<uint8_t> sum_vectors(const vector<uint8_t>& a, const vector<uint8_t>& b) {
    vector<uint8_t> result(a.size(), 0);
    for ( size_t i = 0; i < a.size(); i++ ) {
        result[i] = (a[i] + b[i]);
    }
    // cout << "Sum of vectors: ";
    // for ( const auto& r : a ) {
    //     cout << to_string(r) << " ";
    // }
    // cout << " + ";
    // for ( const auto& r : b ) {
    //     cout << to_string(r) << " ";
    // }
    // cout << " = ";
    // cout << "Summed vectors to: ";
    // for ( const auto& r : result ) {
    //     cout << to_string(r) << " ";
    // }
    // cout << endl;
    return result;
}

string hash_vector(const vector<uint8_t>& vec) {
    string hash;
    for ( const auto& val : vec ) {
        hash += to_string(val) + ",";
    }
    return hash;
}

bool equals_vector(const vector<uint8_t>& a, const vector<uint8_t>& b) {
    if ( a.size() != b.size() ) {
        return false;
    }
    for ( size_t i = 0; i < a.size(); i++ ) {
        if ( a[i] != b[i] ) {
            return false;
        }
    }
    return true;
}

bool vector_no_more_valid(const vector<uint8_t>& vec, const vector<uint8_t>& objective) {
    for ( size_t i = 0; i < vec.size(); i++ ) {
        if ( vec[i] > objective[i] ) {
            return true;
        }
    }
    return false;
}

vector<uint8_t> parse_element(const string& element_str, ssize_t objective_size) {
    uint16_t element = 0;
    size_t prev_pos = 0;
    vector<uint8_t> element_vec(objective_size, 0);
    // cout << "Parsing element string: " << element_str << endl;
    auto pos = element_str.find(',');
    while( pos != string::npos ) {
        // cout << "Parsing substring: " << element_str.substr(prev_pos, pos) << endl;
        element_vec[stoi( element_str.substr(prev_pos, pos) )] = 1;
        prev_pos = pos + 1;
        pos = element_str.find(',', prev_pos);
    }
    element_vec[stoi( element_str.substr(prev_pos) )] = 1;
    return element_vec;
}

vector<vector<uint8_t>> parse_elements(const string& elements_str, ssize_t objective_size) {
    vector<vector<uint8_t>> elements {};
    size_t left_pos = elements_str.find('(');
    while( left_pos != string::npos ) {
        size_t right_pos = elements_str.find(')', left_pos);
        string element_str = elements_str.substr(left_pos + 1, right_pos - left_pos - 1);
        elements.push_back( parse_element(element_str, objective_size) );
        left_pos = elements_str.find('(', right_pos);
    }
    return elements;
}

uint32_t bfs(vector<uint8_t> objective, const vector<vector<uint8_t>>& elements) {
    
    unordered_set<string> visited {{hash_vector(vector<uint8_t>(objective.size(), 0))}};
    deque<pair<vector<uint8_t>, uint32_t>> queue {{vector<uint8_t>(objective.size(), 0), 0}};
    while ( !queue.empty() ) {
        auto current = queue.front().first;
        cout << "Current: ";
        for ( const auto& c : current ) {
            cout << to_string(c) << " ";
        }
        uint32_t history = queue.front().second;
        cout << ", History: " << history << endl;
        queue.pop_front();
        visited.insert(hash_vector(current));
        for ( const auto& element : elements ) {
            auto next = sum_vectors(current, element);
            if (equals_vector(next, objective) ) {
                return history + 1;
            }
            if ( visited.find(hash_vector(next)) == visited.end() ) {
                visited.insert(hash_vector(next));
                if (vector_no_more_valid(next, objective)) {
                    continue;
                }
                uint32_t new_history = history + 1;
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

        auto objective = parse_objective(line.substr(line.find('{') + 1, line.find('}') - line.find('{') - 1));
        auto elements = parse_elements(line, objective.size());
        
        cout << "Objective: ";
        for ( const auto& obj : objective ) {
            cout << to_string(obj) << " ";
        }
        cout << ", Elements: ";
        for ( const auto& element : elements ) {
            for ( const auto& e : element ) {
                cout << to_string(e) << " ";
            }
            cout << "| ";
        }
        cout << endl;
        uint16_t result = bfs(objective, elements);
        total += result;
    }
	// Close the file
	input_file.close();

    cout << "Total: " << total << endl;
}
