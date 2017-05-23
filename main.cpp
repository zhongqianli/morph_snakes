#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    string imagepath = "image.bmp";
    string maskpath = "image_mask.bmp";

    Mat image = imread(imagepath);
    Mat mask = imread(maskpath);

    if(image.empty())
    {
        cout << "read image failed." << endl;
        return -1;
    }

    if(mask.empty())
    {
        cout << "read mask failed." << endl;
        return -1;
    }

    imshow("image", image);
    imshow("mask", mask);

    waitKey(30000);

    return 0;
}

