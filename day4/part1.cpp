#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int calculate_neighbors(const vector<vector<uint8_t>>& matrix, int x, int y) {
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    int count = 0;
    for (auto& dir : directions) {
        int newX = x + dir[0];
        int newY = y + dir[1];
        if (newX >= 0 && newX < matrix.size() && newY >= 0 && newY < matrix[0].size()) {
            count += matrix[newX][newY];
        }
    }
    return count;
}

int main() {
	std::string line;

	std::ifstream MyReadFile("input.txt");
	long total {};
    vector<vector<uint8_t>> matrix;
    vector<vector<string>> char_matrix;
	while (std::getline (MyReadFile, line)) {
        vector<uint8_t> row;
        vector<string> char_row;
        for ( auto c : line ) {
            if(c == '@'){
                row.push_back(1);
                char_row.push_back("@");
            } else if (c == '.'){
                row.push_back(0);
                char_row.push_back(".");
            } else {
                throw std::runtime_error("Unexpected character in input");
            }
        }
        matrix.push_back(row);
        char_matrix.push_back(char_row);
	}
	// Close the file
	MyReadFile.close();

    for( int i=0; i<matrix.size(); i++) {
        for (int j=0; j<matrix[0].size(); j++) {
            if (matrix[i][j] == 1) {
                int neighbors = calculate_neighbors(matrix, i, j);
                if ( neighbors < 4 ){
                    total += 1;
                    std::cout << 'X';
                } else {
                    std::cout << char_matrix[i][j];
                }
            } else {
                std::cout << char_matrix[i][j];
            }
            
        }
        std::cout << std::endl;
    }

	std::cout << "Total: " << total << std::endl;

}
