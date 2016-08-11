#include <algorithm>

#include "long-bar-filter.hh"

namespace detection
{
  LongBarFilter::LongBarFilter(const cv::Mat& matrix)
    : matrix_{matrix}
    , contours_{}
    , boxes_{}
  {}

  void LongBarFilter::find_contours()
  {
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(matrix_, contours_, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
  }

  void LongBarFilter::find_bounding_boxes()
  {
    for (size_t i = 0; i < contours_.size(); ++i)
    {
      boxes_.push_back(cv::minAreaRect(cv::Mat(contours_[i])));
    }
  }

  void LongBarFilter::filter_small_boxes()
  {
    std::function<bool(cv::RotatedRect&)> predicate = [this](cv::RotatedRect& rect)
    {
      return std::max(rect.size.height, rect.size.width) <= 7 * std::min(rect.size.height, rect.size.width)
             || std::max(rect.size.height, rect.size.width) < 20;
    };
    boxes_.erase(std::remove_if(boxes_.begin(), boxes_.end(), predicate), boxes_.end());
  }

  void LongBarFilter::draw_bounding_boxes(cv::Mat& image, cv::Scalar color)
  {
    for (auto& rect : boxes_)
    {
      cv::Point2f rect_points[4];
      rect.points(rect_points);

      for (int i = 0; i < 4; ++i)
        cv::line(image, rect_points[i], rect_points[(i + 1) % 4], color, 1, 8);
    }
  }

} // namespace detection