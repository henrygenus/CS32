#include <string>
#include <iostream>
#include <cassert>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

/*
int main()
{
    string maze[5] = {
        "XXXXX",
        "X...X",
        "X.XXX",
        "X.X.X",
        "XXXXX"
    };
    assert( ! pathExists(maze, 5,5, 1,1, 3,3));
    
    string maze1[5] = {
        "XXXXX",
        "X...X",
        "X.XXX",
        "X.X.X",
        "XXXXX"
    };
    assert(pathExists(maze1, 5,5, 1,1, 1,1));
    
    string maze2[5] = {
        "XXXXX",
        "X...X",
        "X.XXX",
        "X.X.X",
        "XXXXX"
    };
    assert(pathExists(maze2, 5,5, 1,1, 3,1));
    
    string maze3[5] = {
        "XXXXX",
        "X...X",
        "X.XXX",
        "X.X.X",
        "XXXXX"
    };
   assert(pathExists(maze3, 5,5, 1,1, 1,3));
    
    string maze4[10] = {
        "XXXXXXXXXX",
        "X....X...X",
        "X.XX.XX..X",
        "XXX....X.X",
        "X.XXX.XXXX",
        "X.X...X..X",
        "X...X.X..X",
        "XXXXX.X.XX",
        "X........X",
        "XXXXXXXXXX"
    };
    assert(pathExists(maze4, 10,10, 3,5, 8,8));
    
    cerr << "All Tests Passed." << endl;
}
*/
int main()
{
string maze1[10] = {
    "XXXXXXXXXX",
    "X...X....X",
    "XX.X..XX.X",
    "XX...X...X",
    "X.X.X..XXX",
    "X...X.X.XX",
    "X.X.X....X",
    "XXX.X.XX.X",
    "X...X..X.X",
    "XXXXXXXXXX",
};

string maze2[10] = {
    "XXXXXXXXXX",
    "X...X....X",
    "XX.X..XX.X",
    "XX...X...X",
    "X.X.X..XXX",
    "X...X.X.XX",
    "X.X.X....X",
    "XXX.X.XXXX",
    "X...X..X.X",
    "XXXXXXXXXX",
};

string maze3[10] = {
    "XXXXXXXXXX",
    "XXXXXXX..X",
    "XX.......X",
    "X..X....XX",
    "X..X..XXXX",
    "X..XXX.X.X",
    "XX...X...X",
    "X....XX..X",
    "XXX.....XX",
    "XXXXXXXXXX",
};

string maze4[10] = {
    "XXXXXXXXXX",
    "XXXXXXX..X",
    "XX.....X.X",
    "X..X....XX",
    "X..X..XXXX",
    "X..XXX.X.X",
    "XX...X...X",
    "X....XX..X",
    "XXX.....XX",
    "XXXXXXXXXX",
};

assert(pathExists(maze1, 10, 10, 1, 3, 8, 8));
assert(!pathExists(maze2, 10, 10, 1, 3, 8, 8));
assert(pathExists(maze3, 10, 10, 5, 6, 2, 8));
assert(!pathExists(maze4, 10, 10, 5, 6, 2, 8));
}
