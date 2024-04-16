#include <fstream>
#include <sstream>
#include "Database.h"
#include "string.h"

using namespace std;

void Database::readNetList(string fileName)
{
    ifstream fin(fileName);
    string line;

    while(getline(fin, line))
    {
        //std::cout<<line<<std::endl;
        line.erase(0, line.find_first_not_of(' '));
        istringstream iss(line);
        string fsymbol;
        getline(iss, fsymbol, ' ');
        if (fsymbol.find("grid") != fsymbol.npos)
        {
            string NumgridcolStr, NumgridrowStr;
            iss >> NumgridcolStr >> NumgridrowStr;
            Numgridcols = stoi(NumgridcolStr);
            Numgridrows = stoi(NumgridrowStr);
        }
        else if (fsymbol.find("vertical") != fsymbol.npos)
        {
            string vSymbol, vCapacityStr;
            iss >> vSymbol >> vCapacityStr;
            vCapacity = stoi(vCapacityStr);
        }
        else if (fsymbol.find("horizontal") != fsymbol.npos)
        {
            string hSymbol, hCapacityStr;
            iss >> hSymbol >> hCapacityStr;
            hCapacity = stoi(hCapacityStr);
        }
        else if (fsymbol.find("num") != fsymbol.npos)
        {
            string nSymbol, numNetsStr;
            iss >> nSymbol >> numNetsStr;
            num_nets = stoi(numNetsStr);
        }
        else if (fsymbol.find("net") != fsymbol.npos)
        {
            string netName = fsymbol;
            string idStr, NumPinStr;
            iss >> idStr >> NumPinStr;
            nets.emplace_back(netName, stoi(idStr));
            netName2Id[netName] = stoi(idStr);
        }
        else
        {
            string PinLocXStr, PinLocYStr;
            vector<int> PinLoc;
            PinLocXStr = fsymbol;
            getline(iss, PinLocYStr);
            nets.back().addPinId(pins.size());
            pins.emplace_back(pins.size(), stoi(PinLocXStr), stoi(PinLocYStr));
            pins.back().addNetId(nets.size() - 1);
        }
    }

    std::cout<<"Number of the Pins: "<<pins.size()<<std::endl;
    std::cout<<"Number of the Nets: "<<nets.size()<<std::endl;
    std::cout<<"Grid Width: "<<Numgridcols<<", Grid Height: "<<Numgridrows<<std::endl;
    std::cout<<"vertical capacity: "<<vCapacity<<", horizontal capacity: "<<hCapacity<<std::endl;

    calHPWL();
}


void Database::calHPWL()
{
    for (int i = 0; i < nets.size(); i++)
    {
        int leftcorr = 1000000;
        int rightcorr = -1;
        int downcorr = 1000000;
        int upcorr = -1;

        for (int j = 0; j < nets[i].pinIds.size(); j++)
        {
            int pinid = nets[i].pinIds[j];
            int locX = pins[pinid].getLocX();
            int locY = pins[pinid].getLocY();

            if (locX < leftcorr)  leftcorr = locX;
            if (locX > rightcorr) rightcorr = locX;
            if (locY < downcorr)   downcorr = locY;
            if (locY > upcorr)  upcorr = locY;
        }

        int hpwl = (rightcorr - leftcorr) + (upcorr - downcorr);
        nets[i].setHPWL(hpwl);
    }
}