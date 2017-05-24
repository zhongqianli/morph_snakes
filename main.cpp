#include <iostream>
#include <opencv2/opencv.hpp>
#include "morph_snakes.h"

using namespace std;
using namespace cv;

int main()
{
    string imagepath = "image.bmp";
    string maskpath = "image_mask.bmp";

    Mat image = imread(imagepath, CV_LOAD_IMAGE_GRAYSCALE);
    Mat mask = imread(maskpath, CV_LOAD_IMAGE_GRAYSCALE);

    if(image.empty())
    {
        cout << "read image failed." << endl;
        return -1;
    }

//    imshow("image", image);

    if(mask.empty())
    {
        cout << "read mask failed." << endl;
        return -1;
    }

    // median blur
    medianBlur(image, image, 13);

//    imshow("mask", mask);

    Mat image_64F;
    image.convertTo(image_64F, CV_64FC1);
//    image_64F = image_64F;

    Mat gI;
    Mat gI_64F(image.size(), CV_64FC1, cv::Scalar(0));
    double alpha = 2;
    double sigma = 2;

    gborders(image_64F, gI_64F, alpha, sigma);

//    for(int i=0; i<image.rows; ++i)
//    {
//        for(int j=0; j<image.cols; ++j)
//        {
//            printf("%d,%d: %f\n", i, j, image_64F.at<double>(i,j));
//        }
//    }

//    gI_64F.convertTo(gI, CV_8UC1);

    imwrite("E:/0-myWorkSpace/matlab_IrisianCapture_exp/result.bmp", gI_64F);
    cout << "finish" << endl;

    imshow("gI", gI_64F);



    waitKey(30000);

    return 0;
}

