#include <iostream>
#include <fstream>

using namespace std;

int main() {
	string line;
	ifstream input_file("input.txt");
	int current_angle {50};
	uint16_t number_of_zeros {0};
	while (getline(input_file, line)) {
		auto pair = make_pair(line[0], stoi(line.substr(1)));
		if (pair.first == 'L') {
			current_angle = (current_angle - pair.second);
		} else if (pair.first == 'R') {
			current_angle = (current_angle + pair.second);
		} else {
			cout << "Invalid rotation direction: " << pair.first << endl;
		}

		current_angle = current_angle % 100;
		if (current_angle == 0) {
			number_of_zeros++;
		}
	}
	// Close the file
	input_file.close();
	cout << "Number of zeros: " << number_of_zeros << endl;
}
