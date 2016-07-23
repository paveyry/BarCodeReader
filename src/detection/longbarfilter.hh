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
    // Constructor. The matrix is the input matrix for treatments.
    LongBarFilter(const cv::Mat& matrix);

    // Find contours in matrix
    void find_contours();

    // Determine rotated bounding boxes of contours
    void find_bounding_boxes();

    // Remove bounding boxes with non relevant proportions
    void filter_small_boxes();

    // Draw the stored bounding boxes on the image in parameters
    void draw_bounding_boxes(cv::Mat& image, cv::Scalar color);

    // Getters
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