#include "Database.h"
#pragma once

using namespace std;


class GridEdge
{
    public:
        GridEdge(vector<int> _startgridnode, vector<int> _endgridnode, int _capacity, bool _vertical)
        {
            gridnodes.push_back(_startgridnode);
            gridnodes.push_back(_endgridnode);
            capacity = _capacity;
            vertical = _vertical;
        }
    
    vector<vector<int>> gridnodes;
    int capacity = 0;
    bool vertical;
};


class MazeRouter
{
    public:
        MazeRouter(Database _database): database(_database) {}
        void constructRouteGraph();
        void calNetOrder();
        void mazerouting();
        void WriteRoutingResults(string outfileName);
    
    Database database;
    vector<vector<GridEdge>> routegraph;
    vector<vector<int>> supplies;
    vector<vector<int>> demands;
    vector<int> netorder;
    double lambda;  // The ratio between the wirelength and the overflow

    vector<vector<vector<int>>> trailCol;

    int totaloverflow = 0; // total overflow
    int totalwirelength = 0; // total hpwl
};