#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;
// extern "C" __attribute__((visibility("default"))) __attribute__((used))
// void Gaussian(char *path) {
//     Mat img = imread(path);
//     Mat temp;
//     GaussianBlur(img, temp, Size(75, 75), 0, 0);
//     imwrite(path, temp);
// }

extern "C" __attribute__((visibility("default"))) __attribute__((used))
int DetectLines(const uint8_t* imageBytes, int size, uint8_t* outputBytes) {

    std::vector<uint8_t> v(imageBytes, imageBytes + size);
    // cv::Mat image(height, width, CV_8UC(channels), (void*)imageBytes);
    cv::Mat image = cv::imdecode(cv::Mat(v), cv::IMREAD_COLOR);

    Mat dst, bw_dst;
    cvtColor(image, bw_dst, cv::COLOR_RGB2GRAY);

    // cv::Mat gaused;
    // cv::GaussianBlur(dst, gaused, Size(5, 5), 0);

    cv::Mat thresholded;
    cv::threshold(bw_dst, thresholded, 200, 255, cv::THRESH_BINARY);

    cv::Mat cannied;
    cv::Canny(thresholded, cannied, 50, 200, 3);

    std::vector<Vec4i> lines; // will hold the results of the detection
    HoughLinesP(cannied, lines, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
    // Draw the lines
    int count = 0;
    double angle_agg = 0;

    for( size_t i = 0; i < lines.size(); i++ )
    {
        int x1 = lines[i][0];
        int y1 = lines[i][1];
        int x2 = lines[i][2];
        int y2 = lines[i][3];

        double angle = std::abs(atan2(x2 - x1, y2 - y1));

        if (angle > 40 * CV_PI / 180) {
            continue;
        }

        angle_agg += angle;
        ++count;

        Vec4i l = lines[i];
        line(image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }

    std::vector<uint8_t> retv;
    cv::imencode(".jpg", image, retv);

    memcpy(outputBytes, retv.data(), retv.size());

    return (angle_agg * (180 / CV_PI)) / (double)count;//(right_angle + left_angle) / 2;
}


