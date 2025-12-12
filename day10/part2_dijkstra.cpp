#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>

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
    auto pos = element_str.find(',');
    while( pos != string::npos ) {
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

struct State {
    vector<uint8_t> position;
    uint32_t cost;
    
    // Max heap: bigger cost first
    bool operator<(const State& other) const {
        return cost < other.cost;
    }
};

uint32_t dijkstra(vector<uint8_t> objective, const vector<vector<uint8_t>>& elements) {
    priority_queue<State> pq;
    unordered_map<string, uint32_t> best_cost;
    
    vector<uint8_t> start(objective.size(), 0);
    pq.push({start, 0});
    best_cost[hash_vector(start)] = 0;
    
    while ( !pq.empty() ) {
        uint32_t current_level = pq.top().cost;
        
        // Process all states at current level before moving to next
        vector<State> level_states;
        while ( !pq.empty() && pq.top().cost == current_level ) {
            level_states.push_back(pq.top());
            pq.pop();
        }
        
        cout << "Processing level: " << current_level << " with " << level_states.size() << " states" << endl;
        
        for ( const auto& current : level_states ) {
            string current_hash = hash_vector(current.position);
            
            if ( best_cost.count(current_hash) && best_cost[current_hash] < current.cost ) {
                continue;
            }
            
            cout << "  Current cost: " << current.cost << ", Position: ";
            for ( const auto& c : current.position ) {
                cout << to_string(c) << " ";
            }
            cout << endl;
            
            if ( equals_vector(current.position, objective) ) {
                return current.cost;
            }
            
            for ( const auto& element : elements ) {
                vector<uint8_t> next = sum_vectors(current.position, element);
                uint32_t next_cost = current.cost + 1;
                
                if ( vector_no_more_valid(next, objective) ) {
                    continue;
                }
                
                string next_hash = hash_vector(next);
                
                if ( !best_cost.count(next_hash) || next_cost < best_cost[next_hash] ) {
                    best_cost[next_hash] = next_cost;
                    pq.push({next, next_cost});
                }
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
        uint16_t result = dijkstra(objective, elements);
        total += result;
    }
	input_file.close();

    cout << "Total: " << total << endl;
}
