#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

int main() {
	string line;
	ifstream input_file("input.txt");
    
    vector<vector<string>> operations{};

    while (getline(input_file, line)) {
        vector<string> operation_row;
        while (line.length() > 0) {
            // split the line by spaces
            size_t pos = line.find(' ');
            if (pos == string::npos) {
                operation_row.push_back(line);
                break;
            } else {
                // trim spaces and push back
                auto line_trimmed = line.substr(0, pos);
                if (line_trimmed.length() > 0){
                    operation_row.push_back(line_trimmed);
                }
                line = line.substr(pos + 1);
            }
        }
        operations.push_back(operation_row);

    }
	input_file.close();

    //print operations
    for (const auto& row : operations) {
        for (const auto& op : row) {
            cout << op << " ";
        }
        cout << endl;
    }

    long total = 0;
    int col = 0;
    while(col < operations[0].size()) {
        int row = 0;
        long total_col = 0;
        if (operations[operations.size()-1][col] == "*") {
            total_col = 1;
        }
        while(row < operations.size()-1) {
            if (operations[operations.size()-1][col] == "*") {
                total_col *= stol(operations[row][col]);
            }else if( operations[operations.size()-1][col] == "+") {
                total_col += stol(operations[row][col]);
            }else{
                cout << "Unknown operation: " << operations[operations.size()-1][col] << endl;
            }
            cout << " " << operations[row][col] << " " << operations[operations.size()-1][col];
            row++;
        }
        cout << " = " << total_col << endl;
        total += total_col;
        col++;
    }
    cout << "Total: " << total << endl;
}
