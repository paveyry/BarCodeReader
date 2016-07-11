#pragma once

#include <opencv2/core/core.hpp>

namespace preprocess
{
  cv::Mat grayscale(const cv::Mat& image);
}