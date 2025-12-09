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

    int row = 0;
    int num_splits = 0;
    while(row < matrix.size()) {
        int col = 0;
        while(col < matrix[row].size()) {
            if( row > 0 && (matrix[row][col]!='^') && (matrix[row-1][col] == 'S' || matrix[row-1][col] == '|') ) {
                matrix[row][col] = '|';
            } else if ( col > 0 && col < matrix[0].size() - 1 && row > 0 && matrix[row][col] == '^' && matrix[row-1][col] == '|') {
                matrix[row][col+1] = '|';
                matrix[row][col-1] = '|';
                num_splits++;
            }
            col++;
        }
        print_matrix(matrix);
        row++;
    }
    cout << "Number of splits: " << num_splits << endl;

    
}
