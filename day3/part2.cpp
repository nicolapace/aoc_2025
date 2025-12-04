#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

static constexpr int SIZE = 12;

int main() {
	std::string line;

	std::ifstream MyReadFile("input.txt");
	long total {};
	while (std::getline (MyReadFile, line)) {
       
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
	std::cout << "Total: " << total << std::endl;

	// Close the file
	MyReadFile.close();
}
