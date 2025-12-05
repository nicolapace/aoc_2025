#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

static constexpr int SIZE = 12;

int main() {
	string line;
	ifstream input_file("input.txt");
	long total {};
	while (getline(input_file, line)) {
       
        int i = SIZE-1;
        while (i>=0){
            int j = 0;
            int max_num = 0;
            int max_j = 0;
            while (j<int(line.length())-i){
                auto current_num = stoi(string(1,line[j]));
                if (current_num > max_num) {
                    max_num = current_num;
                    max_j = j;
                    if (max_num == 9) {
                        break;
                    }
                }
                j++;
            }
            line = line.substr(max_j+1, line.length() - max_j);
            total += max_num * pow(10,i);
            i--;
        }
        
	}
	// Close the file
	input_file.close();
	cout << "Total: " << total << endl;
}
