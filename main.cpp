#include <iostream>
#include <opencv2/opencv.hpp>
#include "morph_snakes.h"
#include <stdlib.h>

using namespace std;
using namespace cv;

#define SNAKE_DEBUG 1
//#define SNAKE_DEMO_DEBUG 1

int main(int argc, char* argv[])
{
    double total_begin_time;
    double total_end_time;
    double t;
#ifdef SNAKE_DEBUG
    double begin_time;
    double end_time;
#endif

    if(argc != 8)
    {
        printf("usage: %s <image_path> <mask_path> <alpha> <sigma> <gI_threshold> <gI_balloon_v> <num_iters>\n", argv[0]);
        return -1;
    }

    string imagepath = argv[1];
    string maskpath = argv[2];

    // g(I) parameter, 2
    double alpha = atof(argv[3]);
    // standard variance of gassian blur, 3
    double sigma = atof(argv[4]);

    // gI_binary_threshold, 0.17
    double gI_threshold = atof(argv[5]);
    // ballon force, gI_binary_threshold; 1 expand, -1 shrink;
    double gI_balloon_v = atof(argv[6]);

    // num of iteration
    int num_iters = atof(argv[7]);

    Mat image = imread(imagepath, CV_LOAD_IMAGE_GRAYSCALE);
    Mat mask = imread(maskpath, CV_LOAD_IMAGE_GRAYSCALE);

    if(image.empty())
    {
        printf("read image failed.\n");
        return -1;
    }

    if(mask.empty())
    {
        printf("read mask failed.\n");
        return -1;
    }

    cv::Mat image_copy = image.clone();

    // median blur
    medianBlur(image, image, 13);

    total_begin_time = getTickCount();
#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    Mat image_F;
    image.convertTo(image_F, SNAKE_DATA_TYPE);

    Mat gI_F(image_F.size(), SNAKE_DATA_TYPE);

    // g(I)
    gborders(image_F, gI_F, alpha, sigma);

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    printf("time of gborders: %d ms.\n", (int)t);
#endif

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    cv::Mat gI_threshold_mask(gI_F.size(), CV_8UC1, cv::Scalar(0));
    get_gI_threshold_mask(gI_F, gI_threshold_mask, gI_threshold, gI_balloon_v);

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    printf("time of get_gI_threshold_mask: %d ms.\n", (int)t);
#endif

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    cv::Mat derivative_gI_X(gI_F.size(), SNAKE_DATA_TYPE, cv::Scalar(0));
    cv::Mat derivative_gI_Y(gI_F.size(), SNAKE_DATA_TYPE, cv::Scalar(0));
    calc_derivative(gI_F, derivative_gI_X, derivative_gI_Y);

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    printf("time of calc_derivative: %d ms.\n", (int)t);
#endif

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    for(int i=0; i<num_iters; ++i)
    {
//        mask_F.convertTo(mask_F, SNAKE_DATA_TYPE);

        // step.1 & step.2
        // mask input type: CV_8UC1; mask output type: CV_8UC1
        morph_gac(gI_threshold_mask, derivative_gI_X, derivative_gI_Y, mask, gI_balloon_v);

//        // step.3 ; ISoSI can use in iterations or use it out of iterations
//          ISoSI(mask_F);

//        // medianBlur can replace ISoSI approximatively
//        // smoothing curve
//        medianBlur(mask_F, mask_F, 3);

//        mask_F.convertTo(mask_F, CV_8UC1);

#ifdef SNAKE_DEMO_DEBUG
        Canny(mask_F, mask, 30, 120);
        threshold(mask, mask, 255, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
        cv::Mat res;
        image.copyTo(res, mask);
        imshow("res", res);
        waitKey(200);
        printf("num_iters: %d\n", i+1);
#endif

//        imshow("mask", mask);
//        waitKey(20);

    }

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    printf("time of morph_gac iterations: %d ms.\n", (int)t);
#endif

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    // step.3 ;  smoothing curve
//    Mat mask_F;
//    mask.convertTo(mask_F, SNAKE_DATA_TYPE);
    // overflow ??? uchar double
    ISoSI(mask);

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    printf("time of ISoSI: %d ms.\n", (int)t);
#endif

    total_end_time = getTickCount();
    t = (total_end_time - total_begin_time)*1000 / getTickFrequency();
    printf("total time: %d ms.\n", (int)t);

    Mat res;
    mask.convertTo(mask, CV_8UC1);
    Canny(mask, mask, 30, 120);
    threshold(mask, mask, 255, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
    image_copy.copyTo(res, mask);
    imwrite("result.bmp", res);
    imwrite("result_mask.bmp", mask);

#if SNAKE_DEMO_DEBUG
    imshow("res", res);
#endif

    printf("finished.\n");

#if SNAKE_DEBUG || SNAKE_DEMO_DEBUG
    waitKey(30000);
#endif

    return 0;
}

