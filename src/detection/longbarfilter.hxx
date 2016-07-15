#pragma once

#include "longbarfilter.hh"

namespace detection
{
  inline
  const std::vector<std::vector<cv::Point>>&
  LongBarFilter::contours_get() const
  {
    return contours_;
  }

  inline
  std::vector<std::vector<cv::Point>>&
  LongBarFilter::contours_get()
  {
    return contours_;
  }

  inline
  const std::vector<cv::RotatedRect>&
  LongBarFilter::boxes_get() const
  {
    return boxes_;
  }

  inline
  std::vector<cv::RotatedRect>&
  LongBarFilter::boxes_get()
  {
    return boxes_;
  }
}
