#include "tools.h"
#include <string>
#include <vector>

int main()
{
    std::string filename = "hello.txt";
    std::string datastr = "Hello world";
    std::vector<char> data;
    for (int i = 0; i < datastr.size(); ++i)
        data.push_back(datastr[i]);

    Image image(filename.size(), data.size());
    image.WriteData(filename, data);
    image.WriteImage("/home/spiros/mynewimage.png");
}
