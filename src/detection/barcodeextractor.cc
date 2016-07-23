#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "barcodeextractor.hh"
#include "../misc/distance.hh"


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
        if (elt1.first == elt2.first && elt1.second == elt2.second)
          return true;
    return false;
  }

} // Anonymous namespace

namespace detection
{
  BarCodeExtractor::BarCodeExtractor()
    : segments_{}
    , comparator_{[](const segm_t& p1, const segm_t& p2)
                  {
                    return p1.first.x + p1.first.y + p1.second.x + p1.second.y
                           < p2.first.x + p2.first.y + p2.second.x + p2.second.y;
                  }}
    , segment_groups_{}
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

  void BarCodeExtractor::draw_segments(cv::Mat& image, cv::Scalar color)
  {
    for (auto& segment : segments_)
      cv::line(image, segment.first, segment.second, color, 1, 8);
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
        if (segment == potential_neighb)
          continue;

        double distance_first = misc::distance(segment.first, potential_neighb.first);
        double distance_second = misc::distance(segment.second, potential_neighb.second);

        if (std::abs(distance_first - distance_second) < 30
            && distance_first < 50
            && distance_second < 50)
          group.insert({potential_neighb});
      }

      if (group.size() >= 2)
        segment_groups_.push_back(std::move(group));
    }

    // Find groups with common elements and merge them
    for (auto it1 = segment_groups_.begin(); it1 != segment_groups_.end(); ++it1)
    {
      for (auto it2 = segment_groups_.begin(); it2 != segment_groups_.end(); ++it2)
      {
        if (it1 == it2)
          continue;

        if (is_common_elt_in_sets(*it1, *it2))
        {
          it1->insert(it2->begin(), it2->end());
          segment_groups_.erase(it2);
        }
      }
    }


  }
} // namespace detection
