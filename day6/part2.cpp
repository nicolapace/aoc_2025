#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

int main() {
	std::string line;
	std::ifstream input_file("input.txt");
    
    vector<vector<string>> matrix{};

    while (getline(input_file, line)) {
        vector<string> matrix_row;
        int char_idx = 0;
        while (char_idx < line.length()) {
            matrix_row.push_back(string(1, line[char_idx]));
            char_idx++;
        }
        matrix_row.push_back(" "); // add space at the end of the row
        matrix.push_back(matrix_row);
    }
	input_file.close();

    vector<string> operations = matrix.back();
    //remove spaces from operations
    vector<string> operations_without_spaces{};
    for ( const auto& op : operations ) {
        if ( op != " " ) {
            operations_without_spaces.push_back(op);
        }
    }
    operations = operations_without_spaces;
    matrix.pop_back();

    //print matrix
    cout << "Matrix:" << endl;
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            cout << val;
        }
        cout << endl;
    }

    int row = matrix.size() - 1;
    int col = 0;
    long total = 0;
    long curr_total = 0;
    int curr_operation_idx = 0;
    if (operations[curr_operation_idx] == "*") {
        curr_total = 1;
    }
    long curr_num = 0;
    while (col < matrix[0].size())
    {
        int exp = 0;
        bool all_empty = true;
        while (row >= 0)
        {
            if (matrix[row][col] == " ") {
                row--;
                continue;
            }
            curr_num += stol(matrix[row][col]) * pow(10, exp);
            exp++;
            all_empty = false;
            row--;
        }

        cout << "Current num: " << curr_num << " Current total: " << curr_total << endl;
        if (all_empty) {
            curr_operation_idx++;
            total += curr_total;
            cout << " Partial total: " << total << endl;
            curr_total = 0;
            
            if( curr_operation_idx < operations.size() ) {
                if (operations[curr_operation_idx] == "*") {
                    curr_total = 1;
                } else if (operations[curr_operation_idx] == "+") {
                    curr_total = 0;
                } else {
                    cout << "Unknown operation: " << operations[curr_operation_idx] << endl;
                }
            }
        }else{
            if (operations[curr_operation_idx] == "*") {
                curr_total *= curr_num;
            } else if (operations[curr_operation_idx] == "+") {
                curr_total += curr_num;
            } else {
                cout << "Unknown operation: " << operations[curr_operation_idx] << endl;
            }
        }
        curr_num = 0;
        col++;
        row = matrix.size() - 1;
    
    }
    cout << "Total: " << total << endl;
    

}
