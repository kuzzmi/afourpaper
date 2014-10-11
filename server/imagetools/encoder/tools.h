#pragma once

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "lodepng.h"

class Image
{
public:
    Image(int namebytes, int databytes)
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


    void WriteData(const std::string& filename, const std::vector<char>& data);

    void WriteImage(const std::string& path) const
    {
        unsigned error = lodepng::encode(path.c_str(), pixels_, width_, height_);
        if(!error)
            std::cout << "Image " << path << " saved correctly\n";
    }

    int height() const { return height_; }
    int width() const { return width_; }

private:
    std::vector<unsigned char> pixels_;
    int height_, width_;

    void WriteInto(int row, int col, bool value)
    {
        if (row >= height_)
            std::cerr << "Error: wrong row\n";
        if (col >= width_)
            std::cerr << "Error: wrong col\n";
        int offset = 4*(width_*row + col);
        pixels_[offset+0] = value ? 0 : 255;
        pixels_[offset+1] = value ? 0 : 255;
        pixels_[offset+2] = value ? 0 : 255;
        pixels_[offset+3] = 255;
    }

    void SetupCorners()
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

    void SetupDotted()
    {
        // Top line
        for (int c = 4; c < width_-4; ++c)
            WriteInto(0, c, c%2==0);

        // Side line
        for (int r = 4; r < height_-4; ++r)
            WriteInto(r, 0, r%2==0);
    }
};

