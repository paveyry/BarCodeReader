#pragma once


#include <opencv2/core/core.hpp>
#include "../detection/barcodeextractor.hh"
#include <vector>

namespace decoding
{
  class Decoder
  {
  public:
    Decoder(std::vector<cv::Mat>& barcodes);
    std::vector<int> decode();

  private:
    int process_unit_width(const cv::Mat& barcode) const;

  private:
    std::vector<cv::Mat>& barcodes_;

  };
}
