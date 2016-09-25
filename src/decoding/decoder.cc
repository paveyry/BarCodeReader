#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "decoder.hh"

namespace decoding
{
  Decoder::Decoder(std::vector<cv::Mat>& barcodes)
    : barcodes_{barcodes}
  {}

  std::vector<std::string> Decoder::decode()
  {
    std::vector<std::string> codes;

    for (auto& barcode : barcodes_)
    {
      std::unordered_map<std::string, int> occurrence_hist;

      for (int r = 0; r < barcode.rows; ++r)
      {
        try
        {
          int unit_width = process_unit_width(barcode, r);
          std::vector<int> bars = read_bars(barcode, unit_width, r);

          std::vector<std::string> groups = group_bars(bars);

          std::stringstream stringstream;

          for (const auto& group : groups)
          {
            try
            {
              stringstream << std::to_string(values_.at(group));
            } catch (std::out_of_range e)
            {
              break;
            }
          }

          std::string code = stringstream.str();
          if (code.size() < 12)
            continue;

          if (occurrence_hist.count(code))
            ++occurrence_hist[code];
          else
            occurrence_hist[code] = 1;

          std::cerr << code << std::endl;
        }
        catch (std::runtime_error e)
        {
          continue;
        }
      }

      std::pair<std::string, int> best{"COULD NOT DECODE!", 0};
      for (auto it = occurrence_hist.cbegin(); it != occurrence_hist.cend(); ++it)
        if (it->second > best.second)
          best = *it;

      codes.push_back(best.first);

    }
    return codes;
  }

  int Decoder::process_unit_width(const cv::Mat& barcode, int row) const
  {
    float first_bar_width = 0.f;
    float second_bar_width = 0.f;
    float third_bar_width = 0.f;

    int i = 0;

    while (i < barcode.cols && barcode.at<uchar>(row, i) == 255)
      ++i;

    for (; i < barcode.cols && barcode.at<uchar>(row, i) == 0; ++i)
      ++first_bar_width;

    for (; i < barcode.cols && barcode.at<uchar>(row, i) == 255; ++i)
      ++second_bar_width;

    for (; i < barcode.cols && barcode.at<uchar>(row, i) == 0; ++i)
      ++third_bar_width;

    return static_cast<int>(std::round((first_bar_width + second_bar_width + third_bar_width) / 3.f));
  }

  std::vector<int> Decoder::read_bars(const cv::Mat& barcode, int unit_width, int row) const
  {
    bool black = true;
    float accumulator = 0.0;
    std::vector<int> result;

    int i = 0;
    while (i < barcode.cols && barcode.at<uchar>(row, i))
      ++i;

    for (; i < barcode.cols; ++i)
    {
      if ((black && barcode.at<uchar>(row, i) == 255)
          || (!black && barcode.at<uchar>(row, i) == 0))
      {
        black = !black;
        result.push_back(static_cast<int>(std::round(accumulator / static_cast<float>(unit_width))));
        accumulator = 0.0;
      }

      ++accumulator;
    }

    return result;
  }

  std::vector<std::string> Decoder::group_bars(const std::vector<int>& bars) const
  {
    std::vector<std::string> groups;

    if (bars.size() < 59)
      throw std::runtime_error(
        std::string("Decoder::group_bars: Too few bars could be read on the analyzed barcode row: ")
        + std::to_string(bars.size())
      );

    std::stringstream stringstream;

    // Lambda that adds a bar into a group string and pushes group strings in the vector when they are full
    auto process_bar = [&](int i, int modulo)
    {
      stringstream << std::to_string(bars[i]);

      if (i % 4 == modulo)
      {
        groups.push_back(stringstream.str());
        stringstream = std::stringstream();
      }
    };

    // First half of the barcode
    for (int i = 3; i < 27; ++i)
      process_bar(i, 2);

    // Second half of the barcode
    for (int i = 32; i < 56; ++i)
      process_bar(i, 3);

    return groups;
  }

}
