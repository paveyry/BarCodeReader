#pragma once

#include <opencv2/core/core.hpp>
#include <set>

namespace detection
{
  class BarCodeExtractor
  {
  public:
    using segm_t = std::pair<cv::Point2f, cv::Point2f>;
    using comp_t = std::function<bool(const segm_t&, const segm_t&)>;
    using set_t = std::set<segm_t, comp_t>;

  public:
    // Constructor.
    BarCodeExtractor();

    // Determine the middle segments of the bounding boxes
    void process_segments(const std::vector<cv::RotatedRect>& boxes);

    // Draw the middle segments on the image in parameters
    void draw_segments(cv::Mat& image, cv::Scalar color);

    // Find the groups of neighbour bounding boxes using the segments
    void find_neighbours();

  private:
    std::vector<segm_t> segments_;
    comp_t comparator_;
    std::vector<set_t> segment_groups_;
    std::vector<cv::RotatedRect> barcode
  };
}
