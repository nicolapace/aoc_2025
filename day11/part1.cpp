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

vector<string> parse_elements(const string& elements_str_orig) {
    vector<string> elements {};
    auto elements_str = elements_str_orig.substr(elements_str_orig.find(' ') + 1);
    size_t right_pos = elements_str.find(' ');

    while( right_pos != string::npos ) {
        elements.push_back( elements_str.substr(0, right_pos) );
        elements_str = elements_str.substr(right_pos + 1);
        right_pos = elements_str.find(' ');
    }
    elements.push_back( elements_str );
    return elements;
}

long dfs(const unordered_map<string, vector<string>>& graph, const string& current, const string& objective, unordered_set<string> current_path) {

    if ( current == objective ) {
        return 1;
    }
    current_path.insert(current);
    long paths = 0;
    for ( const auto& neighbor : graph.at(current) ) {
        if ( current_path.find(neighbor) == current_path.end() ) {
            paths += dfs(graph, neighbor, objective, current_path);
        }
    }
    current_path.erase(current);
    return paths;
}

int main() {
	string line;
	ifstream input_file("input.txt");

    unordered_map<string, vector<string>> graph;
    while (getline(input_file, line)) {
        graph[line.substr(0, line.find(':'))] = parse_elements(line);
    }
	// Close the file
	input_file.close();

    auto total_paths = dfs(graph, "you", "out", {});

    cout << "Total: " << total_paths << endl;
}
