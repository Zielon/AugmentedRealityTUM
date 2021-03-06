#include <opencv2/core/mat.hpp>
#include <opencv/cv.hpp>
#include <iostream>
#include "../headers/edgeDetector.h"
#include "../headers/transformations.h"
#include "../headers/poseEstimation.h"

using namespace cv;
using namespace std;

EdgeDetector *EdgeDetector::drawCircles() {
    int contourID = 0;
    for (auto contour : *_contours) {
        int p = 1;
        vector<Line> lines;
        for (; p < contour.size(); p++)
            lines.push_back(draw7points(contour[p], contour[p - 1], contourID));

        lines.push_back(draw7points(contour[p - 1], contour[0], contourID));

        vector<Point2f> points = Transformations::getIntersection(lines, _grey);

        if (points.size() == 4) {
            for (const auto &j : points) {
                circle(_original, j, 1, _BLUE, 2, 8, 0);
            }
            drawMarker(points);
        }
        contourID++;
    }

    return this;
}

EdgeDetector::EdgeDetector() {
    _contours = new vector<vector<Point>>();
    _hierarchy = new vector<Vec4i>();
}

void EdgeDetector::transformToBinary() {
    cvtColor(_original, _grey, CV_BGR2GRAY);
    threshold(_grey, _binary, _threshold, 255, THRESH_BINARY);
    //	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 12);
    //	adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 12);
    //  adaptiveThreshold(grayFrame, binaryFrame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 12);
}

EdgeDetector *EdgeDetector::drawEdges() {
    for (const auto &contour : *_contours)
        polylines(_original, contour, true, _RED, 2, 8, 0);

    return this;
}

void EdgeDetector::filterEdges() {
    transformToBinary();
    findContours(_binary, *_contours, *_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    auto *filtered = new vector<vector<Point>>();
    for (const auto &contour : *_contours) {
        vector<Point> approx;
        approxPolyDP(contour, approx, 3, true);
        if (approx.size() == 4) {
            Rect rect = boundingRect(Mat(approx));
            if (rect.size().area() > 300)
                filtered->push_back(approx);
        }
    }

    _contours = filtered;
}

Line EdgeDetector::draw7points(Point2f a, Point2f b, int &contourID) {
    Point2f c = b - a;
    Point2f point;
    vector<Point2f> linePoints;
    for (int i = 1; i < 8; i++) {
        double indicator = (double) i / 7.0;
        point = a + indicator * c;

        // #### EXERCISE 3 ####
        auto points = Transformations::getSubimage(point, _grey, _original, linePoints);
        auto result = Transformations::convertToMat(points);

        if (contourID == 5)
            imshow("Strip", result);
        contourID++;
    }

    return fitLine(linePoints);
}

EdgeDetector *EdgeDetector::setFrame(Mat original) {
    _original = original;
    _binary = original;
    filterEdges();
    return this;
}

Mat EdgeDetector::getTransformed() {
    return _original;
}

EdgeDetector *EdgeDetector::setThreshold(int threshold) {
    _threshold = threshold;
    return this;
}

Line EdgeDetector::fitLine(vector<Point2f> points) {
    Vec4f line;
    cv::fitLine(points, line, CV_DIST_L2, 0, 0.01, 0.01);
    auto d = Point2f(line[0], line[1]) * 200;
    auto s = Point2f(line[2], line[3]);
    //cv::line(_original, s - d, s + d, _BLUE, 1, 8, 0);
    return Line(s - d, s + d);
}

EdgeDetector *EdgeDetector::drawMarker(vector<Point2f> inter) {

    if (any_of(inter.begin(), inter.end(), [this](Point2f i) { return i.x < 0 || i.y < 0; }))
        return this;

    vector<Point2f> target;
    vector<Point2f> intersections;

    // #### KEEP THE SAME ORDER ####

    sort(inter.begin(), inter.end(), [](const Point2f &p1, const Point2f &p2) { return p1.x > p2.x; });

    auto right_up = inter[0].y < inter[1].y ? inter[0] : inter[1];
    auto right_down = inter[0].y < inter[1].y ? inter[1] : inter[0];

    sort(inter.begin(), inter.end(), [](const Point2f &p1, const Point2f &p2) { return p1.x < p2.x; });

    auto left_up = inter[0].y < inter[1].y ? inter[0] : inter[1];
    auto left_down = inter[0].y < inter[1].y ? inter[1] : inter[0];

    target.emplace_back(Point2f(-0.5f, -0.5f)); // left_up
    target.emplace_back(Point2f(5.5, -0.5f));   // right_up
    target.emplace_back(5.5, 5.5);              // right_down
    target.emplace_back(Point2f(-0.5f, 5.5));   // left_down

    intersections.emplace_back(left_up);
    intersections.emplace_back(right_up);
    intersections.emplace_back(right_down);
    intersections.emplace_back(left_down);

    // #### SOURCE ####

    auto rec = boundingRect(inter);

    if (rec.height < 0 || rec.height > _original.cols || rec.width < 0 || rec.width > _original.rows)
        return this;

    auto input = Mat(_original, rec);
    resize(input, input, Size(6 * 50, 6 * 50));
    imshow("Marker source", input);

    // ### TRANSFORMATIONS ###

    Size size(6, 6);
    Mat marker(size, _grey.type());

    // Get a homography transformation matrix
    Mat homography = getPerspectiveTransform(intersections, target);

    // Apply the matrix and transform the view
    warpPerspective(_grey, marker, homography, size);

    threshold(marker, marker, _threshold, 255, THRESH_BINARY);

    // ### COUNT THE ID VALUE ###

    int id = 0;

    for (int i = 1; i < marker.rows - 1; i++)
        for (int j = 1; j < marker.cols - 1; j++)
            id += marker.at<uchar>(i, j) == 255 ? pow(2, 4 - j) : 0;

    // ### FINAL RESULT ###

    Mat identifier(Size(70, 50), marker.type());
    identifier.setTo(_WHITE);

    char str[100];
    sprintf(str, "%d", id);
    putText(identifier, str, Point2f(5, 30), FONT_HERSHEY_PLAIN, 2, _GREEN);
    imshow("Id", identifier);

    resize(marker, marker, Size(6 * 50, 6 * 50), 0, 0, INTER_NEAREST);
    imshow("Marker", marker);

    // ### EXERCISE 5 ###

    float result[16];

    estimateSquarePose(result, &intersections[0], 0.045);

    for (auto x = std::end(result); x != std::begin(result);) std::cout << *--x << ' ';

    std::cout << endl;

    return this;
}
