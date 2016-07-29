#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "barcodeextractor.hh"
#include "../misc/distance.hh"
#include "../misc/rotation.hh"
#include "../preprocess/threshold.hh"


namespace
{
  /**
   * Return true if one element is common in the two sets
   */
  static bool is_common_elt_in_sets(const detection::BarCodeExtractor::set_t& s1,
                                    const detection::BarCodeExtractor::set_t& s2)
  {
    for (auto& elt1 : s1)
      for (auto& elt2 : s2)
        if (std::get<0>(elt1) == std::get<0>(elt2) && std::get<1>(elt1) == std::get<1>(elt2))
          return true;
    return false;
  }

} // Anonymous namespace

namespace detection
{
  BarCodeExtractor::BarCodeExtractor(cv::Mat mat, const std::vector<cv::RotatedRect>& boxes)
    : matrix_{mat}
    , boxes_{boxes}
    , segments_{}
    , comparator_{[](const segm_t& p1, const segm_t& p2)
                  {
                    return std::get<0>(p1).x + std::get<0>(p1).y + std::get<1>(p1).x + std::get<1>(p1).y
                           < std::get<0>(p2).x + std::get<0>(p2).y + std::get<1>(p2).x + std::get<1>(p2).y;
                  }}
    , segment_groups_{}
    , group_boxes_{}
    , barcodes_{}
  {}

  void BarCodeExtractor::process_segments()
  {
    for (size_t i = 0; i < boxes_.size(); ++i)
    {
      cv::Point2f points[4];
      boxes_[i].points(points);

      // Calculate width and height using the points in the right order instead of width and height class metadata
      double width = misc::distance(points[0], points[3]);
      double height = misc::distance(points[0], points[1]);

      cv::RotatedRect rect_copy = boxes_[i];

      if (width < height)
        segments_.push_back(std::make_tuple<cv::Point2f, cv::Point2f, const cv::RotatedRect&>
                              ({(points[1].x + points[2].x) / 2,
                                (points[1].y + points[2].y) / 2},
                               {(points[0].x + points[3].x) / 2,
                                (points[0].y + points[3].y) / 2},
                               std::move(rect_copy)));
      else
        segments_.push_back(std::make_tuple<cv::Point2f, cv::Point2f, const cv::RotatedRect&>
                              ({(points[1].x + points[0].x) / 2,
                                (points[1].y + points[0].y) / 2},
                               {(points[2].x + points[3].x) / 2,
                                (points[2].y + points[3].y) / 2},
                               std::move(rect_copy)));
    }
  }

  void BarCodeExtractor::draw_segments(cv::Mat& image, cv::Scalar color)
  {
    for (auto& segment : segments_)
      cv::line(image, std::get<0>(segment), std::get<1>(segment), color, 1, 8);
  }

  void BarCodeExtractor::find_neighbours()
  {
    // Find neighbour groups
    for (auto& segment : segments_)
    {
      set_t group{comparator_};
      group.insert({segment});

      for (auto& potential_neighb : segments_)
      {
        if (std::get<0>(segment) == std::get<0>(potential_neighb)
            && std::get<1>(segment) == std::get<1>(potential_neighb))
          continue;

        double distance_first = misc::distance(std::get<0>(segment), std::get<0>(potential_neighb));
        double distance_second = misc::distance(std::get<1>(segment), std::get<1>(potential_neighb));

        double average_length = (misc::distance(std::get<0>(segment), std::get<1>(segment))
                                 + misc::distance(std::get<0>(potential_neighb), std::get<1>(potential_neighb))) / 2.0;

        if (std::abs(distance_first - distance_second) < average_length / 5.0
            && (distance_first < average_length / 5.0
                || distance_second < average_length / 5.0))
          group.insert({potential_neighb});
      }

      if (group.size() >= 2)
        segment_groups_.push_back(std::move(group));
    }

    // Find groups with common elements and merge them
    for (auto it1 = segment_groups_.begin(); it1 != segment_groups_.end(); ++it1)
    {
      for (auto it2 = segment_groups_.begin(); it2 != segment_groups_.end();)
      {
        if (it1 == it2)
        {
          ++it2;
          continue;
        }

        if (is_common_elt_in_sets(*it1, *it2))
        {
          it1->insert(it2->begin(), it2->end());
          segment_groups_.erase(it2);
          // Starting from begin again because erasing invalidates iterators. A better method would be nice
          // but for now I don't know since creating a new vector brings other problems.
          it1 = segment_groups_.begin();
          it2 = segment_groups_.begin();
        }
        else
          ++it2;
      }
    }
  }

  void BarCodeExtractor::process_group_boxes()
  {
    for (auto& group : segment_groups_)
    {
      if (group.size() < 6)
        continue;

      // Store all the points in a temp vector
      std::vector<cv::Point2f> points;
      for (auto& elt : group)
      {
        cv::Point2f p[4];
        std::get<2>(elt).points(p);
        for (int i = 0; i < 4; ++i)
          points.push_back(p[i]);
      }

      // Determine the minimum area rectangle of the points
      cv::RotatedRect rect = cv::minAreaRect(points);

      group_boxes_.push_back(std::move(rect));
    }
  }

  void BarCodeExtractor::filter_group_boxes()
  {
    for (auto it = group_boxes_.begin(); it != group_boxes_.end();)
    {
      // Remove elements with height lower than (width / 7)
      if (std::min(it->size.width, it->size.height) < std::max(it->size.width, it->size.height) / 7.0)
        it = group_boxes_.erase(it);
      else
        ++it;
    }
  }

  void BarCodeExtractor::draw_group_boxes(cv::Mat& image, cv::Scalar color, int boldness)
  {
    for (auto& rect : group_boxes_)
    {
      cv::Point2f rect_points[4];
      rect.points(rect_points);

      for (int i = 0; i < 4; ++i)
        cv::line(image, rect_points[i], rect_points[(i + 1) % 4], color, boldness, 8);
    }
  }

  void BarCodeExtractor::extract_barcodes()
  {
    for (auto& rect : group_boxes_)
    {
      // Calculate a rotation matrix to apply the transform
      cv::Mat rotation_mat = cv::getRotationMatrix2D(rect.center, rect.angle, 1.0);

      // Rotate the complete image using the rotation matrix
      cv::Mat rotated_matrix;
      cv::warpAffine(matrix_, rotated_matrix, rotation_mat, matrix_.size(), cv::INTER_LINEAR);

      // Extract the concerned rectangle in a new Matrix
      cv::Mat extracted;
      cv::getRectSubPix(rotated_matrix, rect.size, rect.center, extracted);

      // If our barcode is vertical, make it horizontal
      if (extracted.rows > extracted.cols)
        extracted = misc::rotation90(extracted);

      // Reapply threshold (because we extracted a part of the grayscaled image)
      extracted = preprocess::threshold(extracted);

      cv::imwrite("test1.png", extracted);
      // Store the barcode
      barcodes_.push_back(extracted);
    }
  }
} // namespace detection
