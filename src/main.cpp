# include "string.h"
# include "Database.h"
# include "MazeRouter.h"
# include "ctime"

using namespace std;

int main(int argc, char *argv[])
{
    string netlistfile = argv[1];
    string outfile = argv[2];
    // Read the benchmark
    std::clock_t c_start = std::clock();
    Database database;
    database.readNetList(netlistfile);
    MazeRouter router(database);
    router.constructRouteGraph();
    router.calNetOrder();
    router.mazerouting();
    router.WriteRoutingResults(outfile);
    std::clock_t c_end = std::clock();
    std::cout<<"Runtime (s): "<<(c_end - c_start)*1.0/CLOCKS_PER_SEC<<std::endl;

    return 0;
}