#include "Image.h"
#include "lodepng.h"
#include <deque>

Image::Image(const std::string& imagePath)
{
    unsigned width, height;
    unsigned error = lodepng::decode(pixels_, width, height, imagePath.c_str());
    width_ = width;
    height_ = height;
}

Image::Image(int namebytes, int databytes)
{
    const double r2 = std::sqrt(2.);

    int t = 4*16        // corners
          + 2*8         // filename length
          + 4*8         // data length
          + namebytes*8 // name
          + databytes*8 // data
      ;

    // x is the temporary width
    int x = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(t)/r2)));

    // Now add the dotted lines
    t += (1. + r2) * static_cast<double>(x);

    // Finally compute the actual sizes
    width_ = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(t)/r2)));
    height_ = static_cast<int>(std::ceil(r2 * width_));

    // And allocate the memory
    pixels_.resize(4*height_*width_);

    SetupCorners();
    SetupDotted();
}

void Image::WriteImage(const std::string& path) const
{
    unsigned error = lodepng::encode(path.c_str(), pixels_, width_, height_);
}

void Image::SetupCorners()
{
    // Upper left corner
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
        {
            bool value = (r != 3) && (c != 3) && (r != 1 || c != 1);
            WriteInto(r, c, value);
        }

    // Upper right corner
    for (int r = 0; r < 4; ++r)
        for (int c = width_-4; c < width_; ++c)
        {
            bool value = r == 0 && (c % 2 == 0);
            WriteInto(r, c, value);
        }

    // Lower left corner
    for (int r = height_-4; r < height_; ++r)
        for (int c = 0; c < 4; ++c)
        {
            bool value = c == 0 && (r % 2 == 0);
            WriteInto(r, c, value);
        }


    // Lower right corner
    for (int r = height_-4; r < height_; ++r)
        for (int c = width_-4; c < width_; ++c)
        {
            bool value = c == (width_-1) && (r == height_-1);
            WriteInto(r, c, value);
        }
}

void Image::SetupDotted()
{
    // Top line
    for (int c = 4; c < width_-4; ++c)
        WriteInto(0, c, c%2==0);

    // Side line
    for (int r = 4; r < height_-4; ++r)
        WriteInto(r, 0, r%2==0);
}

void Image::WriteData(const std::string& filename, const std::vector<char>& data)
{
    int m = data.size();
    int namelength = filename.size();

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

void Image::GetData(std::string& filename, std::vector<char>& data) const
{
    std::vector<bool> values(width_*height_-64-(width_-8)-(height_-8));

    // Read into boolean vector
    int n = 0;

    // First portion: three lines at the top
    for (int r = 1; r < 4; ++r)
        for (int c = 4; c < width_-4; ++c)
        {
            values[n] = ReadFrom(r, c);
            ++n;
        }

    // Second portion: main part
    for (int r = 4; r < height_-4; ++r)
        for (int c = 1; c < width_; ++c)
        {
            values[n] = ReadFrom(r, c);
            ++n;
        }

    // Third portion: three bottom lines
    for (int r = height_-4; r < height_; ++r)
        for (int c = 4; c < width_-4; ++c)
        {
            values[n] = ReadFrom(r, c);
            ++n;
        }

    // Process data size
    int datasize = 0;
    for (int i = 0; i < 32; ++i)
    {
        datasize += (values[i] ? 1 : 0) << (31-i);
    }
    int offset = 32;

    // Process filename size
    int fnamesize = 0;
    for (int i = 0; i < 16; ++i)
    {
        fnamesize += (values[offset+i] ? 1 : 0) << (15-i);
    }
    offset += 16;

    // Read filename
    filename.resize(fnamesize);
    for (int i = 0; i < fnamesize; ++i)
    {
        char c = 0;
        for (int bit = 0; bit < 8; ++bit)
        {
            c += (values[offset+bit] ? 1 : 0) << (7-bit);
        }
        filename[i] = c;
        offset += 8;
    }

    // Read data
    data.resize(datasize);
    for (int i = 0; i < datasize; ++i)
    {
        char c = 0;
        for (int bit = 0; bit < 8; ++bit)
        {
            c += (values[offset+bit] ? 1 : 0) << (7-bit);
        }
        data[i] = c;
        offset += 8;
    }
}
