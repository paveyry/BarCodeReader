#pragma once

#include <opencv2/core/core.hpp>

namespace preprocess
{
  // Apply black top-hat morphological operation to matrix
  cv::Mat blackhat(cv::Mat matrix);
}
