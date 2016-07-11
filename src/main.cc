#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "preprocess/grayscale.hh"

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

  cv::namedWindow("test");
  cv::imshow("test", matrix);
  cv::waitKey(0);


  return 0;
}
