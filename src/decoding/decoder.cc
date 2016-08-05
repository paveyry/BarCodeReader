#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "decoder.hh"

namespace decoding
{
  Decoder::Decoder(std::vector<cv::Mat>& barcodes)
    : barcodes_{barcodes}
  {}



  std::vector<int> Decoder::decode()
  {
    int i = 0;
    for (auto& barcode : barcodes_)
    {
      int unit_width = process_unit_width(barcode);
      std::cout << std::to_string(unit_width) << std::endl;
      cv::imwrite("test" + std::to_string(i++) + ".jpg", barcode);

    }
  }

  int Decoder::process_unit_width(const cv::Mat& barcode) const
  {
     int row = barcode.rows / 3;

      float first_bar_width = 0.f;
      float second_bar_width = 0.f;
      float third_bar_width = 0.f;

      int i = 0;

      while (barcode.at<uchar>(row, i) == 255)
        ++i;

      for (; barcode.at<uchar>(row, i) == 0; ++i)
        ++first_bar_width;

      for (; barcode.at<uchar>(row, i) == 255; ++i)
        ++second_bar_width;

      for (; barcode.at<uchar>(row, i) == 0; ++i)
        ++third_bar_width;

      return static_cast<int>(round((first_bar_width + second_bar_width + third_bar_width) / 3.f));
  }


}
