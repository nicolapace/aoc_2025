#include <iostream>
#include <fstream>

using namespace std;

int main() {
	string line;
	ifstream input_file("input.txt");
	int total {};
	while (getline(input_file, line)) {
        int first_num = 1;
        int second_num = 1;
        for ( auto c : line.substr(0, line.length()-1) ) {
            int num = stoi(string(1, c));
            if(num>first_num) {
                second_num = 1;
                first_num = num;
            } else if (num > second_num) {
                second_num = num;
            }
            // cout << " "  << first_num << second_num << " ";
        }
        int last_num = stoi(string(1, line[line.length()-1]));
        if ( last_num > second_num) {
            second_num = last_num;
        }
        cout << line << " ";
        cout << "First: " << first_num << " Second: " << second_num << endl;
        total += 10*first_num + second_num;
	}
    // Close the file
	input_file.close();
	cout << "Total: " << total << endl;
}
