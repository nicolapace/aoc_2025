#include <iostream>
#include <fstream>

int main() {
	std::string myText;

	std::ifstream MyReadFile("input.txt");
	int total {};
	while (std::getline (MyReadFile, myText)) {
        int first_num = 1;
        int second_num = 1;
        for ( auto c : myText.substr(0, myText.length()-1) ) {
            int num = std::stoi(std::string(1, c));
            if(num>first_num) {
                second_num = 1;
                first_num = num;
            } else if (num > second_num) {
                second_num = num;
            }
            // std::cout << " "  << first_num << second_num << " ";
        }
        int last_num = std::stoi(std::string(1, myText[myText.length()-1]));
        if ( last_num > second_num) {
            second_num = last_num;
        }
        std::cout << myText << " ";
        std::cout << "First: " << first_num << " Second: " << second_num << std::endl;
        total += 10*first_num + second_num;
        
	}
	std::cout << "Total: " << total << std::endl;

	// Close the file
	MyReadFile.close();
}
