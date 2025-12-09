#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

void print_matrix(const vector<vector<char>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            cout << val;
        }
        cout << endl;
    }
    cout << endl;
}

string pair_to_str(pair<int, int> pos) {
    return to_string(pos.first) + to_string(pos.second);
}

void insert_into_map(queue<pair<int, int>> &timelines, unordered_map<string, long>& m, pair<int, int> key, long value) {
    string key_str = pair_to_str(key);
    if ( m.find(key_str) == m.end() ) {
        m[key_str] = value;
        timelines.push(key);
    } else {
        m[key_str] += value;
    }
}

int main() {
	string line;
	ifstream input_file("input.txt");
    
    vector<vector<char>> matrix{};

    while (getline(input_file, line)) {
        vector<char> row;
        int col = 0;
        while (col < line.length()) {
            row.push_back(line[col]);
            col++;
        }
        matrix.push_back(row);

    }
	input_file.close();

    queue<pair<int, int>> timelines = {};
    unordered_map<string, long> timelines_map = {};
    int col = 0;
    while(col < matrix[0].size()) {
        if( matrix[0][col] == 'S') {
            timelines.push({1, col});
            auto pos_str = pair_to_str({1, col});
            timelines_map[pos_str] = 1;
            break;
        }
        col++;
    }


    long num_timelines = 0;
    while (timelines.size() > 0)
    {
        pair<int, int> curr_time_point = timelines.front();
        int row = curr_time_point.first;
        int col = curr_time_point.second;
        auto curr_time_point_str = pair_to_str(curr_time_point);
        long current_superpositions = timelines_map[curr_time_point_str];

        timelines.pop();
        timelines_map.erase(curr_time_point_str);

        if ( row == matrix.size()-1 ) {
            cout << "Reached end at (" << row << "," << col << ") with " << current_superpositions << " superpositions." << endl;
            num_timelines += current_superpositions;
            continue;
        }
        if ( matrix[row+1][col] == '^' ) {
            insert_into_map(timelines, timelines_map, {row+1, col-1}, current_superpositions);
            insert_into_map(timelines, timelines_map, {row+1, col+1}, current_superpositions);
        } else {
            insert_into_map(timelines, timelines_map, {row+1, col}, current_superpositions);
        }
    }
    
    cout << "Number of timelines: " << num_timelines << endl;

    
}
