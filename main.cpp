#include <iostream>
#include <opencv2/opencv.hpp>
#include "morph_snakes.h"

using namespace std;
using namespace cv;

int main()
{
    double begin_time;
    double end_time;
    double t;
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
    Mat gI_64F(image_64F.size(), image_64F.type(), cv::Scalar(0));
    double alpha = 2;
    double sigma = 2;

    begin_time = getTickCount();

    gborders(image_64F, gI_64F, alpha, sigma);

//    for(int i=0; i<image.rows; ++i)
//    {
//        for(int j=0; j<image.cols; ++j)
//        {
//            printf("%d,%d: %f\n", i, j, image_64F.at<double>(i,j));
//        }
//    }

//    gI_64F.convertTo(gI, CV_8UC1);


//    imwrite("E:/0-myWorkSpace/matlab_IrisianCapture_exp/result.bmp", gI_64F);

    int num_smoothing = 1;
    double gI_threshold = 0.17;
    double gI_balloon_v = 1;
    int num_iters = 80;

    Mat mask_64F;
    mask.convertTo(mask_64F, image_64F.type());

    cv::Mat gI_threshold_mask(gI_64F.size(), CV_8UC1, cv::Scalar(0));
    get_gI_threshold_mask(gI_64F, gI_threshold_mask, gI_threshold, gI_balloon_v);

//    imshow("gI_threshold_mask", gI_threshold_mask);

    cv::Mat derivative_gI_X(image_64F.size(), image_64F.type(), cv::Scalar(0));
    cv::Mat derivative_gI_Y(image_64F.size(), image_64F.type(), cv::Scalar(0));
    calc_derivative(gI_64F, derivative_gI_X, derivative_gI_Y);

    cvtColor(image, image, CV_GRAY2BGR);

    for(int i=0; i<num_iters; ++i)
    {
        morph_gac(gI_threshold_mask, derivative_gI_X, derivative_gI_Y, mask_64F, gI_balloon_v);
        imshow("mask_64F", mask_64F);

        mask_64F.convertTo(mask, CV_8UC1);
        Canny(mask, mask, 30, 120);
        threshold(mask, mask, 255, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
        Mat image_res;
        image.copyTo(image_res, mask);
        imshow("image_res", image_res);
        waitKey(100);
    }

    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    cout << "total time: " << (int)t << " ms" << endl ;

    cout << "finish" << endl;

//    imshow("gI", gI_64F);


    imshow("mask_64F", mask_64F);

    waitKey(30000);

    return 0;
}

