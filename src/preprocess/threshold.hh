#pragma once

#include <opencv2/core/core.hpp>

namespace preprocess
{
  // Determine a threshold using Otsu's method and binarize the matrix
  cv::Mat threshold(const cv::Mat& matrix);
}

