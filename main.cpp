#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int imgMatchTemplate();
int imgMatchTemplatePlus();
int main() {
//    imgMatchTemplate();
    imgMatchTemplatePlus();
    return 0;
}

vector<Mat> buildTemplates(Mat templ) {
    vector<Mat> templates = vector<Mat>();
    templates.push_back(templ);
    float resizeRetain = 0.8;
    for (int i = 0; i < 4; i++) {
        Mat resizeTempl;
        //放大
        float resizeLargeRetain = pow(2 - resizeRetain, i + 1);
        resize(templ, resizeTempl, Size(0, 0), resizeLargeRetain, resizeLargeRetain);
        templates.push_back(resizeTempl);

        //缩小
        float resizeSamllReduce = pow(resizeRetain, i + 1);
        resize(templ, resizeTempl, Size(0, 0), resizeSamllReduce, resizeSamllReduce);
        templates.push_back(resizeTempl);
    }
    return templates;
}

int imgMatchTemplatePlus() {
    string img_path = "/Users/lake/dounine/github/apple/opencv-learn/iphone.png";
    cv::Mat image = cv::imread(img_path);
    cv::Mat templ = cv::imread("/Users/lake/dounine/github/apple/opencv-learn/find.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not read the image: " << img_path << std::endl;
        return 1;
    }
    if (templ.empty()) {
        std::cout << "Could not read the image: " << img_path << std::endl;
        return 1;
    }

    long start = clock();
    // 使用SIFT检测关键点各提取特征
    Ptr<SIFT> sift = SIFT::create();
    vector<KeyPoint> keypointsMain, keypointsTemplate;
    Mat descriptorsMain, descriptorsTemplate;
    sift->detectAndCompute(image, Mat(), keypointsMain, descriptorsMain);
    sift->detectAndCompute(templ, Mat(), keypointsTemplate, descriptorsTemplate);

    //使用FLANN进行特征匹配
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptorsTemplate, descriptorsMain, matches);

    //绘制匹配结果
    double minDist = 100.0;
    vector<DMatch> goodMatches;
    for (int i = 0; i < descriptorsTemplate.rows; i++) {
        if (matches[i].distance < minDist) {
            goodMatches.push_back(matches[i]);
        }
    }

    long end = clock();

    std::cout << "耗时: " << (end - start) / 1000 << "ms" << std::endl;

    Mat imgMatches;
    cv::drawMatches(templ, keypointsTemplate, image, keypointsMain, goodMatches, imgMatches);

    // 绘制矩形框标记匹配位置
//    for (size_t i = 0; i < goodMatches.size(); i++) {
//        Point2f point = keypointsMain[goodMatches[i].trainIdx].pt;
//        rectangle(image, point, cv::Point(point.x + templ.cols, point.y + templ.rows), cv::Scalar(0, 255, 0), 2);
//    }

    // 显示结果
    cv::imshow("Matches", imgMatches);
    cv::waitKey(0);
    return 0;
}

int imgMatchTemplate() {
    string img_path = "/Users/lake/dounine/github/apple/opencv-learn/iphone.png";
    cv::Mat image = cv::imread(img_path);
    cv::Mat find_image = cv::imread("/Users/lake/dounine/github/apple/opencv-learn/find.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Could not read the image: " << img_path << std::endl;
        return 1;
    }
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_RGB2GRAY);


    //计算匹配耗时
    long start = clock();
    vector<Mat> templates = buildTemplates(find_image);
    bool isFind = false;
    for (int i = 0; i < templates.size(); i++) {
        Mat tmpl = templates[i];

        int result_rows = gray_image.cols - tmpl.cols + 1;
        int result_cols = gray_image.rows - tmpl.rows + 1;

        //构建结果矩阵
        Mat result(result_rows, result_cols, CV_32FC1);
        //模板匹配
        matchTemplate(gray_image, tmpl, result, TM_CCORR_NORMED);

        double minValue, maxValue;//分数最大值
        Point minLocation, maxLocation;//最大值坐标
        //寻找结果矩阵中的最大值
        minMaxLoc(result, &minValue, &maxValue, &minLocation, &maxLocation, Mat());

        std::cout << "dMaxVal: " << maxValue << std::endl;

        if (maxValue >= 0.9) {
            isFind = true;
            rectangle(image, Rect(
                    maxLocation.x,
                    maxLocation.y,
                    tmpl.cols,
                    tmpl.rows
            ), Scalar(0, 255, 0), 10);
            imshow("hello", image);
            // x:963 y:1815 width:245 height:233 //正确的
            cout << "找到了 x:" << maxLocation.x << " y:" << maxLocation.y << " width:" << tmpl.cols << " height:"
                 << tmpl.rows << endl;
            break;
        }
    }
    long end = clock();
    std::cout << "耗时: " << (end - start) / 1000 << "ms" << std::endl;
    if (!isFind) {
        std::cout << "未找到" << std::endl;
    }
//    imshow("hello", image);
    waitKey(0);
//    cv::imshow("hello", dest);
//    waitKey(0);

//    VideoCapture cap(0);
//    if (!cap.isOpened()) {
//        std::cout << "Could not open the camera" << std::endl;
//        return 1;
//    }
//    Mat frame;
//    while(true){
//        cap >> frame;
//        if (frame.empty()) {
//            std::cout << "Could not read the frame" << std::endl;
//            break;
//        }
//        imshow("hello", frame);
//        if (waitKey(10) == 27) {
//            std::cout << "Esc key is pressed by user. Stopping the video" << std::endl;
//            break;
//        }
//    }
    std::cout << "Hello, World!" << std::endl;
}

//void matchMethod(int,void*){
//    Mat img_display;
//    img.copyTo(img_display);
//    int result_cols = img.cols - templ.cols + 1;
//    int result_rows = img.rows - templ.rows + 1;
//    //构建结果矩阵
//    Mat result(img.rows - templ.rows + 1, img.cols - templ.cols + 1, CV_32FC1);
//    result.create(result_rows, result_cols, CV_32FC1);
//    matchTemplate(img, templ, result, match_method);
//    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());//将结果归一化防止溢出，显示
//    double minVal; double maxVal; Point minLoc; Point maxLoc;
//    Point matchLoc;
//    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());//不同方法对应最小值与最大值不同
//    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
//    {
//        matchLoc = minLoc;
//    }
//    else
//    {
//        matchLoc = maxLoc;
//    }
//    rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);//绘制矩形
//    // rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//    imshow(image_window, img_display);
//    imshow(result_window, result);
//    imshow("template", templ);
//    return;
//}
