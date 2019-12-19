#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;
History::History(int nRows, int nCols)
{
    m_Rows = nRows; m_Cols = nCols;
    m_Grid = new char* [MAXROWS];
    for(int count = 0; count < MAXROWS; count++)
    {
        m_Grid[count] = new char[MAXCOLS];
    }
    for(int r = 0; r < nRows; r++)
    {
        for(int c = 0; c < nCols; c++)
        {
            m_Grid[r][c] = '.';
        }
    }
}

bool History::record(int r, int c)
{
    if(r > m_Rows || c > m_Cols)
        return false;
    switch(m_Grid[r][c])
    {
        case '.':   m_Grid[r][c] = 'A'; break;
        case 'Z':   break;
        default:    m_Grid[r][c]++; break;
    }
    return true;
}

void History::display() const
{
    // Draw the grid
    clearScreen();
    for (int r = 0; r < m_Rows; r++)
    {
        for (int c = 0; c < m_Cols; c++)
            cout << m_Grid[r][c];
        cout << endl;
    }
    cout << endl;
}

History::~History()
{
    for(int count = 0; count < MAXROWS; count++)
        delete[] m_Grid[count];
    delete[] m_Grid;
}
