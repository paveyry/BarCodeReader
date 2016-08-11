#pragma once

#include <opencv2/core/core.hpp>
#include <set>

namespace detection
{
  class BarCodeExtractor
  {
  public:
    using segm_t = std::tuple<cv::Point2f, cv::Point2f, cv::RotatedRect>;
    using comp_t = std::function<bool(const segm_t&, const segm_t&)>;
    using set_t = std::set<segm_t, comp_t>;

  public:
    // Constructor.
    BarCodeExtractor(cv::Mat mat, const std::vector<cv::RotatedRect>& boxes);

    // Determine the middle segments of the bounding boxes
    void process_segments();

    // Draw the middle segments on the image in parameters
    void draw_segments(cv::Mat& image, cv::Scalar color);

    // Find the groups of neighbour bounding boxes using the segments
    void find_neighbours();

    // Determine the bounding box of each neighbour group
    void process_group_boxes();

    // Remove group boxes with wrong proportions.
    void filter_group_boxes();

    // Draw the group boxes on the image in parameters
    void draw_group_boxes(cv::Mat& image, cv::Scalar color, int boldness);

    void extract_barcodes();

    // Getters
    std::vector<cv::RotatedRect>& group_boxes_get();
    const std::vector<cv::RotatedRect>& group_boxes_get() const;
    std::vector<cv::Mat>& barcodes_get();
    const std::vector<cv::Mat>& barcodes_get() const;

  private:
    cv::Mat matrix_;
    const std::vector<cv::RotatedRect>& boxes_;
    std::vector<segm_t> segments_;
    comp_t comparator_;
    std::vector<set_t> segment_groups_;
    std::vector<cv::RotatedRect> group_boxes_;
    std::vector<cv::Mat> barcodes_;
  };
}

#include "barcode-extractor.hxx"
