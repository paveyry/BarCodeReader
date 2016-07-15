#pragma once

#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

namespace detection
{
  class LongBarFilter
  {
  public:
    LongBarFilter(const cv::Mat& matrix);
    void find_contours();
    void find_bounding_boxes();
    void filter_small_boxes();
    void draw_bounding_boxes(cv::Mat& image, cv::Scalar color);
    const std::vector<std::vector<cv::Point>>& contours_get() const;
    std::vector<std::vector<cv::Point>>& contours_get();
    const std::vector<cv::RotatedRect>& boxes_get() const;
    std::vector<cv::RotatedRect>& boxes_get();

  private:
    const cv::Mat& matrix_;
    std::vector<std::vector<cv::Point>> contours_;
    std::vector<cv::RotatedRect> boxes_;
  };
}

#include "longbarfilter.hxx"