#include "Polygon.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Not enough data files" << std::endl;
        return -1;
    }

    std::string filename = argv[1];
    std::vector<Polygon> polygons;
    try
    {
        polygons = readPolygonsFromFile(filename);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return -1;
    }

    processCommands(polygons);

    return 0;
}