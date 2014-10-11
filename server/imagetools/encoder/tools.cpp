#include "tools.h"
#include <deque>

void Image::WriteData(const std::string& filename, const std::vector<char>& data)
{
    int m = data.size();
    int namelength = filename.size();

    std::cout << "data size is " << m << std::endl;

    // Fill predata
    const int predatasize = 4+2+namelength;
    std::vector<char> predata(predatasize);
    predata[0] = m >> 24;
    predata[1] = m >> 16;
    predata[2] = m >>  8;
    predata[3] = m      ;
    predata[4] = namelength >> 8;
    predata[5] = namelength     ;
    for (int i = 0; i < namelength; ++i)
    {
        predata[i+6] = filename[i];
    }

    m += predatasize;

    std::cout << "total size is " << m << std::endl;

    // Fill boolean values
    std::vector<bool> values(m*8);
    int v = 0;
    int n = 0;
    for (int i = 0; i < m; ++i)
    {
        const char& byte = n < predatasize ? predata[n] : data[n-predatasize];
        for (int b = 7; b >= 0; --b)
        {
            values[v] = (byte >> b) % 2;
            ++v;
        }
        ++n;
    }
    m *= 8; // Now m is the length of the bit values vector

    // Now write the boolean values into the image

    n = 0;

    // First portion: three lines at the top
    for (int r = 1; r < 4; ++r)
        for (int c = 4; c < width_-4; ++c)
        {
            if (n >= m)
                break;
            WriteInto(r, c, values[n]);
            ++n;
        }

    // Second portion: main part
    for (int r = 4; r < height_-4; ++r)
        for (int c = 1; c < width_; ++c)
        {
            if (n >= m)
                break;
            WriteInto(r, c, values[n]);
            ++n;
        }

    // Third portion: three bottom lines
    for (int r = height_-4; r < height_; ++r)
        for (int c = 4; c < width_-4; ++c)
        {
            if (n >= m)
                break;
            WriteInto(r, c, values[n]);
            ++n;
        }

    return;
}
