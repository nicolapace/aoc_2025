#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <deque>

using namespace std;

class Gift {
    public: 
        int gift_id;
        vector<vector<char>> data;
        int area;
        Gift(int id) : gift_id(id), area(0) {}
};

class Tree {
    public:
        uint8_t rows;
        uint8_t cols;
        vector<vector<char>> space;
        vector<uint8_t> num_gifts;
        int area;
        
        void init_space() {
            space = vector<vector<char>>(rows, vector<char>(cols, '.'));
        }
        Tree(uint8_t r, uint8_t c) : rows(r), cols(c), area(0) {
            init_space();
        }

};

bool gift_collides(const Gift &gift, const Tree &tree, uint8_t row_offvector, uint8_t col_offvector) {
    for ( size_t r = 0; r < gift.data.size(); r++ ) {
        for ( size_t c = 0; c < gift.data[r].size(); c++ ) {
            if ( gift.data[r][c] == '#' ) {
                if ( row_offvector + r >= tree.rows || col_offvector + c >= tree.cols ) {
                    return true;
                }
                if ( tree.space[row_offvector + r][col_offvector + c] == '#' ) {
                    return true;
                }
            }
        }
    }
    return false;
}

vector<Gift> build_gifts_to_place(const Tree &tree, const vector<Gift> &gifts) {
    vector<Gift> gifts_to_place {};
    int gift_index = 0;
    while ( gift_index < gifts.size() ) {
        int num_gifts_to_place = tree.num_gifts[gift_index];
        while ( num_gifts_to_place > 0 ) {
            gifts_to_place.push_back(gifts[gift_index]);
            num_gifts_to_place--;
        }
        gift_index++;
    }
    return gifts_to_place;
}

vector<pair<uint8_t, uint8_t>> generate_possible_positions(Tree &tree, const Gift &gift) {
    vector<pair<uint8_t, uint8_t>> positions {};
    
    uint8_t max_row = tree.rows > gift.data.size() ? tree.rows - gift.data.size() : 0;
    uint8_t max_col = tree.cols > gift.data[0].size() ? tree.cols - gift.data[0].size() : 0;
    
    for ( uint8_t r = 0; r <= max_row; r++ ) {
        for ( uint8_t c = 0; c <= max_col; c++ ) {
            positions.push_back({r, c});
        }
    }
    return positions;
}
void place_gift_on_tree(const Gift &gift, Tree &tree, uint8_t row_offset, uint8_t col_offset) {
    for ( size_t r = 0; r < gift.data.size(); r++ ) {
        for ( size_t c = 0; c < gift.data[r].size(); c++ ) {
            if ( gift.data[r][c] == '#' ) {
                tree.space[row_offset + r][col_offset + c] = '#';
            }
        }
    }
    tree.area += gift.area;
}

void remove_gift_from_tree(const Gift &gift, Tree &tree, uint8_t row_offset, uint8_t col_offset) {
    for ( size_t r = 0; r < gift.data.size(); r++ ) {
        for ( size_t c = 0; c < gift.data[r].size(); c++ ) {
            if ( gift.data[r][c] == '#' ) {
                tree.space[row_offset + r][col_offset + c] = '.';
            }
        }
    }
    tree.area -= gift.area;
}

string calculate_gift_hash(const Gift &gift) {
    string hash;
    for ( const auto& row : gift.data ) {
        for ( const auto& ch : row ) {
            hash += ch;
        }
        hash += "/";
    }
    return hash;
}

vector<Gift> generate_4_gift_rotations(const Gift &gift) {
    vector<Gift> rotations {};
    unordered_set<string> rotation_hashes {};

    if ( gift.data.empty() || gift.data[0].empty() ) {
        cout << "Generated 0 unique rotations for gift " << to_string(gift.gift_id) << endl;
        return rotations;
    }

    Gift current_rotation = gift;
    string current_hash = calculate_gift_hash(current_rotation);

    rotations.push_back(current_rotation);
    rotation_hashes.insert(current_hash);

    // Generate up to 3 more rotations (90/180/270 degrees) and keep only unique ones.
    for ( size_t i = 0; i < 3; i++ ) {
        Gift next_rotation = Gift(gift.gift_id);
        next_rotation.data = vector<vector<char>>(current_rotation.data[0].size(), vector<char>(current_rotation.data.size(), '.'));
        for ( size_t r = 0; r < current_rotation.data.size(); r++ ) {
            for ( size_t c = 0; c < current_rotation.data[r].size(); c++ ) {
                next_rotation.data[c][current_rotation.data.size() - 1 - r] = current_rotation.data[r][c];
            }
        }
        next_rotation.area = current_rotation.area;

        current_rotation = next_rotation;
        current_hash = calculate_gift_hash(current_rotation);

        if ( rotation_hashes.insert(current_hash).second ) {
            rotations.push_back(current_rotation);
        }
    }

    cout << "Generated " << to_string(rotations.size()) << " unique rotations for gift " << to_string(gift.gift_id) << endl;
    return rotations;
}   

bool dfs(Tree &tree, vector<Gift> &gifts_to_place, const vector<vector<Gift>> &all_gift_rotations) {
    if ( gifts_to_place.empty() ) {
        return true;
    }

    int area_needed = 0;
    for ( const auto& gift : gifts_to_place ) {
        area_needed += gift.area;
    }
    if ( tree.area + area_needed > tree.rows * tree.cols ) {
        return false;
    }

    Gift gift_to_place = gifts_to_place.back();
    gifts_to_place.pop_back();

    //rotate_gift
    vector<Gift> rotations_of_current_gift = all_gift_rotations[gift_to_place.gift_id];
    //generate all possible positions of the gift on the tree
    vector<pair<uint8_t, uint8_t>> possible_positions = generate_possible_positions(tree, gift_to_place);

    for ( const auto& pos : possible_positions ) {
        for ( const auto& rotation : rotations_of_current_gift ) {
            if ( gift_collides(rotation, tree, pos.first, pos.second) ) {
                continue;
            }
            // Place gift
            place_gift_on_tree(rotation, tree, pos.first, pos.second);
            // Recurse
            if ( dfs(tree, gifts_to_place, all_gift_rotations) ) {
                return true;
            }
            // Remove gift (backtrack)
            remove_gift_from_tree(rotation, tree, pos.first, pos.second);
        }
    }
    
    // Restore the gift and its rotations for backtracking at the parent level
    gifts_to_place.push_back(gift_to_place);
    return false;
}

bool check_tree(Tree &tree, vector<Gift> &gifts, const vector<vector<Gift>> &all_gift_rotations) {
    vector<Gift> gifts_to_place = build_gifts_to_place(tree, gifts);
    return dfs(tree, gifts_to_place, all_gift_rotations);
}

int main() {
	string line;
	ifstream input_file("input.txt");

    // parse gifts
    vector<Gift> gifts {};
    int gift_id = 0;
    while (getline(input_file, line)) {
        if( line.find(' ')!= string::npos ) {
            //put back line
            input_file.seekg(-static_cast<int>(line.length()) - 1, ios_base::cur);
            break;
        }
        if(line.find(':') != string::npos ){
            gifts.push_back(Gift(gift_id++));
            continue;
        }
        if(line.empty()) {
            continue;
        }
        for( char &ch : line ) {
            if ( ch == '#'){
                gifts.back().area += 1;
            }
        }
        gifts.back().data.push_back(vector<char>(line.begin(), line.end()));
    }
    // parse trees
    vector<Tree> trees {};
    while (getline(input_file, line)) {
        // Skip empty lines
        if (line.empty() || line.find('x') == string::npos) {
            continue;
        }
        auto rows = stoi(line.substr(0, line.find('x')));
        auto cols = stoi(line.substr(line.find('x') + 1, line.find(':') - line.find('x') - 1));
        Tree tree(rows, cols);
        //parse num gifts
        vector<uint8_t> num_gifts {};
        auto space = line.find(' ');
        line = line.substr(space + 1);
        space = line.find(' ');
        while ( space != string::npos ) {
            num_gifts.push_back( stoi(line.substr(0, space)) );
            line = line.substr(space + 1);
            space = line.find(' ');
        }
        num_gifts.push_back( stoi(line) );
        tree.num_gifts = num_gifts;
        //store tree
        trees.push_back(tree);
    } 
	// Close the file
	input_file.close();


    // Debug print gifts
    for ( size_t i = 0; i < gifts.size(); i++ ) {
        cout << "Gift " << to_string(i) << " with area " << to_string(gifts[i].area) << ":" << endl;
        for ( const auto& d : gifts[i].data ) {
            for ( const auto& ch : d ) {
                cout << ch;
            }
            cout << endl;
        }
        cout << endl;
    }

    // Debug print trees
    for ( size_t i = 0; i < trees.size(); i++ ) {
        cout << "Tree " << to_string(i) << ": " << to_string(trees[i].rows) << "x" << to_string(trees[i].cols) << ", Gifts: ";
        for ( const auto& ng : trees[i].num_gifts ) {
            cout << to_string(ng) << " ";
        }
        cout << endl;
    }

    //generate all rotations of each gift
    vector<vector<Gift>> all_gift_rotations {};
    for ( const auto& gift : gifts) {
        all_gift_rotations.push_back( generate_4_gift_rotations(gift) );
    }

    // Check each tree
    uint16_t total = 0;
    for ( size_t i = 0; i < trees.size(); i++ ) {
        cout << "Checking tree " << to_string(i) << "..." << endl;
        if ( check_tree(trees[i], gifts, all_gift_rotations) ) {
            cout << "Tree " << to_string(i) << " can hold all gifts!" << endl;
            total++;
        } else {
            cout << "Tree " << to_string(i) << " cannot hold all gifts." << endl;
        }
    }
    cout << "Total trees that can hold all gifts: " << to_string(total) << endl;

}
