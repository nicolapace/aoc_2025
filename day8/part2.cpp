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
    long z;
};

class Distance
{
    public:
        Position pos1;
        Position pos2;
        long dist;

        Distance(Position a, Position b, long d) : pos1(a), pos2(b), dist(d) {}
        
        bool operator>(const Distance& other) const {
            return dist > other.dist;
        }
        bool operator<(const Distance& other) const {
            return dist < other.dist;
        }
        bool operator==(const Distance& other) const {
            return dist == other.dist;
        }
        bool operator<=(const Distance& other) const {
            return dist <= other.dist;
        }
        bool operator>=(const Distance& other) const {
            return dist >= other.dist;
        }
};

long distance(const Position& a, const Position& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

string pos2str(const Position& pos) {
    return to_string(pos.x) + "," + to_string(pos.y) + "," + to_string(pos.z);
}

int main() {
	string line;
	ifstream input_file("input.txt");
    
    vector<Position> positions{};

    while (getline(input_file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        long x = stol(line.substr(0, pos1));
        long y = stol(line.substr(pos1 + 1, pos2 - pos1 - 1));
        long z = stol(line.substr(pos2 + 1));
        positions.push_back({x, y, z});

    }
	input_file.close();

    priority_queue<Distance, vector<Distance>, greater<Distance>> min_heap; // Min-heap

    int i = 0;
    while( i < positions.size() ) {
        int j = i + 1;
        while( j < positions.size() ) {
            if ( i != j ) {
                long dist = distance(positions[i], positions[j]);
                min_heap.push(Distance(positions[i], positions[j], dist));
            }
            j++;
        }
        i++;
    }

    vector<unordered_set<string>> clusters = {};
    for (const auto& pos : positions) {
        unordered_set<string> new_cluster = { pos2str(pos) };
        clusters.push_back(new_cluster);
    }

    Distance last_dist = min_heap.top();
    while (clusters.size() > 1)
    {
        Distance min_dist = min_heap.top();
        min_heap.pop();
        string pos1_str = pos2str(min_dist.pos1);
        string pos2_str = pos2str(min_dist.pos2);
        last_dist = min_dist;

        cout << "Processing nearest distance " << min_dist.dist << " between positions " << pos1_str << " and " << pos2_str << endl;

        int cluster1_ptr = -1;
        int cluster2_ptr = -1;

        int i = 0;
        while (i < clusters.size()) {
            if ( clusters[i].find( pos1_str ) != clusters[i].end() ) {
                cluster1_ptr = i;
            }
            if ( clusters[i].find( pos2_str ) != clusters[i].end() ) {
                cluster2_ptr = i;
            }
            i++;
        }
        if( cluster1_ptr == -1 && cluster2_ptr == -1){
            unordered_set<string> new_cluster = { pos1_str, pos2_str };
            clusters.push_back(new_cluster);
        }else if( cluster1_ptr != -1 && cluster2_ptr == -1 ){
            clusters[cluster1_ptr].insert(pos2_str);
        }else if( cluster1_ptr == -1 && cluster2_ptr != -1 ){
            clusters[cluster2_ptr].insert(pos1_str);
        }else if( cluster1_ptr != cluster2_ptr ){
            // merge clusters
            clusters[cluster1_ptr].insert( clusters[cluster2_ptr].begin(), clusters[cluster2_ptr].end() );
            clusters.erase( clusters.begin() + cluster2_ptr );
        }else {
            // cout << "Both positions are already in the same cluster." << endl;
        }
        cout << "Number of clusters: " << clusters.size() << endl;
    }
    cout << "Last distance processed: " << last_dist.dist << " between positions " << pos2str(last_dist.pos1) << " and " << pos2str(last_dist.pos2) << endl;
    cout << "Result: " << last_dist.pos1.x * last_dist.pos2.x << endl;    
}
