#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "preprocess/grayscale.hh"
#include "preprocess/blackhat.hh"
#include "preprocess/threshold.hh"
#include "detection/longbarfilter.hh"

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
  matrix = preprocess::threshold(matrix);

  // Find connected components bounding boxes and keep only the very long ones
  detection::LongBarFilter lbfilter{matrix};
  lbfilter.find_contours();
  lbfilter.find_bounding_boxes();
  lbfilter.filter_small_boxes();
  lbfilter.draw_bounding_boxes(image, cv::Scalar(0, 0, 255));


  cv::imwrite("output.png", image);
  cv::namedWindow("image");
  cv::imshow("image", image);
  cv::waitKey(0);

  return 0;
}
