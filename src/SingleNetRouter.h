# include <iostream>
# include <vector>

using namespace std;

class SingleNetRouter
{
    public:
        SingleNetRouter(int _xStart, int _yStart, int _xEnd, int _yEnd, vector<vector<int>> _supplies, vector<vector<int>> _demands, int _numrows, int _numcols, int _overflow, double _lambda) 
        {
            xStart = _xStart;
            yStart = _yStart;
            xEnd = _xEnd;
            yEnd = _yEnd;
            lambda = _lambda;
            numRows = _numrows;
            numCols = _numcols;
            totaloverflow = _overflow;
            supplies.assign(_supplies.begin(), _supplies.end());
            demands.assign(_demands.begin(), _demands.end());
            gridnodesorigin.resize(numRows*numCols, -1);
            gridgScore.resize(numRows*numCols, 10000000);
            gridfScore.resize(numRows*numCols, 10000000);
        }

        void AstarRouting();
        int calHPWL(int xStart, int yStart, int xEnd, int yEnd);
        void reconstruct_path();

        int xStart;
        int yStart;
        int xEnd;
        int yEnd;

        vector<vector<int>> supplies;
        vector<vector<int>> demands;
        vector<vector<int>> trails;
        int numRows;
        int numCols;
        int totaloverflow;
        vector<int> gridnodesorigin;
        vector<int> gridgScore;
        vector<int> gridfScore;

        double lambda;
};

