#ifndef History_hpp
#define History_hpp

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    ~History();
    
private:
    char** m_Grid;
    int m_Rows;
    int m_Cols;
};

#endif /* History_h */

