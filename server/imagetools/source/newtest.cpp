#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <cstdio>
#include <cmath>

using namespace cv;

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "No image data" << std::endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if(!image.data)
    {
        std::cout << "Could not open or find image" << std::endl;
        return -1;
    }

    // Increase the contrast
    int xdist = 205 - 50; // indent as index from 0 to 256, for the above picture: indentRight: 235, indentLeft: 15
    double alpha = 256.0 / xdist;
    int beta = -(50 * alpha);
    image.convertTo(image, -1, alpha, beta);

    Size size = image.size();

    // Find left border
    int minc = 0;
    for (int c = 0; c < size.width; ++c)
    {
        bool found = false;
        for (int r = 0; r < size.height; ++r)
        {
            if(image.at<Vec3b>(r, c)[0] < 100)
            {
                found = true;
                minc = c;
                break;
            }
        }

        if (found)
            break;
    }

    // Find upper border
    int minr = 0;
    for (int r = 0; r < size.height; ++r)
    {
        bool found = false;
        for (int c = 0; c < size.width; ++c)
        {
            if(image.at<Vec3b>(r, c)[0] < 100)
            {
                found = true;
                minr = r;
                break;
            }
        }

        if (found)
            break;
    }

    // Find right border
    int maxc = 0;
    for (int c = size.width-1; c >= 0; --c)
    {
        bool found = false;
        for (int r = 0; r < size.height; ++r)
        {
            if(image.at<Vec3b>(r, c)[0] < 100)
            {
                found = true;
                maxc = c;
                break;
            }
        }

        if (found)
            break;
    }

    // Find bottom border
    int maxr = 0;
    for (int r = size.height-1; r >= 0; --r)
    {
        bool found = false;
        for (int c = 0; c < size.width; ++c)
        {
            if(image.at<Vec3b>(r, c)[0] < 100)
            {
                found = true;
                maxr = r;
                break;
            }
        }

        if (found)
            break;
    }

    // Find first black pixel in the diagonal
    for (int n = 0; true; ++n)
    {
        int r = minr + n;
        int c = minc + n;
        if(image.at<Vec3b>(r, c)[0] < 100)
        {
            minr = r;
            minc = c;
            break;
        }
    }


    // Cropping
    Rect cropRect(minc, minr, maxc-minc, maxr-minr);
    Mat croppedRef(image, cropRect);

    Mat cropped;
    croppedRef.copyTo(cropped);

    size = cropped.size();

    // Find white hole in pattern
    int n = 0;
    while (true)
    {
        if (cropped.at<Vec3b>(n, n)[0] > 180)
            break;
        ++n;
    }
    n /= 2;

    // Parse row n
    int flips = 0;
    int status = 0;
    int lastchange = 0;
    std::vector<int> cc;
    for (int c = 1; c < size.width; ++c)
    {
        int r = n;
        int val = cropped.at<Vec3b>(r, c  )[0];

        int newstatus = val < 100 ? 0 : (val < 160 ? 1 : 2);

        if (newstatus == 1 || newstatus == status)
            continue;

        std::cout << c - lastchange << "  ";
        if (newstatus == 0)
            std::cout << flips << "  Getting black at c=" << c << std::endl;

        if (newstatus == 2)
            std::cout << flips << "  Getting white at c=" << c << std::endl;

        cc.push_back(c);
        lastchange = c;
        ++flips;
        status = newstatus;
    }

    int nc = 3+flips;
    double leapc = (double)size.width/(nc);

    // Parse column n
    flips = 0;
    status = 0;
    for (int r = 1; r < size.height; ++r)
    {
        int c = n;
        int val = cropped.at<Vec3b>(r, c  )[0];

        int newstatus = val < 100 ? 0 : (val < 160 ? 1 : 2);

        if (newstatus == 1 || newstatus == status)
            continue;

        ++flips;
        status = newstatus;
    }

    int nr = 3+flips;
    double leapr = (double)size.height/(nr);

    std::cout << leapr << " " << leapc << "\n";

    // Add circles
    for (int r = 0; r < nr; ++r)
        for (int c = 0; c < nc; ++c)
        {
            Point center;
            center.x = leapc/2. + leapc*c;
            center.y = leapr/2. + leapr*r;
            circle(cropped, center, 5, Scalar(0,0,255));
        }

    imwrite("output.png", cropped);

    // Create result image
    Mat result = cvCreateImage(cvSize(3*nr,3*nc),8,3);
    for (int r = 0; r < nr; ++r)
        for (int c = 0; c < nc; ++c)
        {
            double rr = (r+.5)*leapr;
            double cc = (c+.5)*leapc;
            int value = cropped.at<Vec3b>(rr, cc)[0];
            value = value < 100 ? 0 : 255;
            result.at<Vec3b>(r, c) = Vec3b(value, value, value);
        }

    imwrite("result.png", result);
}

