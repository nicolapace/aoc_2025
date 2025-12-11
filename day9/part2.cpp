#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_set>

using namespace std;

struct Position {
    int x;
    int y;
};

inline uint64_t hash_position(const Position& pos) {
    return ((uint64_t)(pos.x) << 32) | (uint32_t)(pos.y);
}

bool point_inside_perimeter(const Position& pos, int min_x, int min_y, int max_x, int max_y, unordered_map<uint64_t, Position> &perimeter) {
    //check left
    auto new_perimeter = perimeter;
    for( int x = pos.x; x >= min_x; x-- ) {
        Position check_pos = {x, pos.y};
        uint64_t pos_hash = hash_position(check_pos);
        new_perimeter.insert({pos_hash, check_pos});
        if ( perimeter.find( pos_hash ) != perimeter.end() ) {
            break;
        }
        if ( x == min_x ) {
            return false;
        }
    }
    //check right
    for( int x = pos.x; x <= max_x; x++ ) {
        Position check_pos = {x, pos.y};
        uint64_t pos_hash = hash_position(check_pos);
        new_perimeter.insert({pos_hash, check_pos});
        if ( perimeter.find( pos_hash ) != perimeter.end() ) {
            break;
        }
        if ( x == max_x ) {
            return false;
        }
    }
    //check up
    for( int y = pos.y; y >= min_y; y-- ) {
        Position check_pos = {pos.x, y};
        uint64_t pos_hash = hash_position(check_pos);
        new_perimeter.insert({pos_hash, check_pos});
        if ( perimeter.find( pos_hash ) != perimeter.end() ) {
            break;
        }
        if ( y == min_y ) {
            return false;
        }
    }
    //check down
    for( int y = pos.y; y <= max_y; y++ ) {
        Position check_pos = {pos.x, y};
        uint64_t pos_hash = hash_position(check_pos);
        new_perimeter.insert({pos_hash, check_pos});
        if ( perimeter.find( pos_hash ) != perimeter.end() ) {
            break;
        }
        if ( y == max_y ) {
            return false;
        }
    }
    perimeter = new_perimeter;
    return true;
}

class Square
{
    public:
        Position vertex1;
        Position vertex2;
        long area;

        Square(Position a, Position b, long c) : vertex1(a), vertex2(b), area(c) {}
        
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
    return (abs(a.x - b.x) + 1) * (abs(a.y - b.y) + 1);
}

unordered_map<uint64_t, Position> build_perimeter_given_vertices(vector<Position>& vertices) {
    unordered_map<uint64_t, Position> perimeter{};

    vertices.push_back(vertices[0]);

    int i=1;
    while ( i<vertices.size() ) {
        // cout << "Processing position (" << vertices[i].x << "," << vertices[i].y << ")" << endl;
        if (vertices[i].x == vertices[i-1].x) {
            if (vertices[i].y < vertices[i-1].y) {
                int y_start = vertices[i-1].y - 1;
                int y_end = vertices[i].y;
                for (int y = y_start; y >= y_end; y--) {
                    Position new_pos = {vertices[i].x, y};
                    perimeter[hash_position(new_pos)] = new_pos;
                }
            } else if (vertices[i].y > vertices[i-1].y) {
                int y_start = vertices[i-1].y + 1;
                int y_end = vertices[i].y;
                for (int y = y_start; y <= y_end; y++) {
                    Position new_pos = {vertices[i].x, y};
                    perimeter[hash_position(new_pos)] = new_pos;
                }
            }
        } else if (vertices[i].y == vertices[i-1].y) {
            if (vertices[i].x < vertices[i-1].x) {
                int x_start = vertices[i-1].x - 1;
                int x_end = vertices[i].x;
                for (int x = x_start; x >= x_end; x--) {
                    Position new_pos = {x, vertices[i].y};
                    perimeter[hash_position(new_pos)] = new_pos;
                }
            } else if (vertices[i].x > vertices[i-1].x) {
                int x_start = vertices[i-1].x + 1;
                int x_end = vertices[i].x;
                for (int x = x_start; x <= x_end; x++) {
                    Position new_pos = {x, vertices[i].y};
                    perimeter[hash_position(new_pos)] = new_pos;
                }
            }
        } else {
            cout << "Invalid position sequence between (" << vertices[i-1].x << "," << vertices[i-1].y << ") and (" << vertices[i].x << "," << vertices[i].y << ")" << endl;
        }
        i++;
    }
    vertices.pop_back(); 

    return perimeter;
}

vector<Position> build_square_perimeter(const Square& square) {
    vector<Position> perimeter = {};
    int min_x = min(square.vertex1.x, square.vertex2.x);
    int max_x = max(square.vertex1.x, square.vertex2.x);
    int min_y = min(square.vertex1.y, square.vertex2.y);
    int max_y = max(square.vertex1.y, square.vertex2.y);
    
    for(int x = min_x; x <= max_x; x++) {
        perimeter.push_back({x, min_y});
        perimeter.push_back({x, max_y});
    }
    for(int y = min_y; y <= max_y; y++) {
        perimeter.push_back({min_x, y});
        perimeter.push_back({max_x, y});
    }
    return perimeter;
}

bool square_inside_perimeter(const Square& square, unordered_map<uint64_t, Position> &inside_perimeter, unordered_map<uint64_t, Position> &outside_perimeter, int64_t min_x, int64_t min_y, int64_t max_x, int64_t max_y) {
    vector<Position> square_perimeter = build_square_perimeter(square);
    cout << "Checking square with area " << square.area << " between (" << square.vertex1.x << "," << square.vertex1.y;
    cout << ") and (" << square.vertex2.x << "," << square.vertex2.y << ") perimeter [has " << square_perimeter.size() << " positions to check]." << endl;

    for (const auto& square_perimeter_pos : square_perimeter) {
        // cout << "Checking perimeter position (" << square_perimeter_pos.x << "," << square_perimeter_pos.y << ")" << endl;
        if ( outside_perimeter.find( hash_position(square_perimeter_pos) ) != outside_perimeter.end() ) {
            return false;
        }
        if ( inside_perimeter.find( hash_position(square_perimeter_pos) ) == inside_perimeter.end() ) {
            if ( !point_inside_perimeter(square_perimeter_pos, min_x, min_y, max_x, max_y, inside_perimeter) ) {
                outside_perimeter[ hash_position(square_perimeter_pos) ] = square_perimeter_pos;
                return false;
            } else {
                inside_perimeter[ hash_position(square_perimeter_pos) ] = square_perimeter_pos;
            }
        }
    }
    return true;
}

//modifies vertices in place to use compressed coordinates
void compress_vertices(vector<Position>& vertices, vector<int>& vertices_x, vector<int>& vertices_y) {

    sort(vertices_x.begin(), vertices_x.end());
    //remove duplicates
    vertices_x.erase( unique( vertices_x.begin(), vertices_x.end() ), vertices_x.end() );
    unordered_map<int, int> compressed_xs{};
    cout << "Compressed Xs:" << endl;
    for (int i=0; i<vertices_x.size(); i++) {
        compressed_xs[ vertices_x[i] ] = i;
        cout << vertices_x[i] << " -> " << i << endl;
    }
    sort(vertices_y.begin(), vertices_y.end());
    //remove duplicates
    vertices_y.erase( unique( vertices_y.begin(), vertices_y.end() ), vertices_y.end() );
    unordered_map<int, int> compressed_ys{};
    cout << "Compressed Ys:" << endl;
    for (int i=0; i<vertices_y.size(); i++) {
        compressed_ys[ vertices_y[i] ] = i;
        cout << vertices_y[i] << " -> " << i << endl;
    }

    vector<Position> compressed_vertices{};
    for (int i=0; i<vertices.size(); i++) {
        int compressed_x = compressed_xs[ vertices[i].x ];
        int compressed_y = compressed_ys[ vertices[i].y ];
        compressed_vertices.push_back({compressed_x, compressed_y});
    }

    vertices = compressed_vertices;
}

priority_queue<Square> build_squares_max_heap(const vector<Position>& vertices, const vector<int>& vertices_x, const vector<int>& vertices_y) {
    priority_queue<Square> max_heap; 
    int i = 0;
    while( i < vertices.size() ) {
        int j = i + 1;
        while( j < vertices.size() ) {
            int area = calc_area({vertices_x[vertices[i].x], vertices_y[vertices[i].y]}, {vertices_x[vertices[j].x], vertices_y[vertices[j].y]});
            max_heap.push(Square(vertices[i], vertices[j], area));
            j++;
        }
        i++;
    }
    return max_heap;
}

void print_perimeter(const unordered_map<uint64_t, Position> &perimeter, int min_x, int min_y, int max_x, int max_y) {
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            uint64_t pos_hash = hash_position({x, y});
            if ( perimeter.find( pos_hash ) != perimeter.end() ) {
                cout << 'X';
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
	string line;
	ifstream input_file("input.txt");
    
    vector<int> vertices_x{};
    vector<int> vertices_y{};
    vector<Position> vertices{};

    while (getline(input_file, line)) {
        size_t vertex1 = line.find(',');
        int x = stol(line.substr(0, vertex1));
        int y = stol(line.substr(vertex1 + 1));

        vertices_x.push_back(x);
        vertices_y.push_back(y);
        vertices.push_back({x, y});
    }
	input_file.close();

    //compress vertices
    compress_vertices(vertices, vertices_x, vertices_y);
    
    int min_x = -1;
    int min_y = -1;
    int max_x = vertices_x.size() + 1;
    int max_y = vertices_y.size() + 1;

    //build perimeter map
    unordered_map<uint64_t, Position> perimeter = build_perimeter_given_vertices(vertices);

    // print_perimeter(perimeter, min_x, min_y, max_x, max_y);

    //build max-heap of squares
    priority_queue<Square> max_heap = build_squares_max_heap( vertices, vertices_x, vertices_y );

    unordered_map<uint64_t, Position> outside_perimeter{}; 
    unordered_map<uint64_t, Position> inside_perimeter = perimeter;
    Square largest_dist = max_heap.top();
    while ( !square_inside_perimeter( largest_dist, inside_perimeter, outside_perimeter, min_x, min_y, max_x, max_y ) ) {
        cout << "Square with area " << largest_dist.area << " between positions (" << largest_dist.vertex1.x << "," << largest_dist.vertex1.y;
        cout << ") and (" << largest_dist.vertex2.x << "," << largest_dist.vertex2.y << ") is not fully inside perimeter." << endl;
        max_heap.pop();
        largest_dist = max_heap.top();
    }
    //print decompressing results
    cout << "Largest area is " << largest_dist.area << " between positions (" << vertices_x[largest_dist.vertex1.x] << "," << vertices_y[largest_dist.vertex1.y];
    cout << ") and (" << vertices_x[largest_dist.vertex2.x] << "," << vertices_y[largest_dist.vertex2.y] << ")" << endl;

}