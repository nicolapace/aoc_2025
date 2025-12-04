#include <iostream>
#include <fstream>

int main() {
	std::string myText;

	std::ifstream MyReadFile("input.txt");
	int current_angle {50};
	uint16_t number_of_zeros {0};
	while (std::getline (MyReadFile, myText)) {
		auto pair = std::make_pair(myText[0], std::stoi(myText.substr(1)));
		if (pair.first == 'L') {
			current_angle = (current_angle - pair.second);
		} else if (pair.first == 'R') {
			current_angle = (current_angle + pair.second);
		} else {
			std::cout << "Invalid rotation direction: " << pair.first << std::endl;
		}

		current_angle = current_angle % 100;
		if (current_angle == 0) {
			number_of_zeros++;
		}
	}
	std::cout << "Number of zeros: " << number_of_zeros << std::endl;

	// Close the file
	MyReadFile.close();
}
