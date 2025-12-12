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

long dfs(const unordered_map<string, vector<string>>& graph, const string& current, const string& objective, const unordered_set<string>& to_avoid, unordered_set<string>& current_path, unordered_map<string, long>& memo) {
    if ( current == objective ) {
        return 1;
    }
    if ( memo.find(current) != memo.end() ) {
        return memo[current];
    }
    if ( to_avoid.find(current) != to_avoid.end() ) {
        cout << "Avoiding node: " << current << endl;
        return 0;
    }
    if ( graph.find(current) == graph.end() ) {
        return 0;
    }
    current_path.insert(current);
    long paths = 0;

    for ( const auto& neighbor : graph.at(current) ) {
        if ( current_path.find(neighbor) == current_path.end() ) {
            paths += dfs(graph, neighbor, objective, to_avoid, current_path, memo);
        }else{
            cout << "Loop detected at: " << neighbor << endl;
        }
    }
    current_path.erase(current);
    memo[current] = paths;
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
    
    // I know that from dac to fft there are not paths
    // So I only explore the paths: svr -> fft, fft -> dac, dac -> out

    unordered_set<string> current_path{};
    unordered_map<string, long> memo{};
    
    auto res1 = dfs(graph, "svr", "fft", {"dac"}, current_path, memo);
    memo = {};
    res1 *= dfs(graph, "fft", "dac", {}, current_path, memo);
    memo = {};
    res1 *= dfs(graph, "dac", "out", {}, current_path, memo);
    cout << "Res svr->fft->dac->out: " << res1 << endl;

    //this can be avoided but whatever
    current_path = {};
    memo = {};
    auto res2 = dfs(graph, "svr", "dac", {"fft"}, current_path, memo);
    memo = {};
    res2 *= dfs(graph, "dac", "fft", {}, current_path, memo);
    memo = {};
    res2 *= dfs(graph, "fft", "out", {}, current_path, memo);
    cout << "Res svr->dac->fft->out: " << res2 << endl;

    cout << "Total: " << res1 + res2 << endl;
}
