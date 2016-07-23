#pragma once

#include <opencv2/core/core.hpp>

namespace preprocess
{
  // Apply grayscale to image and create matrix of uchar
  cv::Mat grayscale(const cv::Mat& image);
}