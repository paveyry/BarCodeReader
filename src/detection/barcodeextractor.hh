#pragma once

#include <opencv2/core/core.hpp>

namespace detection
{
  class BarCodeExtractor
  {
  public:
    BarCodeExtractor();
    void process_segments(const std::vector<cv::RotatedRect>& boxes);
    void draw_segments(cv::Mat& mat, cv::Scalar color);

  private:
    std::vector<std::pair<cv::Point2f, cv::Point2f>> segments_;
  };
}
