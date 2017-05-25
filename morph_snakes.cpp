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
    cv::pow(grad_x, 2, grad_x);
    cv::pow(grad_y, 2, grad_y);
    cv::add(grad_x, grad_y, grad);
    cv::sqrt(grad, grad);

//    cv::Mat angle;
//    cv::cartToPolar(grad_x, grad_y, grad, angle);

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

void morph_gac(const cv::Mat &gI, const cv::Mat &gI_threshold_mask, cv::Mat &u, double gI_balloon_v)
{


}
