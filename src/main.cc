#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "preprocess/grayscale.hh"
#include "preprocess/blackhat.hh"
#include "preprocess/threshold.hh"
#include "detection/longbarfilter.hh"
#include "detection/barcodeextractor.hh"
#include "decoding/decoder.hh"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: ./BarCodeReader file.png\n";
    return 1;
  }

  cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
  cv::Mat image_copy = image.clone();

  // Preprocessing
  cv::Mat matrix = preprocess::grayscale(image);
  cv::Mat grayscale = matrix.clone();
  cv::imwrite("1_output_gray.png", matrix);
  matrix = preprocess::blackhat(matrix);
  cv::imwrite("2_output_blackhat.png", matrix);
  matrix = preprocess::threshold(matrix);
  cv::imwrite("3_output_binary.png", matrix);

  // Find connected components bounding boxes and keep only the very long ones
  detection::LongBarFilter lbfilter{matrix};
  lbfilter.find_contours();
  lbfilter.find_bounding_boxes();
  lbfilter.filter_small_boxes();
  lbfilter.draw_bounding_boxes(image, cv::Scalar(0, 0, 255));
  cv::imwrite("4_output_filtered_boxes.png", image);

  // Extract barcode.
  detection::BarCodeExtractor bce{grayscale, lbfilter.boxes_get()};
  bce.process_segments();
  bce.draw_segments(image, cv::Scalar(255, 255, 0));
  cv::imwrite("5_output_filtered_boxes_with_segments.png", image);
  bce.find_neighbours();
  bce.process_group_boxes();
  bce.filter_group_boxes();
  bce.draw_group_boxes(image_copy, cv::Scalar(0, 0, 255), 3);
  cv::imwrite("6_output_detected_bar_codes.png", image_copy);
  size_t nbbarcodes = bce.group_boxes_get().size();
  std::cout << nbbarcodes << " barcode" << (nbbarcodes <= 1 ? " was" : "s were") << " found." << std::endl;
  bce.extract_barcodes();

  decoding::Decoder decoder{bce.barcodes_get()};
  decoder.decode();

  //cv::namedWindow("image");
  //cv::imshow("image", image);
  //cv::waitKey(0);

  return 0;
}
