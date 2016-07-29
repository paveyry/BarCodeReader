#pragma once

#include <opencv2/core/core.hpp>
#include <sys/dtrace.h>

namespace misc
{
  cv::Mat rotation90(cv::Mat input)
  {
    cv::Mat newmat(input.cols, input.rows, cv::DataType<uchar>::type);

    for (int i = 0; i < input.rows; ++i)
      for (int j = 0; j < input.cols; ++j)
        newmat.at<uchar>(input.cols - 1 - j, i) = input.at<uchar>(i, j);

    return newmat;
  }
}
