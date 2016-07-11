#include "grayscale.hh"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace preprocess
{
  cv::Mat grayscale(const cv::Mat& image)
  {
    cv::Mat newmat(image.rows, image.cols, cv::DataType<uchar>::type);

    for (int i = 0; i < image.rows; ++i)
    {
      for (int j = 0; j < image.cols; ++j)
      {
        cv::Vec3b color = image.at<cv::Vec3b>(i, j);
        newmat.at<uchar>(i, j) = static_cast<uchar>(static_cast<double>(color[0]) * 0.2126
                                                    + static_cast<double>(color[1]) * 0.7152
                                                    + static_cast<double>(color[2]) * 0.0722);
      }
    }
    return newmat;
  }
}
