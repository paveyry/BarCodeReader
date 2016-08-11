#pragma once

#include <opencv2/core/core.hpp>

namespace preprocess
{
  cv::Mat binarize(const cv::Mat& matrix, int threshold);

  // Determine a threshold using Otsu's method and binarize the matrix
  cv::Mat threshold_otsu(const cv::Mat& matrix);
}

