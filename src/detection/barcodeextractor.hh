#pragma once

#include <opencv2/core/core.hpp>

namespace detection
{
  class BarCodeExtractor
  {
  public:
    using segm_t = std::pair<cv::Point2f, cv::Point2f>;

  public:
    BarCodeExtractor();
    void process_segments(const std::vector<cv::RotatedRect>& boxes);
    void draw_segments(cv::Mat& mat, cv::Scalar color);
    void find_neighbours();

  private:
    std::vector<segm_t> segments_;
    std::vector<std::vector<segm_t>> segment_groups_;
  };
}
