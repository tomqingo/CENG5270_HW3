#include <iostream>
#include <vector>
#include "string.h"
#include <unordered_map>

#pragma once

using namespace std;

class Pin
{
    public:
        Pin(int _id, int _locX, int _locY): id(_id), locX(_locX), locY(_locY) {}
        void addNetId (int _netId)
        {
            netIds.push_back(_netId);
        }
        int getLocX() {return locX;}
        int getLocY() {return locY;}

        vector<int> netIds;

    private:
        int id;
        int locX;
        int locY;
};

class Net
{
    public:
        Net(string _name, int _id): id(_id), name(_name) {}
        void addPinId (int _pinId)
        {
            pinIds.push_back(_pinId);
        }
        void setHPWL(int _hpwl) {hpwl = _hpwl;}
        int getHPWL() {return hpwl;}
        int getId() {return id;}
        string getName() {return name;}
        int gethpwl() {return hpwl;}

        vector<int> pinIds;

    private:
        string name;
        int id;
        int hpwl = 0;
};

class Database
{
    public:
        void readNetList(string fileName);
        void constructRouteGraph();
        void calHPWL();

        int num_nets;
        vector<Pin> pins;
        vector<Net> nets;
        unordered_map<string, int> netName2Id;
        int Numgridrows;
        int Numgridcols;
        int vCapacity;
        int hCapacity;
};


