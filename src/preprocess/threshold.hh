#pragma once

#include <opencv2/core/core.hpp>

namespace preprocess
{
  cv::Mat threshold(cv::Mat matrix, int threshold);
}

