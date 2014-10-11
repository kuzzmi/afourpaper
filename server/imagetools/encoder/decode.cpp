#include "Image.h"
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: decode path outputdir\n";
        std::exit(1);
    }

    // Reading command-line arguments
    std::string path = argv[1];
    std::string outputdir = argv[2];

    // Append slash
    if (outputdir[outputdir.size()-1] != '/')
        outputdir.push_back('/');

    // Read image and parse data
    std::string filename;
    std::vector<char> data;
    Image image(path);
    image.GetData(filename, data);

    // Write file
    filename = outputdir+filename;
    std::ofstream fs(filename.c_str(), std::fstream::out | std::fstream::binary);
    fs.write(data.data(), data.size());
    fs.close();

    std::cout << filename << "\n";

    return 0;
}
