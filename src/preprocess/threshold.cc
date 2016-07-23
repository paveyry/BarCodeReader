#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "threshold.hh"

namespace preprocess
{
  cv::Mat threshold(const cv::Mat& matrix)
  {
    cv::Mat newmatrix(matrix.rows, matrix.cols, cv::DataType<uchar>::type);

    // Grey level histogram
    std::vector<int> histogram(256);
    std::fill(histogram.begin(), histogram.end(), 0);

    // Complete histogram
    for (int i = 0; i < matrix.rows; ++i)
      for (int j = 0; j < matrix.cols; ++j)
        histogram[matrix.at<uchar>(i, j)] += 1;

    int nb_pixels = matrix.cols * matrix.rows;

    float sum = 0.f;
    for (int i = 0; i < 256; ++i)
      sum += i * histogram[i];

    float sum_b = 0;
    int w_b = 0;
    int w_f = 0;

    float max = 0;
    int threshold = 0;

    // Calclulate threshold
    for (int i = 0; i < 256; ++i)
    {
      w_b += histogram[i];
      if (w_b == 0)
        continue;

      w_f = nb_pixels - w_b;
      if (w_f == 0)
        break;

      sum_b += static_cast<float>(i * histogram[i]);

      float m_b = sum_b / w_b;
      float m_f = (sum - sum_b) / w_f;

      float dist = static_cast<float>(w_b)
                   * static_cast<float>(w_f)
                   * (m_b - m_f)
                   * (m_b - m_f);

      if (dist > max)
      {
        max = dist;
        threshold = i;
      }
    }

    // Binarize matrix
    for (int i = 0; i < matrix.rows; ++i)
    {
      for (int j = 0; j < matrix.cols; ++j)
      {
        if (matrix.at<uchar>(i, j) < threshold)
          newmatrix.at<uchar>(i, j) = 0;
        else
          newmatrix.at<uchar>(i, j) = 255;
      }
    }

    return newmatrix;
  }
} // namespace preprocess
