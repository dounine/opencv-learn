#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

int main() {
    string img_path = "/Users/lake/CLionProjects/opencv-test/iphone.png";
    cv::Mat image = cv::imread(img_path);
    cv::Mat find_image = cv::imread("/Users/lake/CLionProjects/opencv-test/find.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not read the image: " << img_path << std::endl;
        return 1;
    }
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_RGB2GRAY);

    //构建结果矩阵
    cv::Mat result(gray_image.rows - find_image.rows + 1, gray_image.cols - find_image.cols + 1, CV_32FC1);
    //模板匹配
    cv::matchTemplate(gray_image, find_image, result, cv::TM_CCOEFF);

    double dMaxVal;//分数最大值
    cv::Point ptMaxLoc;//最大值坐标
    //寻找结果矩阵中的最大值
    cv::minMaxLoc(result, nullptr, &dMaxVal, nullptr, &ptMaxLoc);

    std::cout << "dMaxVal: " << dMaxVal << std::endl;

    //绘制矩形
    cv::Point pt1(ptMaxLoc.x, ptMaxLoc.y);
    cv::Point pt2(ptMaxLoc.x + find_image.cols, ptMaxLoc.y);
    cv::Point pt3(ptMaxLoc.x, ptMaxLoc.y + find_image.rows);
    cv::Point pt4(ptMaxLoc.x + find_image.cols, ptMaxLoc.y + find_image.rows);

    //画线
    cv::line(image, pt1, pt2, cv::Scalar(0, 255, 0), 11, 11, 1);
    cv::line(image, pt2, pt4, cv::Scalar(0, 255, 0), 11, 11, 1);
    cv::line(image, pt4, pt3, cv::Scalar(0, 255, 0), 11, 11, 1);
    cv::line(image, pt3, pt1, cv::Scalar(0, 255, 0), 11, 11, 1);

    cv::imshow("hello", image);
    cv::waitKey(0);
//    cv::imshow("hello", dest);
//    cv::waitKey(0);

//    cv::VideoCapture cap(0);
//    if (!cap.isOpened()) {
//        std::cout << "Could not open the camera" << std::endl;
//        return 1;
//    }
//    cv::Mat frame;
//    while(true){
//        cap >> frame;
//        if (frame.empty()) {
//            std::cout << "Could not read the frame" << std::endl;
//            break;
//        }
//        cv::imshow("hello", frame);
//        if (cv::waitKey(10) == 27) {
//            std::cout << "Esc key is pressed by user. Stopping the video" << std::endl;
//            break;
//        }
//    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
