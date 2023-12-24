#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <cmath>
#include <climits> // Include this for INT_MAX
#define P pair<int, int>
#define Cell pair<int, pair<int, int>>
#define X_MAX 9
#define Y_MAX 9

using namespace std;

int perceptionZone; // perception zone of Thanos
int X, Y; // coordinates of the infinity stone
const int dx[4] = {-1, 0, 1, 0}; // Change position for Thanos on the X-axis
const int dy[4] = {0, -1, 0, 1}; // Change position for Thanos on the Y-axis
vector<vector<char>> matrix(X_MAX, vector<char>(Y_MAX, 'N')); //2D grid represented as a vector of vectors for various characters.
map<pair<int, int>, bool> visited; // A map to keep track of visited coordinates.
//Create Thanos control zones dependence of variant
P ThanosZoneVar1[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {-1, 1},
                                   {1, -1}, {1, 1}};
P ThanosZoneVar2[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {-1, 1},
                                   {1, -1}, {1, 1}, {-2, -2}, {2, 2}, {-2, 2}, {2, -2}};
//Create all perceptions zones each avenger
P HulkZones[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
P ThorZones[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1},
                              {1, 1}};
P CaptainMarvelZones[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {-1, 1},
                                       {1, -1}, {1, 1}, {-2, 0}, {2, 0}, {0, -2}, {0, 2}};

//Function to check if a coordinate (x, y) is within the grid boundaries.
bool isValidCoordinate(int x, int y) {
    return (x >= 0 && x < X_MAX && y >= 0 && y < Y_MAX);
}
//Fills the map with character positions based on input. It also handles the perception zone of Thanos by modifying the grid.
void fillMap(P cur){
    //print moves of Thanos
    cout << "m " << cur.first << " " << cur.second << endl;
    int n;
    cin >> n;
    int xx, yy;
    char c;
    //read the console about items around Thanos
    for (int i = 0; i < n; i++) {
        cin >> xx >> yy >> c;
        //Perception Zone
        if (c == 'P'){
            matrix[xx][yy] = 'P';
        } // Hulk position
        else if (c == 'H'){
            matrix[xx][yy] = 'H';
            //filling perception zone around Hulk
            for (auto pair: HulkZones){
                int newX = xx + pair.first;
                int newY = yy + pair.second;
                //check that around Hulk perception zone does not exist zone of Captain Marvel
                if (isValidCoordinate(newX, newY) && matrix[newX][newY]!= 'M') {
                    matrix[newX][newY] = 'P';
                }
            }
        }
        //Thor position
        else if (c == 'T'){
            matrix[xx][yy] = 'T';
            //filling perception zone around Thor
            for (auto pair: ThorZones){
                int newX = xx + pair.first;
                int newY = yy + pair.second;
                //check that around Thor perception zone does not exist zone of Captain Marvel
                if (isValidCoordinate(newX, newY) && matrix[newX][newY]!= 'M') {
                    matrix[newX][newY] = 'P';
                }
            }
        }
        else if (c == 'M'){
            matrix[xx][yy] = 'm';
             for (auto pair: CaptainMarvelZones){
                int newX = xx + pair.first;
                int newY = yy + pair.second;
                if (isValidCoordinate(newX, newY) ) {
                    matrix[newX][newY] = 'M';
                }
            }
        }
    }
    //fill allowed cells around Thanos position dependence of variants and him coordinates
    if (perceptionZone == 1){
        for (auto pair: ThanosZoneVar1){
            if (isValidCoordinate(cur.first + pair.first, cur.second + pair.second)
                && matrix[cur.first + pair.first][cur.second + pair.second] == 'N'){
                matrix[cur.first + pair.first][cur.second + pair.second] = '.';
            }
        }
    } else {
        for (auto pair: ThanosZoneVar2){
            if (isValidCoordinate(cur.first+ pair.first, cur.second + pair.second)
                && matrix[cur.first + pair.first][cur.second + pair.second] == 'N'){
                matrix[cur.first + pair.first][cur.second + pair.second] = '.';
            }

        }
    }
}
//Recursively fills the grid, taking into account character positions and visited coordinates.
void fillingGrid(P p1, P p2){
    //set that this coord was visited
    visited[{p1.first, p1.second}] = true;
    //check 4 directions of moves of Thanos
    for (int i = 0; i < 4; i++){
        int x = p1.first + dx[i];
        int y = p1.second + dy[i];
        //if Thanos could move on this cell and this coordinate was no visited, then continue fill map
        if (isValidCoordinate(x, y) && (matrix[x][y] == '.' || matrix[x][y] == 'I') &&
            !visited[{x, y}]){
            fillMap({x, y});
            fillingGrid({x,y}, {p1.first, p1.second});
        }
    }
    if (!(p2.first == -1 && p2.second == -1)){
        fillMap(p2);
    }
}
//Initializes the grid and starts the backtracking search algorithm to populate it with characters based on their positions.
void backtracking_search(){
    for (int i = 0;i < X_MAX; i++){
        for (int j = 0; j< Y_MAX; j++){
            visited[{i, j}] = false;
        }
    }
    matrix[X][Y] = 'I'; // initialize infinity stone cell
    matrix[0][0] = '.'; // initialize
    fillMap({0, 0});
}

int main() {
    cin >> perceptionZone;
    cin >> X >> Y;
    backtracking_search();
    fillingGrid({0, 0}, {-1, -1});
    //Now calculate the shortest path
    vector<vector<int>> shortestPath(X_MAX, vector<int>(Y_MAX, INT_MAX)); // Initialize shortestPath with INT_MAX
    shortestPath[0][0] = 0;
    //initialize set for saving coordinates and length from (0,0) to current coordinates
    set<Cell> set;
    set.insert({0, {0,0}});// insert start position
    while(!set.empty()){
        //pop the smallest length in the set
        Cell current = *(set.begin());
        set.erase(set.begin()); //delete this Point
        //next check 4 directions, where Thanos can move
        for (int i = 0; i < 4; i++){
            pair<int, int> coord = current.second;
            int x = coord.first + dx[i];
            int y = coord.second + dy[i];
            //consider all direction, if next zone is empty or infinity stone, then calculate for this coordinates
            if (isValidCoordinate(x, y) && (matrix[x][y] == '.' || matrix[x][y] == 'I')){
                //optimize length of the path and add in the set
                if (shortestPath[x][y] > shortestPath[coord.first][coord.second] + 1){
                    set.erase({shortestPath[x][y], {x, y}});
                    shortestPath[x][y] = shortestPath[coord.first][coord.second] + 1;
                    set.insert({shortestPath[x][y],{x, y}});
                }
            }
        }
    }
    //If the length to the infinity stone did not change, then print e -1, otherwise print length path
    if (shortestPath[X][Y] == INT_MAX){
        cout << "e -1";
    } else {
        cout << "e " << shortestPath[X][Y];
    }
    return 0;
}
