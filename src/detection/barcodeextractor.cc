#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "barcodeextractor.hh"
#include "../misc/distance.hh"

namespace detection
{
  BarCodeExtractor::BarCodeExtractor()
    : segments_{}
  {}

  void BarCodeExtractor::process_segments(const std::vector<cv::RotatedRect>& boxes)
  {
    for (auto& box : boxes)
    {
      cv::Point2f points[4];
      box.points(points);

      // Calculate width and height using the points in the right order instead of width and height class metadata
      float width = static_cast<float>(misc::distance(points[0], points[3]));
      float height = static_cast<float>(misc::distance(points[0], points[1]));

      if (width < height)
        segments_.push_back(std::make_pair<cv::Point2f, cv::Point2f>({(points[1].x + points[2].x) / 2,
                                                                      (points[1].y + points[2].y) / 2},
                                                                     {(points[0].x + points[3].x) / 2,
                                                                      (points[0].y + points[3].y) / 2}));
      else
        segments_.push_back(std::make_pair<cv::Point2f, cv::Point2f>({(points[1].x + points[0].x) / 2,
                                                                      (points[1].y + points[0].y) / 2},
                                                                     {(points[2].x + points[3].x) / 2,
                                                                      (points[2].y + points[3].y) / 2}));
    }
  }

  void BarCodeExtractor::draw_segments(cv::Mat& mat, cv::Scalar color)
  {
    for (auto& segment : segments_)
      cv::line(mat, segment.first, segment.second, color, 1, 8);
  }


}
