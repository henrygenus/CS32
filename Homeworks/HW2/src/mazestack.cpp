#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

void encountered(stack<Coord> &coordStack, string maze[], int r, int c)
{
    maze[r][c] = '*';
    coordStack.push(Coord(r, c));
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    if (sr < 0 || er < 0 || sr >= nRows || er >= nRows)
        return false;
    if (sc < 0 || ec < 0 || sc >= nCols || ec >= nCols)
        return false;

    stack<Coord> coordStack;
    coordStack.push(Coord(sr,sc));
    maze[sr][sc] = '*';
    while( !coordStack.empty())
    {
        Coord current = coordStack.top();
        if (current.r() == er && current.c() == ec)
            return true;
        else
        {
            if (maze[current.r()+1][current.c()] == '.')
                encountered(coordStack, maze, current.r()+1, current.c());
            
            else  if (maze[current.r()][current.c()-1] == '.')
                encountered(coordStack, maze, current.r(), current.c()-1);
            
            else  if (maze[current.r()-1][current.c()] == '.')
                encountered(coordStack, maze, current.r()-1, current.c());
            
            else  if (maze[current.r()][current.c()+1] == '.')
                encountered(coordStack, maze, current.r(), current.c()+1);
            else
                coordStack.pop();
        }
    }
    return false;
}

int test(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    for (int i = 0; i < nRows; i++)
      cout << maze[i] << endl;
    return pathExists(maze, nRows, nCols, sr, sc, er, ec);
}

int main()
{
    string maze[5] = {
        "XXXXX",
        "X...X",
        "X.XXX",
        "X.X.X",
        "XXXXX"
    };
    assert(!pathExists(maze, 0, 1, 1, 1, 1, 1));
    assert(!pathExists(maze, 1, 0, 1, 1, 1, 1));
    assert(!pathExists(maze, 1, 1, 0, 1, 1, 1));
    assert(!pathExists(maze, 1, 1, 1, 0, 1, 1));
    assert(!pathExists(maze, 1, 1, 1, 1, 0, 1));
    assert(!pathExists(maze, 1, 1, 1, 1, 1, 0));
    assert(!pathExists(maze, 5,5, 1,1, 3,3));
    
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
    assert(pathExists(maze4, 10, 10, 3, 5, 8, 8));
    cout << "Tests Passed" << endl;
}
