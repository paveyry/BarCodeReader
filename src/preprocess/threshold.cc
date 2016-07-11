#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "threshold.hh"

namespace preprocess
{
  cv::Mat threshold(cv::Mat matrix, int threshold)
  {
    cv::Mat newmatrix;
    cv::threshold(matrix, newmatrix, threshold, 255, cv::THRESH_OTSU);
    return newmatrix;
  }
}
