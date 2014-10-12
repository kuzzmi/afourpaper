#include "Image.h"
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

void readFile(const std::string& filename, std::vector<char>& data)
{
    std::ifstream fs(filename.c_str(), std::fstream::in | std::fstream::binary);
    fs.seekg (0, fs.end);
    int length = fs.tellg();
    fs.seekg (0, fs.beg);

    data.resize(length);

    for (int i = 0; i < length; ++i)
        data[i] = fs.get();
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "Usage: encode path filename outputfile\n";
        std::exit(1);
    }

    // Reading command-line arguments
    std::string path = argv[1];
    std::string filename = argv[2];
    std::string output = argv[3];

    // Reading data
    std::vector<char> data;
    readFile(path, data);

    // Generate image
    Image image(filename.size(), data.size());
    image.WriteData(filename, data);
    image.WriteImage(output);

    std::cout << output << "\n";

    return 0;
}
