#include <iostream>
#include <fstream>

using namespace std;

int main() {
	string line;
	int current_angle {50};
	uint16_t number_of_zeros {0};
	ifstream input_file("input.txt");

	while (getline(input_file, line)) {
		auto pair = make_pair(line[0], stoi(line.substr(1)));
		auto previous_angle = current_angle;
		if (pair.first == 'L') {
			current_angle = (current_angle - pair.second);
		} else if (pair.first == 'R') {
			current_angle = (current_angle + pair.second);
		} else {
			cout << "Invalid rotation direction: " << pair.first << endl;
		}
		cout << "Current angle before normalization: " << current_angle << endl;

		if ( current_angle>=100 )
			number_of_zeros += current_angle/100;
		else if ( current_angle<=0 ) {
			number_of_zeros += 1 + (-current_angle)/100;
			if (previous_angle == 0)
				number_of_zeros -= 1;
		}
		current_angle = ((current_angle % 100) + 100) % 100;
	}
	// Close the file
	input_file.close();
	cout << "Number of zeros: " << number_of_zeros << endl;
}
