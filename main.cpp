#include <iostream>
#include <opencv2/opencv.hpp>
#include "morph_snakes.h"

using namespace std;
using namespace cv;

#define SNAKE_DEBUG
#define SNAKE_DEMO_DEBUG

int main()
{
#ifdef SNAKE_DEBUG
    double begin_time;
    double total_begin_time;
    double end_time;
    double t;
#endif

    string imagepath = "image.bmp";
    string maskpath = "image_mask.bmp";

    Mat image = imread(imagepath, CV_LOAD_IMAGE_GRAYSCALE);
    Mat mask = imread(maskpath, CV_LOAD_IMAGE_GRAYSCALE);

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

    // median blur
    medianBlur(image, image, 13);

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
    total_begin_time = begin_time;
#endif

    Mat image_F;
    image.convertTo(image_F, SNAKE_DATA_TYPE);

    Mat gI_F(image_F.size(), SNAKE_DATA_TYPE);
    // g(I) parameter
    double alpha = 2;
    // standard variance of gassian blur
    double sigma = 3;
    // g(I)
    gborders(image_F, gI_F, alpha, sigma);

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    cout << "time of gborders: " << (int)t << " ms" << endl ;
#endif

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    double gI_threshold = 0.17;
    double gI_balloon_v = 1;

    Mat mask_F;
    mask.convertTo(mask_F, SNAKE_DATA_TYPE);

    cv::Mat gI_threshold_mask(gI_F.size(), CV_8UC1, cv::Scalar(0));
    get_gI_threshold_mask(gI_F, gI_threshold_mask, gI_threshold, gI_balloon_v);

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    cout << "time of get_gI_threshold_mask: " << (int)t << " ms" << endl ;
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
    cout << "time of calc_derivative: " << (int)t << " ms" << endl ;
#endif

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    int num_iters = 60;
    for(int i=0; i<num_iters; ++i)
    {

#ifdef SNAKE_DEMO_DEBUG
        mask_F.convertTo(mask_F, SNAKE_DATA_TYPE);
#endif

        // step.1 & step.2
        morph_gac(gI_threshold_mask, derivative_gI_X, derivative_gI_Y, mask_F, gI_balloon_v);


//        // step.3 ; ISoSI can use in iterations or use it out of iterations
//          ISoSI(mask_F);

//        // medianBlur can replace ISoSI approximatively
//        // smoothing curve
//        medianBlur(mask_F, mask_F, 3);

#ifdef SNAKE_DEMO_DEBUG
        mask_F.convertTo(mask_F, CV_8UC1);
        Canny(mask_F, mask, 30, 120);
        threshold(mask, mask, 255, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
        Mat res;
        image.copyTo(res, mask);
        imshow("res", res);
        waitKey(100);
#endif

    }

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    cout << "time of morph_gac iterations: " << (int)t << " ms" << endl ;
#endif

#ifdef SNAKE_DEBUG
    begin_time = getTickCount();
#endif

    // step.3 ;  smoothing curve
    ISoSI(mask_F);

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - begin_time)*1000 / getTickFrequency();
    cout << "time of ISoSI: " << (int)t << " ms" << endl ;
#endif

#ifdef SNAKE_DEBUG
    end_time = getTickCount();
    t = (end_time - total_begin_time)*1000 / getTickFrequency();
    cout << "total time: " << (int)t << " ms" << endl ;
#endif

#ifdef SNAKE_DEMO_DEBUG
    Canny(mask_F, mask, 30, 120);
    threshold(mask, mask, 255, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
    Mat res;
    image.copyTo(res, mask);
    imshow("res", res);
    waitKey(30000);
#endif

    return 0;
}

