#include "MazeRouter.h"
#include <utility>
#include <algorithm>
#include "SingleNetRouter.h"
#include <fstream>

using namespace std;

void MazeRouter::constructRouteGraph()
{
    // initialize the routing graph
    routegraph.resize(2*database.Numgridcols - 1);
    supplies.resize(2*database.Numgridcols - 1);
    demands.resize(2*database.Numgridcols - 1);
    
    for(int i = 0; i < routegraph.size(); i++)
    {
        vector<GridEdge> edge;
        vector<int> supplyper;
        vector<int> demandper;
        bool vertical = (i % 2 == 0);
        if (vertical)
        {
            for (int j = 0; j < database.Numgridrows - 1; j++)
            {
                vector<int> startgridnode = {i/2, j};
                vector<int> endgridnode = {i/2, j+1};
                edge.emplace_back(startgridnode, endgridnode, database.vCapacity, vertical);
                supplyper.push_back(database.vCapacity);
                demandper.push_back(0);
            }
        }
        else
        {
            for (int j = 0; j < database.Numgridrows; j++)
            {
                vector<int> startgridnode = {i/2, j};
                vector<int> endgridnode = {i/2 + 1, j};
                edge.emplace_back(startgridnode, endgridnode, database.hCapacity, vertical);
                supplyper.push_back(database.hCapacity);
                demandper.push_back(0);
            }
        }

        routegraph[i] = edge;
        supplies[i] = supplyper;
        demands[i] = demandper;
    }
}


void MazeRouter::calNetOrder()
{
    vector<pair<int, int>> hpwlcol;

    for(int i = 0; i < database.nets.size(); i++)
    {
        int netid = database.nets[i].getId();
        int hpwl = database.nets[i].getHPWL();
        hpwlcol.push_back(make_pair(hpwl, netid));
    }

    auto compare = [] (pair<int, int> p1, pair<int, int> p2)
    {
        return p1.first < p2.first;
    };

    sort(hpwlcol.begin(), hpwlcol.end(), compare);

    for(int i = 0 ; i < hpwlcol.size(); i++)
    {
        netorder.push_back(hpwlcol[i].second);
    }

    // // output the net orderings
    // cout<< "Net orderings: "<<endl;
    // for (int i = 0; i < netorder.size(); i++)
    // {
    //     cout << netorder[i] << ' '<<hpwlcol[i].first<<std::endl;
    // }
    // cout<<endl;

}

// maze routing
void MazeRouter::mazerouting()
{
    double init_lambda = 0.0; // The ratio for the wirelength and the overflow
    double lambda = init_lambda;
    for (int i = 0; i < netorder.size(); i++)
    {
        auto net = database.nets[netorder[i]];
        // lambda
        if (i % 200 == 0)
        {
            lambda += 0.1;
            if (lambda >= 1.0)
                lambda = 1.0;
        }

        // (xStart, yStart, xEnd, yEnd)
        int xStart = database.pins[net.pinIds[0]].getLocX();
        int yStart = database.pins[net.pinIds[0]].getLocY();
        int xEnd = database.pins[net.pinIds[1]].getLocX();
        int yEnd = database.pins[net.pinIds[1]].getLocY();

        SingleNetRouter netrouter(xStart, yStart, xEnd, yEnd, supplies, demands, database.Numgridrows, database.Numgridcols, totaloverflow, lambda);

        netrouter.AstarRouting();

        // update the routing information
        totalwirelength += netrouter.trails.size();

        int incremental = 0;
        
        for(auto seg : netrouter.trails)
        {
            if (seg[0] == seg[2])
            {
                demands[2*seg[0]][min(seg[1], seg[3])] += 1;
                if (supplies[2*seg[0]][min(seg[1], seg[3])] < demands[2*seg[0]][min(seg[1], seg[3])])
                {
                    totaloverflow += 1;
                    incremental += 1;
                }
            }
            else
            {
                demands[2*min(seg[0], seg[2])+1][seg[1]] += 1;
                if(supplies[2*min(seg[0], seg[2])+1][seg[1]] < demands[2*min(seg[0], seg[2])+1][seg[1]])
                {
                    totaloverflow += 1;
                    incremental += 1;
                }
            }
        }

        reverse(netrouter.trails.begin(), netrouter.trails.end());
        trailCol.push_back(netrouter.trails);

        std::cout<<"net: "<<netorder[i]<<", wirelength: "<<netrouter.trails.size()<<", hpwl: "<<net.getHPWL()<<", overflow:"<<incremental<<std::endl;
        std::cout<<"Total wirelength: "<<totalwirelength<<", total overflow: "<<totaloverflow<<std::endl;
    }
}


void MazeRouter::WriteRoutingResults(string outfileName)
{
    ofstream outfile;
    outfile.open(outfileName);

    

    for(int i = 0; i < netorder.size(); i++)
    {
        string netName = database.nets[netorder[i]].getName();
        int netId = database.nets[netorder[i]].getId();
        outfile << netName << " " << netId << " " <<endl;

        for (int j = 0; j < trailCol[i].size(); j++)
        {
            outfile << "("<< trailCol[i][j][0] <<", "<<trailCol[i][j][1]<<", 1)-";
            outfile << "("<< trailCol[i][j][2] <<", "<<trailCol[i][j][3]<<", 1)"<<endl;
        }
        outfile <<"!"<<endl;

    }
}
