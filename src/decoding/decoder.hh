#pragma once


#include <opencv2/core/core.hpp>
#include "../detection/barcodeextractor.hh"

namespace decoding
{
  class Decoder
  {
  public:
    Decoder(std::vector<cv::Mat>& barcodes);

  private:
    std::vector<cv::Mat>& barcodes_;

  };
}
