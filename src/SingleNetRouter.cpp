#include "SingleNetRouter.h"
#include <queue>
#include <utility>
#include "math.h"
#include <algorithm>

using namespace std;


void SingleNetRouter::AstarRouting()
{
    // key - fScore, value - gridnodeid

    struct cmp
    {
        bool operator()(pair<int,int> a, pair<int,int> b)
        {
            return a.first > b.first;
        }
    };

    priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> q;

    int initHPWL = calHPWL(xStart, yStart, xEnd, yEnd);
    q.push(make_pair((1-lambda)*initHPWL + totaloverflow*lambda, xStart*numRows + yStart));

    vector<bool> gridnodevisited(numRows*numCols, false);

    // gridgScore
    gridgScore[xStart*numRows + yStart] = 0;
    gridfScore[xStart*numRows + yStart] = initHPWL*(1-lambda) + totaloverflow * lambda;

    while (!q.empty())
    {
        auto gridnodepair = q.top();
        
        int gridnodeid = gridnodepair.second;
        int gridnodeX = gridnodeid / numRows;
        int gridnodeY = gridnodeid % numRows;

        int current_gScore = gridgScore[gridnodeid];

        if ((gridnodeX ==  xEnd) && (gridnodeY == yEnd))
            break;
        
        q.pop();
        gridnodevisited[gridnodeid] = true;

        for (int i = -1; i <= 1; i++)
        {

            if ((gridnodeX + i) < 0 || (gridnodeX + i) >= numCols)
                continue;

            for (int j = -1; j <= 1; j++)
            {
                if((gridnodeY + j) < 0 || (gridnodeY + j) >= numRows)
                    continue;
                // i == j or i == -j
                if ((i == j) || (i == -j))
                    continue;
                
                int gridnodeid_neighbor = (gridnodeX + i) * numRows + (gridnodeY + j);
                int tentative_Score = current_gScore + 1 * (1-lambda);

                // horizontal
                if (i == -1 || i == 1)
                {
                    int supply_edge = supplies[2*min(gridnodeX, gridnodeX+i)+1][gridnodeY];
                    int demand_edge = demands[2*min(gridnodeX, gridnodeX+i)+1][gridnodeY];
                    if (demand_edge > supply_edge)
                        tentative_Score += (demand_edge - supply_edge) * lambda;
                }
                else
                {
                    // vertical
                    int supply_edge = supplies[2*gridnodeX][min(gridnodeY, gridnodeY+j)];
                    int demand_edge = demands[2*gridnodeX][min(gridnodeY, gridnodeY+j)];
                    if (demand_edge > supply_edge)
                        tentative_Score += (demand_edge - supply_edge) * lambda;
                }

                if (tentative_Score < gridgScore[gridnodeid_neighbor])
                {
                    gridnodesorigin[gridnodeid_neighbor] = gridnodeid;
                    gridgScore[gridnodeid_neighbor] = tentative_Score;
                    gridfScore[gridnodeid_neighbor] = tentative_Score + calHPWL(gridnodeX+i, gridnodeY+j, xEnd, yEnd);

                    if (!gridnodevisited[gridnodeid_neighbor])
                        q.push(make_pair(gridfScore[gridnodeid_neighbor], gridnodeid_neighbor));
                }
            }
        } 
    }

    // reconstruct the path from the end nodes
    reconstruct_path();
}


void SingleNetRouter::reconstruct_path()
{
    int xTrasStart = xEnd;
    int yTrasStart = yEnd;
    int TrasIndex;

    while (xTrasStart != xStart || yTrasStart != yStart)
    {
        TrasIndex = xTrasStart *numRows + yTrasStart;
        int SrcIndex = gridnodesorigin[TrasIndex];
        int SrcX = SrcIndex / numRows;
        int SrcY = SrcIndex % numRows;

        vector<int> Srccorr = {SrcX, SrcY, xTrasStart, yTrasStart};
        trails.push_back(Srccorr);

        xTrasStart = SrcX;
        yTrasStart = SrcY;
    }
}

int SingleNetRouter::calHPWL(int xStart, int yStart, int xEnd, int yEnd)
{
    return abs(xEnd - xStart) + abs(yEnd - yStart);
}




