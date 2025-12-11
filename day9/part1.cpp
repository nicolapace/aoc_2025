#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_set>

using namespace std;

struct Position {
    long x;
    long y;
};
class Square
{
    public:
        Position vertex1;
        Position vertex2;
        long area;

        Square(Position a, Position b, long d) : vertex1(a), vertex2(b), area(d) {}
        
        bool operator>(const Square& other) const {
            return area > other.area;
        }
        bool operator<(const Square& other) const {
            return area < other.area;
        }
        bool operator==(const Square& other) const {
            return area == other.area;
        }
        bool operator<=(const Square& other) const {
            return area <= other.area;
        }
        bool operator>=(const Square& other) const {
            return area >= other.area;
        }
};
long calc_area(const Position& a, const Position& b) {
    return (a.x - b.x + 1) * (a.y - b.y + 1);
}

int main() {
	string line;
	ifstream input_file("input.txt");
    
    vector<Position> positions{};

    while (getline(input_file, line)) {
        size_t vertex1 = line.find(',');
        long x = stol(line.substr(0, vertex1));
        long y = stol(line.substr(vertex1 + 1));
        positions.push_back({x, y});
    }
	input_file.close();


    priority_queue<Square> max_heap; 

    int i = 0;
    while( i < positions.size() ) {
        int j = i + 1;
        while( j < positions.size() ) {
            long area = calc_area(positions[i], positions[j]);
            max_heap.push(Square(positions[i], positions[j], area));
            j++;
        }
        i++;
    }

    Square largest_square = max_heap.top();
    cout << "Largest area is " << largest_square.area << " between positions (" << largest_square.vertex1.x << "," << largest_square.vertex1.y << ") and (" << largest_square.vertex2.x << "," << largest_square.vertex2.y << ")" << endl;
}