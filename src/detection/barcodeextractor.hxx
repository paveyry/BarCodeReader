#pragma once
#include "barcodeextractor.hh"

namespace detection
{
  inline
  std::vector<cv::RotatedRect>& BarCodeExtractor::group_boxes_get()
  {
    return group_boxes_;
  }

  inline
  const std::vector<cv::RotatedRect>& BarCodeExtractor::group_boxes_get() const
  {
    return group_boxes_;
  }
}
