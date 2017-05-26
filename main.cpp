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
//    medianBlur(image, image, 13);

    //    imshow("mask", mask);

    Mat image_64F;
    image.convertTo(image_64F, CV_64FC1);
    //    image_64F = image_64F;

    Mat gI;
    Mat gI_64F(image_64F.size(), image_64F.type(), cv::Scalar(0));
    double alpha = 2;
    double sigma = 3;

    begin_time = getTickCount();

    gborders(image_64F, gI_64F, alpha, sigma);

    int num_smoothing = 1;
    double gI_threshold = 0.17;
    double gI_balloon_v = 1;
    int num_iters = 100;

    Mat mask_64F;
    mask.convertTo(mask_64F, image_64F.type());

    cv::Mat gI_threshold_mask(gI_64F.size(), CV_8UC1, cv::Scalar(0));
    get_gI_threshold_mask(gI_64F, gI_threshold_mask, gI_threshold, gI_balloon_v);

    //    imshow("gI_threshold_mask", gI_threshold_mask);

    cv::Mat derivative_gI_X(image_64F.size(), image_64F.type(), cv::Scalar(0));
    cv::Mat derivative_gI_Y(image_64F.size(), image_64F.type(), cv::Scalar(0));
    calc_derivative(gI_64F, derivative_gI_X, derivative_gI_Y);

    for(int i=0; i<num_iters; ++i)
    {
        mask_64F.convertTo(mask_64F, CV_64FC1);

        // step.1 & step.2
        morph_gac(gI_threshold_mask, derivative_gI_X, derivative_gI_Y, mask_64F, gI_balloon_v);
//        imshow("mask_64F", mask_64F);

        mask_64F.convertTo(mask_64F, CV_8UC1);

        // step.3   没有起到应有的作用，原因未知。
//        for(int j=0; j<num_smoothing; j++)
//        {
//            SIoIS(mask_64F);
//        }

        // replace step.3
        // smoothing curve
        medianBlur(mask_64F, mask_64F, 3);

        Canny(mask_64F, mask, 30, 120);
        threshold(mask, mask, 255, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
        Mat res;
        image.copyTo(res, mask);
        imshow("res", res);
        waitKey(100);
    }

    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();

    cout << "total time: " << (int)t << " ms" << endl ;

    cout << "finish" << endl;

    //    imshow("gI", gI_64F);

    waitKey(30000);

    return 0;
}

