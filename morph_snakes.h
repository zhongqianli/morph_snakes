#ifndef MORPH_SNAKES_H
#define MORPH_SNAKES_H

#include <opencv2/opencv.hpp>

void gborders(const cv::Mat &image, cv::Mat &gI, double alpha, double sigma);
void get_gI_threshold_mask(const cv::Mat &gI, cv::Mat &gI_threshold_mask, double gI_threshold, double gI_balloon_v);
void get_derivative_gI(const cv::Mat &gI, cv::Mat &derivative_gI_X, cv::Mat &derivative_gI_Y);
void morph_gac(const cv::Mat &gI_threshold_mask, const cv::Mat &derivative_gI_X, const cv::Mat &derivative_gI_Y, cv::Mat &u, double gI_balloon_v);

#endif // MORPH_SNAKES_H
