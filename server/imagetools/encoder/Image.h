#pragma once

#include <vector>
#include <cmath>
#include <string>
#include <iostream>

class Image
{
public:
    Image(const std::string& imageFile);

    Image(int namebytes, int databytes);


    void WriteData(const std::string& filename, const std::vector<char>& data);

    void WriteImage(const std::string& path) const;

    void GetData(std::string& filename, std::vector<char>& data) const;

private:
    std::vector<unsigned char> pixels_;
    int height_, width_;
    int datasize, filenamesize;

    inline void WriteInto(int row, int col, bool value)
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

    inline bool ReadFrom(int row, int col) const
    {
        if (row >= height_)
            std::cerr << "Error: wrong row\n";
        if (col >= width_)
            std::cerr << "Error: wrong col\n";
        int offset = 4*(width_*row + col);
        return pixels_[offset+0] < 123 ? true : false;
    }

    void SetupCorners();

    void SetupDotted();
};

