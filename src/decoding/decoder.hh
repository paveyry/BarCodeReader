#pragma once


#include <opencv2/core/core.hpp>
#include "../detection/barcode-extractor.hh"
#include <vector>
#include <unordered_map>

namespace decoding
{
  class Decoder
  {
  public:
    // Constructor. The vector contains matrices of binarized, cropped and correctly oriented barcodes
    Decoder(std::vector<cv::Mat>& barcodes);

    // Decode all the barcodes and return their codes
    std::vector<std::string> decode();

  private:
    // Read the three first bars to determine the unit bar width
    int process_unit_width(const cv::Mat& barcode, int row) const;

    // Read all the bars and return a vector containing the list of bar widths (white and black)
    std::vector<int> read_bars(const cv::Mat& barcode, int unit_width, int row) const;

    // Regroupe the bars in groups of four. Ignore useless middle bars.
    std::vector<std::string> group_bars(const std::vector<int>& bars) const;

  private:
    std::vector<cv::Mat>& barcodes_;
    const std::unordered_map<std::string, int> values_ {
      // A and C
      std::pair<std::string, int>("3211", 0),
      std::pair<std::string, int>("2221", 1),
      std::pair<std::string, int>("2122", 2),
      std::pair<std::string, int>("1411", 3),
      std::pair<std::string, int>("1132", 4),
      std::pair<std::string, int>("1231", 5),
      std::pair<std::string, int>("1114", 6),
      std::pair<std::string, int>("1312", 7),
      std::pair<std::string, int>("1213", 8),
      std::pair<std::string, int>("3112", 9),
      // B
      std::pair<std::string, int>("1123", 0),
      std::pair<std::string, int>("1222", 1),
      std::pair<std::string, int>("2212", 2),
      std::pair<std::string, int>("1141", 3),
      std::pair<std::string, int>("2311", 4),
      std::pair<std::string, int>("1321", 5),
      std::pair<std::string, int>("4111", 6),
      std::pair<std::string, int>("2131", 7),
      std::pair<std::string, int>("3121", 8),
      std::pair<std::string, int>("2113", 9)
    };

  };
}
