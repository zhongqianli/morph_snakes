#include "morph_snakes.h"

#include <math.h>
#include <iostream>

using namespace std;

/**
 * @brief gborders: g(I)
 * @param image: CV_64FC1, 1 channel
 * @param gI: CV_64FC1, 1 channel, g(I)
 * @param alpha: g(I) parameter
 * @param sigma: gaussian filter standard variance
 */
void gborders(const cv::Mat &image, cv::Mat &gI, double alpha, double sigma)
{
    //    int width = image.cols;
    //    int height = image.rows;

    //    cout << image.size() << ", " << image.type() << ", " << image.channels() << endl;

    cv::Mat gaus_blur_image(image.size(), image.type(), cv::Scalar(0));
    cv::GaussianBlur(image, gaus_blur_image, cv::Size(0,0), sigma, sigma);

    //    cout<< "gaus_blur_image:" << gaus_blur_image.size() << ", " << gaus_blur_image.type() << ", " << gaus_blur_image.channels() << endl;

    // gradient
    int scale = 1;
    int delta = 0;
    int ddepth = image.type();

    cv::Mat grad(image.size(), image.type(), cv::Scalar(0));
    cv::Mat grad_x(image.size(), image.type(), cv::Scalar(0)), grad_y(image.size(), image.type(), cv::Scalar(0));
    //    cv::Mat abs_grad_x(image.size(), image.type()), abs_grad_y(image.size(), image.type());

    // Scharr function , 3x3, as fast but more accurate than the standar Sobel function
    //    cv::Scharr(gaus_blur_image, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(gaus_blur_image, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
    //    cv::convertScaleAbs(grad_x, abs_grad_x);

    //    cout<< "grad_x:" << grad_x.size() << ", " << grad_x.type() << ", " << grad_x.channels() << endl;
    //    cout<< "abs_grad_x:" << abs_grad_x.size() << ", " << abs_grad_x.type() << ", " << abs_grad_x.channels() << endl;

    //    cv::Scharr(gaus_blur_image, grad_y, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(gaus_blur_image, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
    //    cv::convertScaleAbs(grad_y, abs_grad_y);

    // gradient
//    cv::pow(grad_x, 2, grad_x);
//    cv::pow(grad_y, 2, grad_y);
//    cv::add(grad_x, grad_y, grad);
//    cv::sqrt(grad, grad);

    cv::Mat angle;
    cv::cartToPolar(grad_x, grad_y, grad, angle);

    // g(I)
    gI = alpha*grad + 1e-20;
    cv::sqrt(gI, gI);
    cv::pow(gI, -1, gI);

    //    cout<< "grad_x:" << grad_x.size() << ", " << grad_x.type() << ", " << grad_x.channels() << endl;
    //    cout<< "grad:" << grad.size() << ", " << grad.type() << ", " << grad.channels() << endl;

    //    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

    //    for(int i=0; i<width; ++i)
    //    {
    //        double *grad_i = grad.ptr<double>(i);
    //        double *gI_i = gI.ptr<double>(i);
    //        for(int j=0; j<height; ++j)
    //        {
    //            gI_i[j] = 1.0 /  std::sqrt(1e-20 + alpha*grad_i[j]);
    //        }
    //    }

    //    cout << grad.size() << ", " << grad.type() << ", " << grad.channels() << endl;
    //    cout << tmp.size() << ", " << tmp.type() << ", " << tmp.channels() << endl;

    //    gI = grad.clone();

    //    for(int i=0; i<height; ++i)
    //    {
    //        for(int j=0; j<width; ++j)
    //        {
    //            gI.at<uchar>(i,j) = 1.0 /  sqrt(1.0 + alpha * grad.at<uchar>(i,j));
    //        }
    //    }

}

/**
 * @brief morph_gac
 * @param gI_threshold_mask
 * @param derivative_gI_X
 * @param derivative_gI_Y
 * @param u: mask
 * @param gI_balloon_v: >0 ? dilate : erode
 */
void morph_gac(const cv::Mat &gI_threshold_mask, const cv::Mat &derivative_gI_X, const cv::Mat &derivative_gI_Y, cv::Mat &u, double gI_balloon_v)
{
    // step.1
    cv::Mat aux;
    cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));

    if(gI_balloon_v > 0)
    {
        cv::dilate(u, aux, se);
        aux.copyTo(u, gI_threshold_mask);
//        u.convertTo(u, CV_64FC1);
    }
    else if(gI_balloon_v < 0)
    {
        cv::erode(u, aux, se);
        aux.copyTo(u, gI_threshold_mask);
//        u.convertTo(u, CV_64FC1);
    }

    // step.2
    cv::Mat derivative_u_X, derivative_u_Y;
    calc_derivative(u, derivative_u_X, derivative_u_Y);
    cv::Mat res = derivative_gI_X.mul(derivative_u_X) + derivative_gI_Y.mul(derivative_u_Y);
    res.convertTo(res, CV_8UC1);

//    cv::imshow("res test", res);

//    cv::waitKey(1);

    for(int i=0; i<res.rows; ++i)
    {
        for(int j=0; j<res.cols; ++j)
        {
            if(res.at<uchar>(i,j) > 0)
            {
                u.at<double>(i,j) = 255;
            }
            else if(res.at<uchar>(i,j) < 0)
            {
                u.at<double>(i,j) = 0;
            }
        }
    }
}

/**
 * @brief get_gI_threshold_mask
 * @param gI
 * @param gI_threshold_mask
 * @param gI_threshold
 * @param gI_balloon_v
 */
void get_gI_threshold_mask(const cv::Mat &gI, cv::Mat &gI_threshold_mask, double gI_threshold, double gI_balloon_v)
{
    // max = 1; min = 0.06
    for(int i=0; i<gI.rows; ++i)
    {
        for(int j=0; j<gI.cols; ++j)
        {
            if(gI.at<double>(i,j) > gI_threshold/abs(gI_balloon_v))
                gI_threshold_mask.at<uchar>(i,j) = 255;
            else
                gI_threshold_mask.at<uchar>(i,j) = 0;
        }
    }
}

/**
 * @brief get_derivative_gI
 * @param gI
 * @param derivative_gI_X
 * @param derivative_gI_Y
 */
void calc_derivative(const cv::Mat &image, cv::Mat &derivative_X, cv::Mat &derivative_Y)
{
    int scale = 1;
    int delta = 0;
    int ddepth = image.type();

    // Scharr function , 3x3, as fast but more accurate than the standar Sobel function
//    cv::Scharr(image, derivative_X, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(image, derivative_X, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);

//    cv::Scharr(image, derivative_Y, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT);
    cv::Sobel(image, derivative_Y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);

}

/**
 * @brief IS
 * @param u: mask
 */

void IS(cv::Mat &u)
{
    std::vector<cv::Mat> P(4);

    int p0[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    P[0] = cv::Mat(3, 3, CV_8UC1, p0);

    int p1[3][3] = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };
    P[1] = cv::Mat(3, 3, CV_8UC1, p1);

    int p2[3][3] = {
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 0}
    };
    P[2] = cv::Mat(3, 3, CV_8UC1, p2);

    int p3[3][3] = {
        {0, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    };
    P[3] = cv::Mat(3, 3, CV_8UC1, p3);

    cv::vector<cv::Mat> aux(4);
    cv::Mat res = cv::Mat::ones(u.size(), u.type());
    for(int i=0; i<4; ++i)
    {
        cv::dilate(u, aux[i], P[i]);
        res = res.mul(aux[i]);
    }
    u = res.clone();
}

/**
 * @brief SI
 * @param u: mask
 */
void SI(cv::Mat &u)
{
    std::vector<cv::Mat> P(4);

    int p0[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    P[0] = cv::Mat(3, 3, CV_8UC1, p0);

    int p1[3][3] = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };
    P[1] = cv::Mat(3, 3, CV_8UC1, p1);

    int p2[3][3] = {
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 0}
    };
    P[2] = cv::Mat(3, 3, CV_8UC1, p2);

    int p3[3][3] = {
        {0, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    };
    P[3] = cv::Mat(3, 3, CV_8UC1, p3);

    cv::vector<cv::Mat> aux(4);
    cv::Mat res = cv::Mat::zeros(u.size(), u.type());
    for(int i=0; i<4; ++i)
    {
        cv::erode(u, aux[i], P[i]);
        res = res + aux[i];
    }
    u = res.clone();
}

/**
 * @brief SIoIS: smoothing force; step.3
 * @param u: mask
 */
void SIoIS(cv::Mat &u)
{
    IS(u);
    SI(u);
}

/**
 * @brief ISoSI:  smoothing force; step.3
 * @param u: mask
 */
void ISoSI(cv::Mat &u)
{
    SI(u);
    IS(u);
}
