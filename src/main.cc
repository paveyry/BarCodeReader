#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "preprocess/grayscale.hh"
#include "preprocess/blackhat.hh"
#include "preprocess/threshold.hh"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: ./BarCodeReader file.png\n";
    return 1;
  }

  cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

  // Preprocessing
  cv::Mat matrix = preprocess::grayscale(image);
  matrix = preprocess::blackhat(matrix);
  matrix = preprocess::threshold(matrix, 127);


  cv::imwrite("output.png", matrix);
  cv::namedWindow("image");
  cv::imshow("image", matrix);
  cv::waitKey(0);

  return 0;
}
