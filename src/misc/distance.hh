#pragma once

#include <opencv2/core/core.hpp>

namespace misc
{
  // Calculate the distance between two points
  inline double distance(cv::Point2f p1, cv::Point2f p2)
  {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
  }
}
