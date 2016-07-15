#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "blackhat.hh"

namespace preprocess
{
  cv::Mat blackhat(cv::Mat matrix)
  {
    cv::Mat element = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(25, 25));

    cv::Mat dst;

    cv::morphologyEx(matrix, dst, cv::MORPH_BLACKHAT, element);

    return dst;
  }
}
