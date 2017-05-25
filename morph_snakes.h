#ifndef MORPH_SNAKES_H
#define MORPH_SNAKES_H

#include <opencv2/opencv.hpp>

void gborders(const cv::Mat &image, cv::Mat &gI, double alpha, double sigma);

void morph_gac(const cv::Mat &gI, const cv::Mat &gI_threshold_mask, cv::Mat &u, double gI_balloon_v);

#endif // MORPH_SNAKES_H
